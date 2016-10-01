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

/* Creates a sawtooth-wave (trekantbølge). */
void runDAC(uint16_t sampleAmount)
{
	uint16_t SAMPLE_MEDIAN = 0x400;	//Appears to be 'dont-care' value. This is the min-value of the wave. 
	uint16_t amplitudeStep = 0x1;	//Volume-ish. Actually the step added sampleAmount times to the min-value. 
	//uint16_t sampleAmount = 800;          //Frequency, somewhat. Approximate min: 30. Approximate max: 1500. will make ugly noise further than 1500. 

	for (int i = 0; i < sampleAmount; i++) {
		*DAC0_CH0DATA = SAMPLE_MEDIAN + (i * amplitudeStep);
		//*DAC0_CH1DATA = SAMPLE_MEDIAN + 0x200 + (i * 2 * amplitudeStep);
	}

	for (int i = sampleAmount; i > 0; i--) {
		*DAC0_CH0DATA = SAMPLE_MEDIAN + (i * amplitudeStep);
		//*DAC0_CH1DATA = SAMPLE_MEDIAN + 0x200 + (i * 2 * amplitudeStep);
	}
}


void playArray(uint32_t *inputList, uint16_t length)
{
	/*
	uint16_t a[] = { 127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 234, 239, 242, 245, 248, 250, 252, 253, 254, 254, 254, 254, 252, 251, 248, 246, 243, 239, 235, 231, 226, 220, 215, 209, 203, 196, 189, 182, 175, 167, 160, 152, 144, 136, 128, 120, 112, 104, 96, 89, 81, 74, 67, 60, 53, 47, 41, 35, 30, 25, 20, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 0, 2, 3, 5, 8, 11, 15, 19, 23, 28, 33, 39, 44, 51, 57, 64, 71, 78, 86, 94, 101, 109, 117 };
	uint16_t a_length = 100;
	*/
	
	for (int i=0; i<length; i++)
	{
		*DAC0_CH0DATA = inputList[i];
	}
}
