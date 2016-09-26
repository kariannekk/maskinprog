#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
	/* TODO set input and output pins for the joystick */

	/* Example of HW access from C code: turn on joystick LEDs D4-D8
	   check efm32gg.h for other useful register definitions
	 */
	 
	/* Clock */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	
	
	/* LEDs */
	*GPIO_PA_CTRL = 2;					/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;		/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0x0700;				/* turn on LEDs D4-D8 (LEDs are active low) */
	
	
	/* Buttons */
	*GPIO_PC_MODEL = 0x33333333;		//Enables input with filter. 
	*GPIO_PC_DOUT = 0xFF;				//Enables pull-up resistors. 
	
	
	/* Interrupts */
	/*
	*GPIO_EXTIPSELL = 0x22222222;		//Selects port C for interrupts. 
	*GPIO_EXTIFALL = 0xFF;				//Enables falling edge detection.
	*GPIO_IFC = 0xFF;					//Clears external interrupt flags. 
	*GPIO_IEN = 0xFF;					//Enables external interrupts.
	*/
	
	/* Sleep mode */
	/*
	*SCR |= 0x6;						//Enables sleep mode for CPU.
	*/
}
