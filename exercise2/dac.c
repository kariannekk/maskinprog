#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	/*
	   TODO *When entering Energy Mode 4, both DAC channels must be stopped. [s712; 13]
	   *One conversion takes 2 DAC_CLK cycles and the DAC_CLK should not be set higher than 1 MHz. [s712; 13]
	 */

	/* Enable clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;

	/* Enable input and output channels. */
	*DAC0_CTRL = 0x50010;	//Prescale DAC clock with CMU clock divided by factor of 2⁵. DAC output to pin enabled. Add 0x4 for sample/hold instead of continuous conversion mode. 
	*DAC0_CH0CTRL = 0x1;	//Enable right audio channels. 
	*DAC0_CH1CTRL = 0x1;	//Enable left audio channels. 
}
