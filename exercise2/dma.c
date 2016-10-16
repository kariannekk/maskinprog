#include "efm32gg.h"

#define CMU_HFCORECLKEN0_DMA 	(1<<0)
#define PRS_CH0_CTRL_SOURCESEL_TIMER1	(0b011101<<16)
#define PRS_CH0_CTRL_SIGSEL_OVERFLOW	(0b001<<0)
#define PRS_CH0_CTRL_EDSEL_POSEDGE		(0b1<<24)
#define DAC0_CHnCTRL_PRSEN				(1<<2)
#define DAC0_CHnCTRL_PRSSEL_CLEAR		(0b1111<<4)
#define DMA_CONFIG_ENABLE				(1<<0)

void setupDMA()
{
	/* Enable PRS clock. TODO make sure timer clock as well. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_PRS;

	/* Enable DMA clock. */
	*CMU_HFCORECLKEN0 |= CMU_HFCORECLKEN0_DMA;

	/* Set PRS input to trigger from TIMER1 overflow. */
	*PRS_CH0_CTRL |= PRS_CH0_CTRL_SOURCESEL_TIMER1
	    | PRS_CH0_CTRL_SIGSEL_OVERFLOW | PRS_CH0_CTRL_EDSEL_POSEDGE;

	/* TODO no need for TIMER1 interrupts. */

	/* Set DAC to trigger from PRS CH0. */
	*DAC0_CH0CTRL &= ~(DAC0_CHnCTRL_PRSSEL_CLEAR);
	*DAC0_CH1CTRL &= ~(DAC0_CHnCTRL_PRSSEL_CLEAR);

	/* Set DAC to trigger from output PRS. */
	*DAC0_CH0CTRL |= DAC0_CHnCTRL_PRSEN;
	*DAC0_CH1CTRL |= DAC0_CHnCTRL_PRSEN;

	/* Set DMA to Ping Pong mode. */
	*DMA_CONFIG |= DMA_CONFIG_ENABLE;
	//TODO Write to DAC0_COMBDATA rather than to each separately?
	//TODO End pointer for DAC should be (+12) because 12 bits.
	//TODO How large is the CTRLBASE size? Can it store the entire song? Each separate note? Just a single sample?
	//*DMA_CTRLBASE |= 
	//*DMA_ALTCTRLBASE |= //Is read from DMA_CTRLBASE + 0x100.

	/* Set DMA to send new sample to DAC when (DAC is ready "OR RATHER" TIMER1 is finished?). */
	//DMA_CH0_CTRL |= SOURCESEL & SIGSEL

	/* Enable DMA interrupts. */
	//*DMA_IEN |= 

	/* TODO DMA_IRQ_handler. */
	//*DMA_IFC |= 

}
