#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define RIGHT 1
#define LEFT -1

int playSong();	//From file "sound_manager.c".
int selectSongFromButton(int input_button);

int readGPIOInput();	//From file "gpio.c".
void moveLight(int direction);
void toggleLEDsON();
void toggleLEDsOFF();


//TODO Disable timer & dac. Or use sleep mode for automatic. 


/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Clear timer interrupt flag */
	*TIMER1_IFC = 0x1;
	
	/* Play next sample */
	playSong();
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* Clear even numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0x55;
	
	/* Act on input. */
	switch (*GPIO_PC_DIN) {
		case 0xFE:
			moveLight(LEFT);
			return;
		case 0xFB:
			moveLight(RIGHT);
			return;
		case 0xEF:
			selectSongFromButton(5);	
			return;
		case 0xBF:
			selectSongFromButton(7);
			return;
		default:
			return;
	}
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* Clear odd numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0xAA;
	
	/* Act on input. */
	switch (*GPIO_PC_DIN) {
		case 0xFD:
			selectSongFromButton(2);
			return;
		case 0xF7:
			selectSongFromButton(4);
			return;
		case 0xDF:
			selectSongFromButton(6);
			return;
		case 0x7F:
			selectSongFromButton(8);
			return;
		default:
			return;
	}
}
