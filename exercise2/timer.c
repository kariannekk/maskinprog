#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* TIMER1 sends a sample to the DAC at every overflow until the song is finished. TIMER1 cannot operate in EM2 or higher. */

/* Enable Timer1. It uses the HF Clock at 14 MHz. */
void setupTimer(uint16_t period)
{
	/* Enable Timer clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

	/* Set amount of count ticks. */
	*TIMER1_TOP = period;	//Sets timer period. 
}

void setupTimerInterrupt()
{
	*TIMER1_IFC = 0x1;	//Clears overflow interrupt.
	*TIMER1_IEN = 0x1;	//Enables overflow interrupt.
}
