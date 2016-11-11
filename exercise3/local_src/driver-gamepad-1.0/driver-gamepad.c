/*
 * Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/compiler.h>
#include <asm/io.h>

#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/timer.h>

#include "efm32gg.h"

#define DEVICE_NAME "gamepad"
#define CLASS_NAME "gamepad"

/* Platform device data for TDT4258:
|	IRQ Source	|	IRQ number	|	Platform IRQ index	|	Platform mem index	|
|	GPIO Even	|		17		|			0			|			0			|
|	GPIO Odd	|		18		|			1			|			0			|
|	Timer 3		|		19		|			2			|			1			|
|	DMA			|		20		|			3			|			2			|
|	DAC			|		21		|			4			|			3			|
*/

/* Global variables */
struct resource *resGPIOEvenOdd, *resTimer3, *resDMA, *resDAC;
void *baseAddrResGPIOEvenOdd, *baseAddrResTimer3, *baseAddrResDMA, *baseAddrResDAC;
int irqGPIOEven, irqGPIOOdd, irqTimer3, irqDMA, irqDAC;
struct cdev gamepadCdev; 
static dev_t devno;
struct class *gamepadClass;
struct fasync_struct *async_queue;
int err;

static struct timer_list gameTimer;

/*******************/
/* File operations */
/*******************/


static ssize_t gamepadDriverRead (struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	
	uint32_t buttonData = ioread32(GPIO_PC_DIN);
	//int buttonData = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &buttonData, 1);

	return 1;
}

static ssize_t gamepadDriverWrite (struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	char input;
	
	printk(KERN_INFO "Starting/stopping timer!\n");
		
	if (count == 0){ //Empty input
		return 0;
	}
	
	copy_from_user(&input, buff, count);

	printk("Input %c\n", input);

	switch(input){
		case '0': //Stop Timer3
			iowrite32(0x2, TIMER3_CMD);
			break;
		case '1': //Start Timer3
			iowrite32(0x1, TIMER3_CMD);
			break;
		case '2': //Start kernel timer
			printk(KERN_INFO "Starting timer to fire in 100ms (%ld)\n", jiffies );
			err = mod_timer( &gameTimer, jiffies + msecs_to_jiffies(100) );
			if (err) printk("Error in mod_timer\n");
			break;
		default:
			return 0;
	}

	return count;
}

static int gamepadDriverFasync(int fd, struct file *filp, int mode)
{
	printk(KERN_INFO "Setting up fasync\n");
	
	return fasync_helper(fd, filp, mode, &async_queue);
}

static int gamepadDriverRelease (struct inode *inode, struct file *filp)
{
	/* Remove filp from the asynchronously notifies filp's */
	gamepadDriverFasync(-1, filp, 0);
	
	printk(KERN_INFO "Gamepad driver released!\n");
	return 0;
}

static int gamepadDriverOpen (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Gamepad driver opened!\n");
	
	return 0;
}

	
/* Set up file operations */
static struct file_operations gamepad_fops = {
	.owner   = THIS_MODULE,
	.read    = gamepadDriverRead,
	.write   = gamepadDriverWrite, 
	.open    = gamepadDriverOpen, 
	.fasync  = gamepadDriverFasync,
	.release = gamepadDriverRelease
};

/*****************/
/* Timer handler */
/*****************/
void gameTimerDone( unsigned long data )
{
//	printk( "Kernel ball timer called (%ld).\n", jiffies );
//	mod_timer( &gameTimer, jiffies + msecs_to_jiffies(100));
  	if(async_queue){
		kill_fasync(&async_queue, SIGIO, POLL_OUT);
	}
}

/*********************/
/* Interrupt handler */
/*********************/

/* GPIO Interrupt handler */
irqreturn_t GPIOInterruptHandler(int irq, void* dev_id, struct pt_regs* regs)
{
	printk(KERN_ALERT "GPIO interrupt\n");
	iowrite32(0xff, GPIO_IFC); //Clear interrupt flag

	/* Signal asynchronous readers */
	if(async_queue){
		kill_fasync(&async_queue, SIGIO, POLL_IN);
	}

	return IRQ_HANDLED;
}

irqreturn_t TIMERInterruptHandler(int irq, void* dev_id, struct pt_regs* regs)
{
	printk(KERN_ALERT "TIMER3 interrupt\n");	
	iowrite32(0x01, TIMER3_IFC);
	
	//iowrite32(0x2, TIMER3_CMD);
	
	if(async_queue){
		kill_fasync(&async_queue, SIGIO, POLL_OUT);
	}
		
	return IRQ_HANDLED;
}

/*******************/
/* Platform driver */
/*******************/

/* Initialize/set up/allocate */
static int gamepadDriverProbe(struct platform_device *dev)
{
	/**********************/
	/* Set up char device */
	/**********************/
	
	/* Allocate device number */
	alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
	
	/* Initialization of char device structure */
	cdev_init (&gamepadCdev, &gamepad_fops);
	gamepadCdev.owner = THIS_MODULE;

	/* Pass cdev structure to kernel */
	err = cdev_add(&gamepadCdev, devno, 1);
	if (err) {
		printk(KERN_ALERT "Failed to add cdev to the kernel\n");
		return -1;
	}
	
	/***********************/
	/* Allocate I/O memory */
	/***********************/
	
	/* Find the I/O register base address */
	printk(KERN_INFO "Initialize hardware and allocate i/o memory\n");
	
	resGPIOEvenOdd = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if(unlikely(!resGPIOEvenOdd)){
		pr_err("Resource GPIOEvenOdd not available \n");
		return -1;
	}
	printk(KERN_INFO "\n Memory area GPIOEvenOdd\n");
	printk(KERN_INFO "Start:%lx, End:%lx, Size:%d", (unsigned long)resGPIOEvenOdd->start, (unsigned long)resGPIOEvenOdd->end, resource_size(resGPIOEvenOdd));
	
	resTimer3 = platform_get_resource(dev, IORESOURCE_MEM, 1);
	if(unlikely(!resTimer3)){
		pr_err("Resource Timer 3 not available \n");
		return -1;
	}
	printk(KERN_INFO "\n Memory area Timer 3\n");
	printk(KERN_INFO "Start:%lx, End:%lx, Size:%d", (unsigned long)resTimer3->start, (unsigned long)resTimer3->end, resource_size(resTimer3));
	

	resDMA = platform_get_resource(dev, IORESOURCE_MEM, 2);
	if(unlikely(!resDMA)){
		pr_err("Resource DMA not available \n");
		return -1;
	}
	printk(KERN_INFO "\n Memory area DMA\n");
	printk(KERN_INFO "Start:%lx, End:%lx, Size:%d", (unsigned long)resDMA->start, (unsigned long)resDMA->end, resource_size(resDMA));
	
	resDAC = platform_get_resource(dev, IORESOURCE_MEM, 3);
	if(unlikely(!resDAC)){
		pr_err("Resource DAC not available \n");
		return -1;
	}
	printk(KERN_INFO "\n Memory area DAC\n");
	printk(KERN_INFO "Start:%lx, End:%lx, Size:%d", (unsigned long)resDAC->start, (unsigned long)resDAC->end, resource_size(resDAC));
	
	
	
	/* I/O Memory Allocation -- ask to access hardware*/
	if( (request_mem_region(resGPIOEvenOdd->start, resource_size(resGPIOEvenOdd), dev->name)) == NULL){
		printk(KERN_ALERT "Unable to obtain I/O memory address for GPIOEvenOdd\n");
		return -1;
	}
	
	if( (request_mem_region(resTimer3->start, resource_size(resTimer3), dev->name)) == NULL){
		printk(KERN_ALERT "Unable to obtain I/O memory address for Timer 3\n");
		return -1;
	}
	
	if( (request_mem_region(resDMA->start, resource_size(resDMA), dev->name)) == NULL){
		printk(KERN_ALERT "Unable to obtain I/O memory address for DMA\n");
		return -1;
	}
	
	if( (request_mem_region(resDAC->start, resource_size(resDAC), dev->name)) == NULL){
		printk(KERN_ALERT "Unable to obtain I/O memory address for DAC\n");
		return -1;
	}
	
	/* I/O Memory mapping - Map to virtual memory */
	baseAddrResGPIOEvenOdd = ioremap_nocache(resGPIOEvenOdd->start, resource_size(resGPIOEvenOdd));
	if(unlikely(!baseAddrResGPIOEvenOdd)){
		printk(KERN_ALERT " Cannot map I/O GPIOEvenOdd\n");
		return -1;
	}
	
	baseAddrResTimer3 = ioremap_nocache(resTimer3->start, resource_size(resTimer3));
	if(unlikely(!baseAddrResTimer3)){
		printk(KERN_ALERT " Cannot map I/O Timer 3\n");
		return -1;
	}	

	baseAddrResDMA = ioremap_nocache(resDMA->start, resource_size(resDMA));
	if(unlikely(!baseAddrResDMA)){
		printk(KERN_ALERT " Cannot map I/O DMA\n");
		return -1;
	}

	baseAddrResDAC = ioremap_nocache(resDAC->start, resource_size(resDAC));
	if(unlikely(!baseAddrResDAC)){
		printk(KERN_ALERT " Cannot map I/O DAC\n");
		return -1;
	}

	
	/******************/
	/* Enable buttons */
	/******************/
	
	printk(KERN_INFO "Enabling buttons\n");
	iowrite32(0x33333333, GPIO_PC_MODEL);	//Enables input with filter
	iowrite32(0xFF, GPIO_PC_DOUT);			//Enables pull-up resistors

	/*************************/
	/* Setup and enable IRQs */
	/*************************/
	
	/* Find the IRQ number */
	irqGPIOEven = platform_get_irq(dev, 0);
	irqGPIOOdd  = platform_get_irq(dev, 1);
	irqTimer3   = platform_get_irq(dev, 2);
	irqDMA      = platform_get_irq(dev, 3);
	
	/* Allocate and instansiate irq's */
	printk(KERN_INFO "Initializing GPIO odd/even, Timer3, DMA and DAC interrupts\n");


	/* Enable GPIO interrupt */
	if(request_irq(irqGPIOEven, (irq_handler_t)GPIOInterruptHandler, 0, DEVICE_NAME, &gamepadCdev) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Even\n");
		return -1;
	}

	if(request_irq(irqGPIOOdd, (irq_handler_t)GPIOInterruptHandler, 0, DEVICE_NAME, &gamepadCdev) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}

	iowrite32(0x22222222, GPIO_EXTIPSELL);	//Selects port C for interrupts
	iowrite32(0xFF, GPIO_EXTIFALL);			//Enables falling edge detection
	iowrite32(0xFF, GPIO_IFC);				//Clears external interrupt flags
	iowrite32(0xFF, GPIO_IEN);				//Enables external interrupts

	/* Enable Timer 3 interrupt */
	if(request_irq(irqTimer3, (irq_handler_t)TIMERInterruptHandler, 0, DEVICE_NAME, &gamepadCdev) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}
	
	iowrite32(31700, TIMER3_TOP);
	iowrite32(0x1, TIMER3_IEN);
	
	/**************/
	/* Set up DAC */
	/**************/

	iowrite32(0x50010, DAC0_CTRL);
	iowrite32(0x1, DAC0_CH0CTRL);
	iowrite32(0x1, DAC0_CH1CTRL);

	/***********************/
	/* Set up kernel timer */
	/***********************/	
	printk(KERN_INFO "Instanziating Kernel game timer.\n");

	setup_timer( &gameTimer, gameTimerDone, 0 );


	/********************************/
	/* Driver Visible to User Space */
	/********************************/
	gamepadClass = class_create(THIS_MODULE, CLASS_NAME);
	device_create(gamepadClass, NULL, devno, NULL, CLASS_NAME);

	return 0;
}

/* Deallocate everything allocated in the probe-function */
static int gamepadDriverRemove(struct platform_device *dev)
{
	/* Free device number */
	unregister_chrdev_region(devno, 1);
	
	/* Delete class */
	class_destroy(gamepadClass);
	
	/* Delete cdev */
	cdev_del(&gamepadCdev);
		
	/* Free I/O memory*/
	release_mem_region(resGPIOEvenOdd->start, resource_size(resGPIOEvenOdd));
	release_mem_region(resTimer3->start, resource_size(resTimer3));
	release_mem_region(resDMA->start, resource_size(resDMA));
	release_mem_region(resDAC->start, resource_size(resDAC));
	
	/* Free mapped I/O memory */
	iounmap(baseAddrResGPIOEvenOdd);
	iounmap(baseAddrResTimer3);
	iounmap(baseAddrResDMA);
	iounmap(baseAddrResDAC);
	
	/* Free IRQs */
	free_irq(0, &gamepadCdev);
	free_irq(1, &gamepadCdev);
	free_irq(2, &gamepadCdev);
	free_irq(3, &gamepadCdev);
	free_irq(4, &gamepadCdev);
	
	/* Delete kernel game timer */
	err = del_timer(&gameTimer);
	if (err) printk(KERN_ALERT "The timer is still in use!\n");

	
	return 0;
}






/* Set up platform device */
static const struct of_device_id my_of_match[] = {
	{.compatible = "tdt4258", },
	{ },
};

MODULE_DEVICE_TABLE(of, my_of_match);

/* Set up platform driver */
static struct platform_driver gamepadDriver = {
	.probe  = gamepadDriverProbe,
	.remove = gamepadDriverRemove,
	.driver = {
		.name  = DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = my_of_match,
	},
};

/*
 * template_init - function to insert this module into kernel space
 *

 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *

 * Returns 0 if successfull, otherwise -1
 */
static int __init gamepadDriverInit(void)
{
	printk("Init gamepad driver!\n");
	
	/* Register with Kernel */
	platform_driver_register(&gamepadDriver);
	
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */
static void __exit gamepadDriverCleanup(void)
{
	 printk("Short life for a small module...\n");

	/* Unregister with Kernel */
	platform_driver_unregister(&gamepadDriver);
}


module_init(gamepadDriverInit);
module_exit(gamepadDriverCleanup);

MODULE_DESCRIPTION("platform driver for gamepad");
MODULE_LICENSE("GPL"); //"General purpose license"

