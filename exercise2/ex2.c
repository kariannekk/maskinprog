#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound waves you want to generate. The core clock (which the timer clock is derived from) runs at 14 MHz by default. Also remember that the timer counter registers are 16 bits.
*/

/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   317

#define RIGHT 1			//Consider making enumerator.
#define LEFT -1

/* Declaration of peripheral setup functions */
void setupGPIO();		//From file "gpio.c"
void setupGPIOInterrupts();
int readGPIOInput();

void setupTimer(uint32_t period);	//From file "timer.c"
void setupTimerInterrupt();
void startTimer();

void setupDAC();		//From file "dac.c"

void setupInterrupt();		//From file "ex2.c"
void setupSleepMode();

void setSong(int **input_song);	//From file "sound_manager.c"
int playSong();

/* Temporary peripheral decalrations. */
void my_polling_programA();
void my_polling_programB();
void moveLight(int direction);
void selectSongFromButton(int input_button);

/* Main program code. */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	setupInterrupt();

	/* Enable energy efficiency. */
	//setupSleepMode();

	/* Run main program. */
	while (1) {
	};
	//my_polling_programA();

	return 0;
}

void my_polling_programA()
{
	int input_button = 0;
	int buttonReleased = 1;

	/* Play opening song. */
	//setSong((int**)1);
	//set variables = no other buttons pressed. 

//      startTimer();
	/* Play sound from button presses. */
	while (1) {
		if (*TIMER1_CNT == *TIMER1_TOP) {
			playSong();
		}
		if ((input_button = readGPIOInput())) {
			if (buttonReleased) {
				buttonReleased = 0;
				selectSongFromButton(input_button);
				if (input_button == 1) {
					moveLight(LEFT);
				} else if (input_button == 3) {
					moveLight(RIGHT);
				}
			}
		} else {
			buttonReleased = 1;
		}
	}
}

void my_polling_programB()
{
	while (1) {
		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
		if (*TIMER1_CNT == *TIMER1_TOP) {
			playSong();
		}
		if (!(*GPIO_PC_DIN & 1))	//stop all actions.
		{		//button pair: 1, 2             //e.g. stop noise.
			while (1) {
			}
		}
		if (!(*GPIO_PC_DIN & 2))	//set song. 
		{		//button pair: 1, 2
			setSong((int **)1);	//testsong);
		}
	}
}

void setupNVIC()
{
	/* Set NVIC ISERx register for interrupt enable. */
	*ISER0 |= 0x1802;	//Enables GPIO odd and even interrupts, and TIMER1 interrupts. 
}

void setupInterrupt()
{
	setupGPIOInterrupts();
	setupTimerInterrupt();
	setupNVIC();
}

void setupSleepMode()
{
	/* Sleep mode */
	*SCR |= 0x6;		//Enables sleep mode for CPU.

	//wfi;          //What is the non-assembler version of this? Do we have alternatives?
	//TODO Disable timer & dac. Or use sleep mode for automatic. 
}

/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
