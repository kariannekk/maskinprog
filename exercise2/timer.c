#include "efm32gg.h"

/* TIMER1 is started when a song is set. It is stopped when the song is finished. See sound_manager.c. */

/* Enable internal timer in the program. Maximum input period is 0xFFFF. */
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
