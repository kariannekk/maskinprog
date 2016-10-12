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

/* GPIO even pin interrupt handler. Pins are 0-indexed. */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* Clear even numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0x55;
	
	/* Act on input. Buttons are 1-indexed. */
	switch (*GPIO_PC_DIN) {
		case 0xFE:	//Button SW1. 
			moveLight(LEFT);
			return;
		case 0xFB:	//Button SW3. 
			moveLight(RIGHT);
			return;
		case 0xEF:	//Button SW5. 
			selectSongFromButton(5);	
			return;
		case 0xBF:	//Button SW7. 
			selectSongFromButton(7);
			return;
		default:
			return;
	}
}

/* GPIO odd pin interrupt handler. Pins are 0-indexed. */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* Clear odd numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0xAA;
	
	/* Act on input. Buttons are 1-indexed. */
	switch (*GPIO_PC_DIN) {
		case 0xFD:	//Button SW2. 
			selectSongFromButton(2);
			return;
		case 0xF7:	//Button SW4. 
			selectSongFromButton(4);
			return;
		case 0xDF:	//Button SW6. 
			selectSongFromButton(6);
			return;
		case 0x7F:	//Button SW8. 
			selectSongFromButton(8);
			return;
		default:
			return;
	}
}
