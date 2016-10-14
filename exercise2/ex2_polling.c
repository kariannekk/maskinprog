#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* The period between sound samples, in clock cycles. */
#define   SAMPLE_PERIOD   10

/* Declaration of peripheral setup functions. */
void setupGPIO();		//From file "gpio.c"
void GPIOOddInput();
void GPIOEvenInput();

void setupTimer(uint32_t period);	//From file "timer.c"

void setupDAC();		//From file "dac.c"

void playSong();		//From file "sound_manager.c"
void playIntroSong();

void pollingProgram();		//From file "ex2.c"

/* Main program code. */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Play opening song. */
	playIntroSong();

	/* Run main program. */
	pollingProgram();

	return 0;
}

void pollingProgram()
{
	bool buttonReleased = true;	//Ensures buttons must be re-pressed to restart their action. 

	while (true) {
		/* Play sound when there is a song. */
		if (*TIMER1_CNT == *TIMER1_TOP) {
			playSong();
		}
		/* Set songs and operate LEDs. */
		if (*GPIO_PC_DIN != 0xFF) {
			if (buttonReleased) {
				buttonReleased = false;
				GPIOOddInput();
				GPIOEvenInput();
			}
		} else {
			buttonReleased = true;
		}
	}
}

/* Activate sleep mode (EM1). This does not disable TIMER1. */
void setNormalSleepMode()
{
	/* Polling version never enters sleep mode anyway. */
	//*SCR = 0x2;           //Enables sleep mode for CPU.
	*DAC0_CH0CTRL = 0x1;	//Enable right audio channels. 
	*DAC0_CH1CTRL = 0x1;	//Enable left audio channels.
	*TIMER1_CMD = 0x1;	//Starts the timer.
}

/* Activate deep sleep mode (EM2). This disables TIMER1. */
void setDeepSleepMode()
{
	/* Polling version never enters sleep mode anyway. */
	*TIMER1_CMD = 0x2;	//Stops the timer. 
	//*SCR = 0x6;           //Enables deep sleep mode for CPU.
	*DAC0_CH0CTRL = 0x0;	//Disable right audio channels. 
	*DAC0_CH1CTRL = 0x0;	//Disable left audio channels. 
}
