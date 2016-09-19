.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

.section .vectors

	.long   stack_top               /* Top of Stack                 */
	.long   _reset                  /* Reset Handler                */
	.long   dummy_handler           /* NMI Handler                  */
	.long   dummy_handler           /* Hard Fault Handler           */
	.long   dummy_handler           /* MPU Fault Handler            */
	.long   dummy_handler           /* Bus Fault Handler            */
	.long   dummy_handler           /* Usage Fault Handler          */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* SVCall Handler               */
	.long   dummy_handler           /* Debug Monitor Handler        */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* PendSV Handler               */
	.long   dummy_handler           /* SysTick Handler              */

	/* External Interrupts */
	.long   dummy_handler
	.long   gpio_handler            /* GPIO even handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   gpio_handler            /* GPIO odd handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler

.section .text

/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset:
	// -- Enable GPIO-clk
	ldr r1, cmu_base_addr

	mov r3, #1
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0] 	//Enables CMU_HFPERCLKEN0 for GPIO.

	// -- Enable GPIO LEDs
	ldr r0, gpio_base_addr

	mov r1, #0x2
	mov r2, #0x55555555
	mov r3, #0xFF00
	
	str r1, [r0, #GPIO_PA_CTRL]	//Sets high drive strength (20 mA).
	str r2, [r0, #GPIO_PA_MODEH]	//Sets push-pull output with the same drive strength. 
	str r3, [r0, #GPIO_PA_DOUT]	//Initializes LEDs off. 


	// -- Enable GPIO buttons
	mov r1, #0x33333333
	mov r2, #0xFF
	mov r3, #0x22222222	
	
	str r1, [r0, #GPIO_PC_MODEL]	//Enables input with filter. 
	str r2, [r0, #GPIO_PC_DOUT]	//Enables pull-up resistors.
	
	
	// -- Enable GPIO interrupts
	mov r1, #0x800
	str r3, [r0, #GPIO_EXTIPSELL]	//Selects port C for interrupts. 
	str r2, [r0, #GPIO_EXTIFALL]	//Enables falling edge detection.

	orr r1, r1, #0x2
	ldr r3, iser_base_addr

	str r2, [r0, #GPIO_IFC]		//Clears external interrupt flags.
	str r2, [r0, #GPIO_IEN]		//Enables external interrupts.

	str r1, [r3]			//Enables _odd and _even interrupt handlers for GPIO pins. 

	
	// -- Setup sleep mode.
	mov r7, #6
	ldr r6, =SCR
	str r7, [r6]

	
	// -- Wait for interrupt.
	wfi


cmu_base_addr:
	.long CMU_BASE

gpio_base_addr:
	.long GPIO_BASE

iser_base_addr:
	.long ISER0

	
/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:
	ldr r3, [r0, #GPIO_IF]		//Reads external interrupt flags.
	str r3, [r0, #GPIO_IFC]		//Clears external interrupt flags.

	// -- Read buttons and jump to "exit" if none is pushed.
read_buttons:
	ldr r3, [r0, #GPIO_PC_DIN]	//Read buttons.
	eors r2, r3, #0xFF		//Invert the register. 
	beq exit

	mov r3, #0x80		//Seeks the MSB by right-shifting every loop until the comparison hits.
	mov r4, #0xFF		//Keeps track of how many lights should be on.
	
	// -- Find the most significant bit; the highest pushed button value.
read_MSB:
	ands r5, r2, r3
	bne set_lights
	lsr r4, #1
	lsr r3, #1
	cmp r4, #0
	bne read_MSB

	// -- Turn on x LEDs, given by MSB.
set_lights:
	mvn r4, r4
	lsl r4, #8
	str r4, [r0, #GPIO_PA_DOUT]	//Store the LED word.
	b read_buttons
	
	// -- Turn off LEDs and go to LR.
exit:
	mov r1, #0xFF00
	str r1, [r0, #GPIO_PA_DOUT]	//Store the LED word.
	bx LR				//Return to pre-interrupt state.

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:	//The program should never enter here.
	mov r1, #0xAA00
	str r1, [r0, #GPIO_PA_DOUT]	//Indicate something went wrong.
	b .  // do nothing


	
