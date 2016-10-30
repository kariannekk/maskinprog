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

#include "efm32gg.h"

#define DEVICE_NAME "gamepad"

/* Platform device data for TDT4258:
|	IRQ Source	|	IRQ number	|	Platform IRQ index	|	Platform mem index	|
|	GPIO Even	|		17		|			0			|			0			|
|	GPIO Odd	|		18		|			1			|			0			|
|	Timer 3		|		19		|			2			|			1			|
|	DMA			|		20		|			3			|			2			|
|	DAC			|		21		|			4			|			3			|
*/

/* GPIO Interrupt handler */
irqreturn_t GPIOInterruptHandler(int irq, void* dev_id, struct pt_regs* regs)
{
	printk(KERN_ALERT "GPIO interrupt\n");
	iowrite32(0xff, GPIO_IFC); //Clear interrupt flag

	//TO DO: asyncronic queue

	return IRQ_HANDLED;
}


/* Initialize/set up/allocate */
static int gamepadDriverProbe(struct platform_device *dev)
{

	/* Variables */
	void *baseAddrResGPIOEvenOdd, *baseAddrResTimer3, *baseAddrResDMA, *baseAddrResDAC;
	struct resource *resGPIOEvenOdd, *resTimer3, *resDMA, *resDAC;
	
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
	printk(KERN_ALERT "\n Memory area GPIOEvenOdd\n");
	printk(KERN_ALERT "Start:%lx, End:%lx, Size:%d", (unsigned long)resGPIOEvenOdd->start, (unsigned long)resGPIOEvenOdd->end, resource_size(resGPIOEvenOdd));
	
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
	
	/* I/O Memory Allocation */
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
	
	/* I/O Memory mapping - Convert the physical address to virtual address */

	
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

	
	/**************************/
	/* Setup IRQs and buttons */
	/**************************/
	
	printk(KERN_INFO "Enabling buttons\n");
	iowrite32(0x33333333, GPIO_PC_MODEL);	//Enables input with filter
	iowrite32(0xFF, GPIO_PC_DOUT);			//Enables pull-up resistors
	
	/* Find the IRQ number */
	int irqGPIOEven = platform_get_irq(dev, 0);
	int irqGPIOOdd  = platform_get_irq(dev, 1);
	int irqTimer3   = platform_get_irq(dev, 2);
	int irqDMA      = platform_get_irq(dev, 3);
	int irqDAC      = platform_get_irq(dev, 4);
	
	/* Allocate and instansiate irq's */
	printk(KERN_INFO "Initializing GPIO odd/even, Timer3, DMA and DAC interrupts\n");

	/* Enable GPIO interrupt */
	iowrite32(0x22222222, GPIO_EXTIPSELL);	//Selects port C for interrupts
	iowrite32(0xFF, GPIO_EXTIFALL);			//Enables falling edge detection
	iowrite32(0xFF, GPIO_IFC);				//Clears external interrupt flags
	iowrite32(0xFF, GPIO_IEN);				//Enables external interrupts

	if(request_irq(irqGPIOEven, (irq_handler_t)GPIOInterruptHandler, NULL, DEVICE_NAME, NULL) < 0){ // siste param = void *dev_id
		printk(KERN_ALERT "IRQ request failed for GPIO Even\n");
		return -1;
	}

	if(request_irq(irqGPIOOdd, (irq_handler_t)GPIOInterruptHandler, NULL, DEVICE_NAME, NULL) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}
	
	/* Enable Timer 3 interrupt */
//	iowrite(???, ????);
/*	if(request_irq(irqTimer3, (irq_handler_t)TIMERInterruptHandler, NULL, DEVICE_NAME, void *dev_id) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}*/

	/* Enable DMA intterrupt */
//	iowrite(???, ????);
/*	if(request_irq(irqTimer3, (irq_handler_t)TIMERInterruptHandler, NULL, DEVICE_NAME, void *dev_id) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}*/
	
	/* Enable DAC interrupt */
//	iowrite(???, ????);
/*	if(request_irq(irqTimer3, (irq_handler_t)TIMERInterruptHandler, NULL, DEVICE_NAME, void *dev_id) < 0){
		printk(KERN_ALERT "IRQ request failed for GPIO Odd\n");
		return -1;
	}*/	

	/**********************/
	/* Set up char device */
	/**********************/


	/* Make the driver visible to user space */
//	struct class *cl;
//	dev_t devno;
//	cl = class_create(THIS_MODULE, CLASS_NAME);
//	device_create(cl, NULL, devno, NULL, CLASS_NAME);

	return 0;
}

/* Deallocate everything allocated in the probe-function */
static int gamepadDriverRemove(struct platform_device *dev)
{
	return 0;
}

static const struct of_device_id my_of_match[] = {
	{.compatible = "tdt4258", },
	{ },
};

MODULE_DEVICE_TABLE(of, my_of_match);


/* Structures */
static struct platform_driver gamepadDriver = {
	.probe  = gamepadDriverProbe,
	.remove = gamepadDriverRemove,
	.read = gamepadDriverRead, 
	.driver = {
		.name  = DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = my_of_match,
	},
};
struct cdev gamepadDriverCdev; 



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

	int err = alloc_chrdev_region(&devno, 0, 1, DRIVER);

	/* Initialization of char device structure aka cdev */
	cdev_init (&gamepadDriverCdev, &gamepadDriver);
	gamepadDriverCdev.owner = THIS_MODULE;


	/* Pass cdev structure to kernel */
	err = cdev_add(&gamepadDriverCdev, devno, 1);
	

	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */



static ssize_t gamepadDriverRead (struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	uint32_t buttonData = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &buttonData, 1)

	return 1;
}





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

