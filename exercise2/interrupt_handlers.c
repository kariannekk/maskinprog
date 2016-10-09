#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setSong(int ** inpit_song);
void runThis();

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Remove timer interrupt flag */
	*TIMER1_IFC = 1;

	/* Play next sample */
	runThis();

	/* Stop timer when song is finished*/
	//if(current_song == 0){
	//	*TIMER1_CMD = 0x2;
	//}

}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC |= 0xAA;	//Clears even numbered interrupt flags (1-indexed).
	
	GPIOHandler();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC |= 0x55;	//Clears odd numbered interrupt flags (1-indexed).
	
	GPIOHandler();
}

void GPIOHandler(){
	
	buttonNumber = readGPIOInput();
	
	/* If only one button is pushed, continue */
	if(buttonNumber != 0){
		/* Set song */
		
		if(current_song != 0){
			/* Start timer */
			*TIMER1_CMD = 0x1;
		}
		

		
	}
}