#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupGPIOinterrupts();

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
	/* TODO set input and output pins for the joystick */
	//Joystick = GPIO board?
	/* Example of HW access from C code: turn on joystick LEDs D4-D8
	   check efm32gg.h for other useful register definitions
	 */

	/* Enable GPIO clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	

	/* Enable LEDs. */
	*GPIO_PA_CTRL = 2;				//Sets high drive strength (20mA).
	*GPIO_PA_MODEH = 0x55555555;	//Sets pins A8-15 as output.
	*GPIO_PA_DOUT = 0x0700;			//Turn on LEDs D4-D8 (LEDs are active low).

	/* Enable buttons. */
	*GPIO_PC_MODEL = 0x33333333;	//Enables input with filter. 
	*GPIO_PC_DOUT = 0xFF;			//Enables pull-up resistors. 

	setupGPIOinterrupts();

}

void setupGPIOinterrupts()
{
	/* Enable interrupts. */
	*GPIO_EXTIPSELL = 0x22222222;	//Selects port C for interrupts. 
	*GPIO_EXTIFALL = 0xFF;			//Enables falling edge detection.
	//*GPIO_IFC = 0xFF;				//Clears external interrupt flags. 
	*GPIO_IEN = 0xFF;				//Enables external interrupts.
}
