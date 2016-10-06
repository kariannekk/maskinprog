#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound waves you want to generate. The core clock (which the timer clock is derived from) runs at 14 MHz by default. Also remember that the timer counter registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   SAMPLE_PERIOD   0xFFF

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupGPIOinterrupts();
void setGPIOLight(int buttonNumber);
int readGPIOInput(int GPIOButton);

void setupTimer(uint32_t period);
void setupTimerInterrupts();
void setupDAC();
void setupNVIC();
void setupInterrupt();
void setupSleepMode();
void __attribute__ ((interrupt)) TIMER1_IRQHandler();

void my_polling_programB();

void runDAC(uint16_t sampleAmount);
void runTimerOnce(uint16_t DAC_Freq, int sample_period);

void runThis();
void setSong(int ** input_song);



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

	my_polling_program();

	return 0;
}

	
void my_polling_program()
{
	int buttonNumber = 0;
	
	/* Play opening song*/
	setSong((int**)1);
	runThis();
	
	//*GPIO_PA_DOUT = 0xFEFF;
	//*TIMER1_CTRL = 0xa000000;	//Prescale clock for slower counting. 

	/* Play buttonsound */
	while (1)
	{
		if (*TIMER1_CNT <= 0)
		{
			runThis();
		}

		if(buttonNumber = readGPIOInput(*GPIO_PC_DIN))
		{
			setSong((int**)1);
			continue;
		}
	}
}

void setupInterrupt(){
	setupGPIOinterrupts();
	setupTimerInterrupts();
	setupNVIC();
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
	//TODO Disable timer & dac.
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