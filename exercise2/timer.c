#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer. Maximum input period is 0xFFFF. */
void setupTimer(uint16_t period)
{
	/*
	   This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
	 */

	/* Enable Timer clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

	/* Set number of count ticks */
	*TIMER1_TOP = period;	//Sets timer period. Max is 0xFFFF.

	/* Start/stop the timer. */
	*TIMER1_CMD = 0x1;	//0x1 start, 0x2 stops, 0x0 does naught.
}

void setupTimerInterrupts(){
	/* Enable interrupt */
	*TIMER1_IEN = 0x1;	//Enables overflow interrupt. Read more 20.5.4 [13].
}
