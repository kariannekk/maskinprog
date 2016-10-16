# Exercise 2, TDT4258, fall 2016.

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

ex2.elf : $(TARGET:.c=.o) timer.o dac.o gpio.o interrupt_handlers.o sound_manager.o letimer.o
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
	@echo "<none> \t\t- make from ex2.c."
	@echo "help \t\t- displays this."
	@echo "upload \t\t- uploads .elf file to the EFM32GG circuit board."
	@echo "clean \t\t- removes .o, .elf, .bin and .hex files from directory."
	@echo "pretty \t\t- adjusts the files' indentations and format."
	@echo "debug \t\t- runs the debug program."
	@echo "interrupt \t- make from ex2_interrupt."
	@echo "polling \t- make from ex2_polling."
	@echo ""
