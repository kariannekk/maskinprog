# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

EX1 =ex1.s


ex1.bin : ex1.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1.o : ${EX1}
	${AS} ${ASFLAGS} $< -o $@


.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex

.PHONY : polling
polling :
	make clean
	make EX1=ex1_polling.s

.PHONY : interrupt
interrupt :
	make clean
	make EX1=ex1_interrupt.s

.PHONY : debug
debug :
	gnome-terminal -x JLinkGDBServer
	arm-none-eabi-gdb ex1.elf -tui

.PHONY : help
help :
	@echo "--------------------------------------------------------------"
	@echo "Commands: "
	@echo "<none>, help, upload, clean, debug, polling, interrupt.\n"


