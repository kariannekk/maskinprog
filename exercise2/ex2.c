#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* The period between sound samples, in clock cycles. */
#define   SAMPLE_PERIOD   317

/* Declaration of peripheral setup functions. */
void setupGPIO();		//From file "gpio.c"
void setupGPIOInterrupts();
void GPIOOddInput();
void GPIOEvenInput();

void setupTimer(uint32_t period);	//From file "timer.c"
void setupTimerInterrupt();

void setupDAC();		//From file "dac.c"

void playSong();		//From file "sound_manager.c"
void playIntroSong();

void setupInterrupt();		//From file "ex2.c"
void pollingProgram();

/* Main program code. */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	//setupInterrupt();

	/* Play opening song. */
	playIntroSong();

	/* Run main program. */
	//__asm("wfi"); //Wait for interrupt. 

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

/* Set NVIC ISERx register for interrupt enable. */
void setupNVIC()
{
	*ISER0 |= 0x1802;	//Enables GPIO odd and even interrupts, and TIMER1 interrupts. 
}

/* Activate interrupt mode. */
void setupInterrupt()
{
	setupGPIOInterrupts();
	setupTimerInterrupt();
	setupNVIC();
}

/* Activate sleep mode (EM1). This does not disable TIMER1. */
void setNormalSleepMode()
{
	*SCR = 0x2;		//Enables sleep mode for CPU.
	*DAC0_CH0CTRL = 0x1;	//Enable right audio channels. 
	*DAC0_CH1CTRL = 0x1;	//Enable left audio channels.
	*TIMER1_CMD = 0x1;	//Starts the timer.
}

/* Activate deep sleep mode (EM2). This disables TIMER1. */
void setDeepSleepMode()
{
	*TIMER1_CMD = 0x2;	//Stops the timer. 
	*SCR = 0x6;		//Enables deep sleep mode for CPU.
	*DAC0_CH0CTRL = 0x0;	//Disable right audio channels. 
	*DAC0_CH1CTRL = 0x0;	//Disable left audio channels. 
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
