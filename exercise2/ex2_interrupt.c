#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* Select timer for sending samples. Use TIMER1 if true, LETIMER0 if false. Notes are optimized for TIMER1. */
#define SOUND_FROM_TIMER1		true

/* The period between sound samples, in clock cycles. */
#define		SAMPLE_PERIOD		317
#define		LETIMER0_PERIOD		0

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

void setupLETimer();		//From file "letimer.c"

void setupInterrupt();		//From file "ex2_interrupt.c"
void pollingProgram();

/* Main program code. */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
#if SOUND_FROM_TIMER1
	setupTimer(SAMPLE_PERIOD);
#endif

	/* Set sleep mode and activate DAC + Timer. */
	setNormalSleepMode();

	/* Enable interrupt handling */
	setupInterrupt();

	/* Play opening song. */
	playIntroSong();

	/* Run main program. */
	__asm("wfi");		//Wait for interrupt. 

	return 0;
}

/* Set NVIC ISERx register for interrupt enable. */
void setupNVIC()
{
	*ISER0 |= NVIC_ISER0_GPIO_EVEN | NVIC_ISER0_GPIO_ODD
#if SOUND_FROM_TIMER1
	    | NVIC_ISER0_TIMER1
#else
	    | NVIC_ISER0_LETIMER0
#endif
	    ;			//Enables GPIO odd and even interrupts, and TIMER1 and LETIMER0 interrupts. 
}

/* Activate interrupt mode. */
void setupInterrupt()
{
	setupGPIOInterrupts();
#if SOUND_FROM_TIMER1
	setupTimerInterrupt();
#else
	setupLETimer(LETIMER0_PERIOD);
#endif
	setupNVIC();
}

/* Activate sleep mode (EM1). This does not disable TIMER1. */
void setNormalSleepMode()
{
	*SCR = 0x2;		//Enables sleep mode for CPU.
	*DAC0_CH0CTRL |= 0x1;	//Enable right audio channels. 
	*DAC0_CH1CTRL |= 0x1;	//Enable left audio channels.
#if SOUND_FROM_TIMER1
	*TIMER1_CMD = TIMER1_CMD_START;
#else
	*CMU_OSCENCMD |= CMU_OSCENCMD_LFRCO_ACTIVATE;
	*LETIMER0_CMD = LETIMER0_CMD_START;
#endif
}

/* Activate deep sleep mode (EM2). This disables TIMER1. */
void setDeepSleepMode()
{
#if SOUND_FROM_TIMER1
	*TIMER1_CMD = TIMER1_CMD_STOP;
#else
	*LETIMER0_CMD = LETIMER0_CMD_STOP;
	*CMU_OSCENCMD |= CMU_OSCENCMD_LFRCO_DEACTIVATE;
#endif
	*SCR = 0x6;		//Enables deep sleep mode for CPU.
	*DAC0_CH0CTRL &= ~(0x1);	//Disable right audio channel. 
	*DAC0_CH1CTRL &= ~(0x1);	//Disable left audio channel.
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
