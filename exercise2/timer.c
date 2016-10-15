#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* TIMER1 is started when a song is set. It is stopped when the song is finished. See sound_manager.c. */

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

/* Enable Low Energy Timer0. It uses the LF Clock at 32,768 kHz. */
void setupLETimer(uint16_t period)
{
	/* Activate LFRCO clock. */
	*CMU_OSCENCMD |= CMU_OSCENCMD_LFRCO_ACTIVATE;

	/* Select LFRCO as LF clock source. */
	*CMU_LFCLKSEL &= ~((1 << 16) | (1 << 1) | (1 << 0));
	*CMU_LFCLKSEL |= (1 << 0);

	/* Enable clock for LE peripheral interface. */
	*CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_LE;

	/* Enable LETIMER0 clock. */
	*CMU_LFACLKEN0 |= CMU_LFACLKEN0_LETIMER0;

	/* Clear LETIMER0 register. */
	*LETIMER0_CMD = LETIMER0_CMD_CLEAR;

	/* Set COMP0 register as counter top. */
	*LETIMER0_CTRL |= LETIMER0_CTRL_COMP0TOP;

	/* Set value for COMP0 register. */
	*LETIMER0_COMP0 = period;

	/* Enable interrupt. */
	*LETIMER0_IEN = LETIMER0_IEN_UNDERFLOW;

	/* Start LETIMER0. */
	*LETIMER0_CMD = LETIMER0_CMD_START;
}
