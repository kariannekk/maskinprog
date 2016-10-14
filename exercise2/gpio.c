#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define RIGHT 1
#define LEFT -1

void selectSongFromButton();

unsigned int LED_offset = 0;
int LED_value;

/* Enable LEDs and buttons. */
void setupGPIO()
{
	/* Enable GPIO clock. */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	/* Enable LEDs. */
	*GPIO_PA_CTRL = 0x2;	//Sets high drive strength (20mA).
	*GPIO_PA_MODEH = 0x55555555;	//Sets pins A8-15 as output.
	*GPIO_PA_DOUT = 0x0700;	//Turn on LEDs D4-D8 (LEDs are active low).

	/* Enable buttons. */
	*GPIO_PC_MODEL = 0x33333333;	//Enables input with filter. 
	*GPIO_PC_DOUT = 0xFF;	//Enables pull-up resistors. 

	/* Set start position of lights. */
	*GPIO_PA_DOUT = 0xFE00;
	LED_value = 0xFE00;
}

void setupGPIOInterrupts()
{
	*GPIO_EXTIPSELL = 0x22222222;	//Selects port C for interrupts. 
	*GPIO_EXTIFALL = 0xFF;	//Enables falling edge detection.
	*GPIO_IFC = 0xFF;	//Clears external interrupt flags. 
	*GPIO_IEN = 0xFF;	//Enables external interrupts.
}

/* Move a lit LED, 'direction' amount of steps, right (+) or left (-). */
void moveSingleLED(int direction)
{
	LED_offset = (LED_offset + direction) % 8;
	//*GPIO_PA_DOUT = (0xFEFF << LED_offset);
	LED_value = (0xFEFF << LED_offset);
}

/* Act on a single pushed odd GPIO button. Multiple are regarded as none. Buttons are 1-indexed. */
void GPIOOddInput()
{
	switch (*GPIO_PC_DIN) {
	case 0xFD:		//Button SW2. 
		selectSongFromButton(2);
		return;
	case 0xF7:		//Button SW4. 
		selectSongFromButton(4);
		return;
	case 0xDF:		//Button SW6. 
		selectSongFromButton(6);
		return;
	case 0x7F:		//Button SW8. 
		selectSongFromButton(8);
		return;
	default:
		return;
	}
}

/* Act on a single pushed even GPIO button. Multiple are regarded as none. Buttons are 1-indexed. */
void GPIOEvenInput()
{
	switch (*GPIO_PC_DIN) {
	case 0xFE:		//Button SW1. 
		moveSingleLED(LEFT);
		return;
	case 0xFB:		//Button SW3. 
		moveSingleLED(RIGHT);
		return;
	case 0xEF:		//Button SW5. 
		selectSongFromButton(5);
		return;
	case 0xBF:		//Button SW7. 
		selectSongFromButton(7);
		return;
	default:
		return;
	}
}

/* Use these two to get a weaker LED output light. This saves power and is more relaxing to the eye. */
void toggleLEDsON()
{
	*GPIO_PA_DOUT = LED_value;
}

void toggleLEDsOFF()
{
	LED_value = *GPIO_PA_DOUT;
	*GPIO_PA_DOUT = 0xFF00;
}
