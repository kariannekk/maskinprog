#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define RIGHT 1
#define LEFT -1

int playSong();	//From file "sound_manager.c".
int selectSongFromButton(int input_button);

int readGPIOInput();	//From file "gpio.c".
void moveLight(int direction);

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Clear timer interrupt flag */
	*TIMER1_IFC = 1;
	
	/* Play next sample */
	playSong();
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* Clear even numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0x55;
	
	/* Read input. */
	int input_button = readGPIOInput();	//Prevents several simultaneous buttons. 
	switch (input_button){
	case 0:
		return;
	case 1:
		moveLight(LEFT);
		break;
	case 3:
		moveLight(RIGHT);
		break;
	default:
		selectSongFromButton(input_button);
		break;
	}
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* Clear odd numbered interrupt flags (0-indexed). */
	*GPIO_IFC |= 0xAA;
	
	/* Read input. */
	int input_button = readGPIOInput();	//Prevents several simultaneous buttons. 
	switch (input_button){
	case 0:
		return;
	default:
		selectSongFromButton(input_button);
		break;
	}
}
