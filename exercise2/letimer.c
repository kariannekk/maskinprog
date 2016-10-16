#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* LETIMER0 is used as an alternative to TIMER1 in sending samples to the DAC. This is to compare energy efficiency for both. LETIMER0 runs too slow for a sample ratio of 44100/s, however, but the energy comparison should be little affected. LETIMER0 can operate in EM2, but is less accurate than TIMER1. */

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
	*LETIMER0_IFC = LETIMER0_IFC_UNDERFLOW;
	*LETIMER0_IEN = LETIMER0_IEN_UNDERFLOW;

	/* Start LETIMER0. */
	*LETIMER0_CMD = LETIMER0_CMD_START;
}
