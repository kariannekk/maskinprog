#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
void startTimer();

/* Enable internal timer in the program. Maximum input period is 0xFFFF. */
void setupTimer(uint16_t period)
{
	/* Enable Timer clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;

	/* Set amount of count ticks. */
	*TIMER1_TOP = period;	//Sets timer period. 

	/* Start/stop the timer. */
	// This property has been distributed elsewhere in the code. Keep it here as a comment for future reference. 
	//*TIMER1_CMD = 0x1;	//0x1 starts, 0x2 stops.
}

void setupTimerInterrupt()
{
	*TIMER1_IFC = 0x1;
	*TIMER1_IEN = 0x1;	//Enables overflow interrupt.
}

//void enableTimer()
//{
//	/* Enable Timer clock. */
//	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
//}

//void disableTimer()
//{
//	/* Disable Timer clock. */
//	*CMU_HFPERCLKEN0 &= ~(CMU2_HFPERCLKEN0_TIMER1);
//	/* Disable interrupt */
//	//*TIMER1_IEN = 0x0;
//}
