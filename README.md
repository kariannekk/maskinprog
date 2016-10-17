# maskinprog


#If you wish you may look at the command:
make help

#For baseline solution:
make polling
make upload

#For improved solution:
make interrupt
make upload


#Additional info:
* Within notes.h there is a #define VOLUME. Set it lower if the sound is too high. The current value was ideal for our setup. 
* Within ex2_interrupt.c there is a #define SOUND_FROM_TIMER1. Set it to "false" to use LETIMER0, and to "true" to use TIMER1. The sample rate is tuned for TIMER1 usage.
* We began implementing the DMA as well, but this task became too large for our limited time. The file is included for as far as we achieved. 

