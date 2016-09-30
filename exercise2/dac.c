#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	/*
	   TODO enable and set up the Digital-Analog Converter

	   3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
	   4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
		   4. Alternatively by writing to DAC0_COMBDATA (I guess this is for identical output left and right). 
	   
	   Warnings/tips: 
	   		*When entering Energy Mode 4, both DAC channels must be stopped. [s712; 13]
	   		*One conversion takes 2 DAC_CLK cycles and the DAC_CLK should not be set higher than 1 MHz. [s712; 13]
	 */

	/* Enable clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;
	
	/* Enable input and output channels. */
	*DAC0_CTRL = 0x50010;	//Prescale DAC clock. Clock division factor of 2⁵. DAC output to pin enabled. 
	*DAC0_CH0CTRL = 0x1;	//Enable left audio channels. 
	*DAC0_CH1CTRL = 0x1;	//Enable right audio channels. 
				//Guessed left vs right. 

	//TODO while?? Timer-trigger? Surely not in this function; setupDAC() should setup, then exit. A runDAC() is more suitable for continously running while polling. Timer interrupt is suitable for second implementation. 

}

void runDAC(uint16_t sampleAmount)
	{
	uint16_t SAMPLE_MEDIAN = 0x400;		//Appears to be 'dont-care' value.
	uint16_t amplitudeStep = 0x1;	//Volume.
	//uint16_t sampleAmount = 800;		//Frequency, somewhat. Approximate min: 30. Approximate max: 1500. will make ugly noise further than 1500. 
	
	for (int i=0; i<sampleAmount; i++)
	{
		*DAC0_CH0DATA = SAMPLE_MEDIAN + (i * amplitudeStep);
	}
	
	for (int i=sampleAmount; i>0; i--)
	{
		*DAC0_CH0DATA = SAMPLE_MEDIAN + (i * amplitudeStep);
	} 
}
