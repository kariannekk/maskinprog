#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*GPIO_PA_DOUT = 0xF000;
	
	
	
	*TIMER1_IFC = 1;
	
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_PA_DOUT = 0xAA00;	//temp signal
	
	
	
	
	*GPIO_IFC |= 0xAA;	//Even pins clear (1-indexed).
	/* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_PA_DOUT = 0x5500;	//temp signal
	
	
	
	
	
	*GPIO_IFC |= 0x55;	//Odd pins clear (1-indexed).
	/* TODO handle button pressed event, remember to clear pending interrupt */
}
