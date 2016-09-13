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
	//Enable GPIO-clk
	CMU_BASE = 0x400c8000 		//base addr
	CMU_HFPERCLKEN0 = 0x044 	//offset addr
	CMU_HFPERCLKEN0_GPIO = 13 	//bit representing GPIO
	ldr r1, cmu_base_addr

	ldr r2, [r1, #CMU_HFPERCLKEN0_GPIO]
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0]
	//---
	

	//Enable GPIO LEDs
	GPIO_BASE = 0x40006000		//base addr
	GPIO_PA_CTRL = 0x2 		//offset addr
	GPIO_PA_MODEH = 0x008 		//offset addr
	GPIO_PA_DOUT = 0x00C 		//offset addr
	ldr r0, gpio_base_addr

	mov r1, #0x2
	str r1, [r0, #GPIO_PA_CTRL]

	mov r2, #0x55555555
	str r2, [r0, #GPIO_PA_MODEH]

	mov r3, #0xFF00
	str r3, [r0, #GPIO_PA_DOUT]
	//---


	//Enable GPIO buttons	//Notice: kept GPIO_BASE at register
	GPIO_PC_CTRL = 0x048	//offset addr
	GPIO_PC_MODEL = 0x04C	//offset addr
	GPIO_PC_DOUT = 0x054	//offset addr
	GPIO_PC_DIN = 0x064	//offset addr

	mov r1, #0x33333333
	str r1, [r0, #GPIO_PC_MODEL]

	mov r2, #0xFF
	str r2, [r0, #GPIO_PC_DOUT]
	//---

	b loop

loop:
	ldr r1, [r0, #GPIO_PC_DIN]	//load the button word
	lsl r1, #8
	str r1, [r0, #GPIO_PA_DOUT]	//store the LED word

	b loop




cmu_base_addr:
	.long CMU_BASE

gpio_base_addr:
	.long GPIO_BASE

	
/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:  
	b .  // do nothing

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:  
	b .  // do nothing



	
