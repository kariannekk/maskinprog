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
//	CMU_BASE = 0x400c8000 			//base addr
//	CMU_HFPERCLKEN0 = 0x044 		//offset addr
	ldr r1, cmu_base_addr

	ldr r2, [r1, #CMU_HFPERCLKEN0]
	mov r3, #0x2000
	orr r2, r2, r3					//Does not dispose existing values.
	str r2, [r1, #CMU_HFPERCLKEN0]	//Enables CMU_HFPERCLKEN0 for GPIO.
	// ---
	

	// -- Enable GPIO LEDs
//	GPIO_BASE = 0x40006000		//base addr
//	GPIO_PA_CTRL = 0x2 			//offset addr
//	GPIO_PA_MODEH = 0x008 		//offset addr
//	GPIO_PA_DOUT = 0x00C 		//offset addr
	ldr r0, gpio_base_addr

	mov r1, #0x2
	str r1, [r0, #GPIO_PA_CTRL]		//Sets high drive strength (20 mA).

	mov r2, #0x55555555
	str r2, [r0, #GPIO_PA_MODEH]	//Sets push-pull output with the same drive strength. 

	mov r3, #0xFF00
	str r3, [r0, #GPIO_PA_DOUT]
	// ---


	// -- Enable GPIO buttons	//Notice: kept GPIO_BASE at register.
//	GPIO_PC_CTRL = 0x048	//offset addr
//	GPIO_PC_MODEL = 0x04C	//offset addr
//	GPIO_PC_DOUT = 0x054	//offset addr
//	GPIO_PC_DIN = 0x064		//offset addr

	mov r2, #0x33333333
	str r2, [r0, #GPIO_PC_MODEL]	//Enables input with filter. 

	mov r1, #0xFF
	str r1, [r0, #GPIO_PC_DOUT]	//Enables 
	// ---

	
	// -- Enable GPIO interrupts//Notice: kept GPIO_BASE at register.
//	GPIO_EXTIPSELL = 0x100	//offset addr
//	GPIO_EXTIRISE = 0x108	//offset addr
//	GPIO_EXTIFALL = 0x10C	//offset addr
//	GPIO_IEN = 0x110		//offset addr
//	GPIO_IF = 0x114			//offset addr
//	GPIO_IFC = 0x11C		//offset addr
//	ISER0 = 0xE000E100		//base addr

	mov r2, #0x22222222	
	str r2, [r0, #GPIO_EXTIPSELL]	//Selects port C for interrupts. 

	//mov r1, #0xFF					//Has already loaded #0xFF.
	//str r1, [r0, #GPIO_EXTIRISE]	//Enables rising edge detection.
	str r1, [r0, #GPIO_EXTIFALL]	//Enables falling edge detection.

	str r1, [r0, #GPIO_IFC]			//Clears external interrupt flags.
	str r1, [r0, #GPIO_IEN]			//Enables external interrupts.

	ldr r2, iser_base_addr
	mov r3, #0x800
	orr r3, r3, #0x2
	str r3, [r2]
	//---


//	b i_waiter

//i_waiter:			//Waits for interrupt.
//	mov r2, #0xF000
//	str r2, [r0, #GPIO_PA_DOUT]	//Sets LEDs.

	// -- Set-up sleep mode
	ldr r6, =SCR
	mov r7, #6
	str r7, [r6]
	
	// -- wait for interrupt
	wfi
	
//	b i_waiter
	
//	b . // do nothing




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

	// -- Reads button(s), and jumps to "exit" if none is pushed
buttons:
	ldr r3, [r0, #GPIO_PC_DIN]	//Reads buttons.
	eor r2, r3, #0xFF
	cmp r2, #0
	beq exit

	mov r4, #0xFF
	mov r3, #0x80

	// -- Check for the highest value of pushed button
check:
	and r5, r2, r3
	cmp r5, #0
	bne lights
	lsr r3, #1
	lsr r4, #1
	cmp r4, #0
	bne check

	// -- Turn on x LEDs, given by highest button value pushed
lights:
	eor r4, r4, #0xFF
	lsl r4, #8
	str r4, [r0, #GPIO_PA_DOUT]
	b buttons
	
	// -- Turn off all LEDs, and go LR
exit:
	mov r1, #0xFF00
	str r1, [r0, #GPIO_PA_DOUT]	//store the LED word
	bx LR						//Should return to pre-interrupt state.

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:		//LED signature if this is entered.
	b .  // do nothing


	
