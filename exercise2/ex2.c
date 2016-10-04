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
void runTimerOnce(uint16_t DAC_Freq, int sample_period);
void playArray(uint32_t inputList, uint16_t length);
void my_polling_program();

void poll_the_timer();


/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);	//Consider moving to setupNVIC().

	/* Enable interrupt handling */
	//setupNVIC();

	/* Enable energy efficiency. */
	//setupSleepMode();



	


	/* Run polling program. */
	my_polling_program();	//For easier toggling of interrupt vs polling. We can also easily add my_polling_program_A() and my_polling_program_B().

	return 0;
}

uint32_t a4[] = { 127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 234, 239, 242, 245, 248, 250, 252, 253, 254, 254, 254, 254, 252, 251, 248, 246, 243, 239, 235, 231, 226, 220, 215, 209, 203, 196, 189, 182, 175, 167, 160, 152, 144, 136, 128, 120, 112, 104, 96, 89, 81, 74, 67, 60, 53, 47, 41, 35, 30, 25, 20, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 0, 2, 3, 5, 8, 11, 15, 19, 23, 28, 33, 39, 44, 51, 57, 64, 71, 78, 86, 94, 101, 109, 117 };
//uint32_t a5[] = { 127, 143, 159, 174, 188, 202, 214, 225, 234, 242, 248, 252, 254, 254, 252, 248, 243, 235, 226, 215, 203, 189, 175, 160, 144, 128, 112, 96, 81, 67, 53, 41, 30, 20, 12, 6, 2, 0, 0, 2, 5, 11, 19, 28, 39, 51, 64, 78, 94, 109 };
//uint16_t a_length = 50; //100;

void my_polling_program()
{
	while (1) {
		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
		if (*TIMER1_CNT >= SAMPLE_PERIOD - 100) {
			*GPIO_PA_DOUT = 0x0F00;
		} else if (!(*GPIO_PC_DIN & (1<<0))) {
			runTimerOnce(150, SAMPLE_PERIOD);
		} else if (!(*GPIO_PC_DIN & (1<<1))) {
			runTimerOnce(450, SAMPLE_PERIOD);
		} else if (!(*GPIO_PC_DIN & (1<<2))) {
			*GPIO_PA_DOUT = 0xAA00;
			playArray(*a4, 100);
		} else if (!(*GPIO_PC_DIN & (1<<3))) {
			runTimerOnce(600, SAMPLE_PERIOD);
			runTimerOnce(800, SAMPLE_PERIOD);
		} else if (!(*GPIO_PC_DIN & (1<<4))) {
			poll_the_timer();
		} else if (!(*GPIO_PC_DIN & (1<<5))) {
			
		} else if (!(*GPIO_PC_DIN & (1<<6))) {
			
		}
	}
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

	//wfi;          //What is the non-assembler version of this? Do we have alternatives?
	//TODO Disable timer & adc.
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
