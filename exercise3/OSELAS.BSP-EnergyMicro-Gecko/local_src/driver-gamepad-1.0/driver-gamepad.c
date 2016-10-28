/*
 * Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/compiler.h>
#include <asm/io.h>

/* Platform device data for TDT4258:
|	IRQ Source	|	IRQ number	|	Platform IRQ index	|	Platform mem index	|
|	GPIO Even	|		17		|			0			|			0			|
|	GPIO Odd	|		18		|			1			|			0			|
|	Timer 3		|		19		|			2			|			1			|
|	DMA			|		20		|			3			|			2			|
|	DAC			|		21		|			4			|			3			|
*/

/* Initialize/set up/allocate */
static int gamepadDriverProbe(struct platform_device *dev)
{

	void *baseAddrResGPIOEvenOdd, *baseAddrResTimer3, *baseAddrResDMA, *baseAddrResDAC;
	/* Find the I/O register base address */
	struct resource *resGPIOEvenOdd, *resTimer3, *resDMA, *resDAC;
	
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
	printk(KERN_ALERT "\n Memory area Timer 3\n");
	printk(KERN_ALERT "Start:%lx, End:%lx, Size:%d", (unsigned long)resTimer3->start, (unsigned long)resTimer3->end, resource_size(resTimer3));
	

	resDMA = platform_get_resource(dev, IORESOURCE_MEM, 2);
	if(unlikely(!resDMA)){
		pr_err("Resource DMA not available \n");
		return -1;
	}
	printk(KERN_ALERT "\n Memory area DMA\n");
	printk(KERN_ALERT "Start:%lx, End:%lx, Size:%d", (unsigned long)resDMA->start, (unsigned long)resDMA->end, resource_size(resDMA));
	
	resDAC = platform_get_resource(dev, IORESOURCE_MEM, 3);
	if(unlikely(!resDAC)){
		pr_err("Resource DAC not available \n");
		return -1;
	}
	printk(KERN_ALERT "\n Memory area DAC\n");
	printk(KERN_ALERT "Start:%lx, End:%lx, Size:%d", (unsigned long)resDAC->start, (unsigned long)resDAC->end, resource_size(resDAC));
	
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
	
	/* Find the IRQ number*/
//	int irqGPIOEven = platform_get_irq(dev, 0);
//	int irqGPIOOdd  = platform_get_irq(dev, 1);
//	int irqTimer3   = platform_get_irq(dev, 2);
//	int irqDMA      = platform_get_irq(dev, 3);
//	int irqDAC      = platform_get_irq(dev, 4);
	
	/* Allocate and instansiate irq's */
//	if(request_irq(irq, irqreturn_t (*handler)(int, void *, struct pt_regs *), NULL, dev->name, void *dev_id) < 0){
//		printk(KERN_ALERT "IRQ request failed for res\n");
//		return -1;
//	}
	
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

static struct platform_driver gamepadDriver = {
	.probe  = gamepadDriverProbe,
	.remove = gamepadDriverRemove,
	.driver = {
		.name  = "gamepadDevice",
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

