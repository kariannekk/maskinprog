#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* Declaration of peripheral setup functions. */
void playSong();		//From file "sound_manager.c".

void GPIOOddInput();		//From file "gpio.c".
void GPIOEvenInput();
void toggleLEDsON();
void toggleLEDsOFF();

/* TIMER1 interrupt handler. */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Clear timer interrupt flag */
	*TIMER1_IFC = 0x1;

	/* Play next sample */
	playSong();
}

/* GPIO even pin interrupt handler. Pins are 0-indexed. */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* Clear even numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0x55;

	/* Act on input. Buttons are 1-indexed. */
	GPIOEvenInput();
}

/* GPIO odd pin interrupt handler. Pins are 0-indexed. */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* Clear odd numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0xAA;

	/* Act on input. Buttons are 1-indexed. */
	GPIOOddInput();
}

/* LETIMER0 interrupt handler. */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
	*LETIMER0_IFC = LETIMER0_IFC_UNDERFLOW;
	toggleLEDsON();
	toggleLEDsOFF();
}
