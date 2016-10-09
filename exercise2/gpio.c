#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

int position = 0;

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
	*GPIO_PA_CTRL = 0x2;	//Sets high drive strength (20mA).
	*GPIO_PA_MODEH = 0x55555555;	//Sets pins A8-15 as output.
	*GPIO_PA_DOUT = 0x0700;	//Turn on LEDs D4-D8 (LEDs are active low).

	/* Enable buttons. */
	*GPIO_PC_MODEL = 0x33333333;	//Enables input with filter. 
	*GPIO_PC_DOUT = 0xFF;	//Enables pull-up resistors. 

	/* Set startpositon of lights */
	*GPIO_PA_DOUT = 0xFE00;


}

void setupGPIOinterrupts(){
	/* Enable interrupts. */
	*GPIO_EXTIPSELL = 0x22222222;	//Selects port C for interrupts. 
	*GPIO_EXTIFALL = 0xFF;			//Enables falling edge detection.
	*GPIO_IFC = 0xFF;				//Clears external interrupt flags. 
	*GPIO_IEN = 0xFF;				//Enables external interrupts.
}


void moveLight(int direction){
	position = (position+direction) % 8;
	*GPIO_PA_DOUT = (0xFEFF << position);
}


int readGPIOInput(){
	switch(*GPIO_PC_DIN) {
		case 0xFE :
			return 1;
		
		case 0xFD :
			return 2;
		
		case 0xFB :
			return 3;
		
		case 0xF7 :
			return 4;
		
		case 0xEF :
			return 5;
		
		case 0xDF :
			return 6;
		
		case 0xBF :
			return 7;
		
		case 0x7F :
			return 8;
		
		default :		//Returns 0 if no button is pushed, or multiple buttons are pushed
			return 0;
	}

}

