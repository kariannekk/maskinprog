#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

unsigned int LED_offset = 0;
int LED_value;

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
}

void setupGPIOInterrupts()
{
	*GPIO_EXTIPSELL = 0x22222222;	//Selects port C for interrupts. 
	*GPIO_EXTIFALL = 0xFF;	//Enables falling edge detection.
	*GPIO_IFC = 0xFF;	//Clears external interrupt flags. 
	*GPIO_IEN = 0xFF;	//Enables external interrupts.
}

/* Move a LED, 'direction' amount of steps, right (+) or left (-). */
void moveSingleLED(int direction)
{
	LED_offset = (LED_offset + direction) % 8;
	*GPIO_PA_DOUT = (0xFEFF << LED_offset);
}

/* Return a single pushed button. Multiple are regarded as none. When successfull, output value is the button number. */
int readGPIOInput()
{
	/* Pins are 0-indexed. Buttons are 1-indexed. */
	switch (*GPIO_PC_DIN) {
	case 0xFE:
		return 1;
	case 0xFD:
		return 2;
	case 0xFB:
		return 3;
	case 0xF7:
		return 4;
	case 0xEF:
		return 5;
	case 0xDF:
		return 6;
	case 0xBF:
		return 7;
	case 0x7F:
		return 8;
	default:
		return 0;
	}
}

/* Use these two to get a weaker LED output light. This saves power and is more relaxing to the eye. */
//TODO remove these, or use them within LETIMER or similar. 
void toggleLEDsON()
{
	*GPIO_PA_DOUT = LED_value;
}
void toggleLEDsOFF()
{
	LED_value = *GPIO_PA_DOUT;
	*GPIO_PA_DOUT = 0xFF00;
}

