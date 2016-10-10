#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setSong(int ** inpit_song);
void runThis();

int playing = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	
	/* Remove timer interrupt flag */
	*TIMER1_IFC = 1;

	/* Play next sample */
	playing = playSong();

	/* Stop timer when song is finished */
	if(playing == 0){
		*TIMER1_CMD = 0x2;
	}
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	GPIOHandler();

	*GPIO_IFC |= 0xAA;	//Clears even numbered interrupt flags (1-indexed).
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	GPIOHandler();
	
	*GPIO_IFC |= 0x55;	//Clears odd numbered interrupt flags (1-indexed).
}

void GPIOHandler(){
	
	buttonNumber = readGPIOInput();
	
	/* If only one button is pushed, continue */
	if(buttonNumber != 0){
		/* Set song */
		if(buttonSongSelector(buttonNumber)){ //checks button
			if(playing != 0){	//Start timer if no song is playing
				*TIMER1_CMD = 0x1;
			}
		}
		/* Set light movement */
		if(buttonNumber == 1){
			moveLight(LEFT);
		}
		else if(buttonNumber == 3){
			moveLight(RIGHT);
		}
	}
}