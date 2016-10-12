# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

TARGET=ex2.c

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : $(TARGET:.c=.o) timer.o dac.o gpio.o interrupt_handlers.o sound_manager.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex 
	
.PHONY : interrupt
interrupt :
	make clean
	make TARGET=ex2_interrupt.c
	
.PHONY : polling
polling : 
	make clean
	make TARGET=ex2_polling.c

.PHONY : debug
debug : 
	gnome-terminal -x JLinkGDBServer
	arm-none-eabi-gdb ex2.elf
	
.PHONY : help
help :
	@echo "---------------------------------------------------------------"
	@echo "Available commands: "
	@echo "<none>, help, upload, clean, pretty, debug, interrupt, polling.\n"
	@echo "---------------------------------------------------------------"
	@echo "help - displays this."
	@echo "upload - uploads .elf file to the EFM32GG circuit board."
	@echo "clean - removes .o, .elf, .bin and .hex files from directory."
	@echo "pretty - adjusts the files' indentations and format."
	@echo "debug - runs the debug program."
