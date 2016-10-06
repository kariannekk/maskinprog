#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void runDAC();

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
//      runDAC(1200);
	*GPIO_PA_DOUT = 0x1100;	//temp signal. 

	*TIMER1_IFC = 1;	//altern: = *TIMER1_IF;         //Clears all interrupt flags.
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */

	*GPIO_PA_DOUT = 0xAA00;	//temp signal.

	*GPIO_IFC |= 0xAA;	//Clears even numbered interrupt flags (1-indexed).
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */

	*GPIO_PA_DOUT = 0x5500;	//temp signal.

	*GPIO_IFC |= 0x55;	//Clears odd numbered interrupt flags (1-indexed).
}