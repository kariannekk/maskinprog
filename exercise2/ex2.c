#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"



/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   0xFFF0



/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void runDAC(uint16_t sampleAmount);
void setupNVIC();
void setupSleepMode();
void __attribute__ ((interrupt)) TIMER1_IRQHandler();
void runTimerOnce(uint16_t DAC_Freq);


/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);		//Consider moving to setupNVIC().

	/* Enable interrupt handling */
	//setupNVIC();

	/* Enable energy efficiency. */
	//setupSleepMode();
	
	while (1){
		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
		if (!(*GPIO_PC_DIN & 1))
		{
//			runDAC(850);
/*			uint32_t temp = *TIMER1_CNT;
			if (temp < SAMPLE_PERIOD/3){
				runDAC(150);
			}
			if (temp < SAMPLE_PERIOD){
				runDAC(850);
			}
			*/
			
			runTimerOnce(150);
		}
		else if (!(*GPIO_PC_DIN & 2))
		{
			runTimerOnce(450);
		}
		
		
		
	}

	return 0;
}



void setupNVIC()
{
	/* Set NVIC ISERx register for interrupt enable. */
	*ISER0 |= 0x1802;	//Enables GPIO odd and even interrupts, and TIMER1 interrupts. 
}

void setupSleepMode()
{
	/* Sleep mode */
	*SCR |= 0x6;		//Enables sleep mode for CPU.
	
	//wfi;		//What is the non-assembler version of this? Do we have alternatives?
}

void runTimerOnce(uint16_t DAC_Freq)
{
	*TIMER1_CTRL = 0x9000000;		//Prescale signal. 
	*TIMER1_TOP = SAMPLE_PERIOD;

	while (*TIMER1_CNT < (SAMPLE_PERIOD * 4 / 5))
	{
		runDAC(DAC_Freq);
	
	}

	*TIMER1_TOP = 0x0008;
	*TIMER1_CTRL = 0x0000000;
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
