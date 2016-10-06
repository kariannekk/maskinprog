#include <stdio.h>
#include <stdbool.h>

#include "efm32gg.h"



/* temp define. */
#define SAMPLE_FREQUENCY 44100 //samples per second.



/* Global variables for tracking. */
int ** current_song;
int current_note;
int current_sample;
int current_note_duration;
//current period of note
//volume



/* temp inputs. */
int a4[] = {101, 127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 234, 239, 242, 245, 248, 250, 252, 253, 254, 254, 254, 254, 252, 251, 248, 246, 243, 239, 235, 231, 226, 220, 215, 209, 203, 196, 189, 182, 175, 167, 160, 152, 144, 136, 128, 120, 112, 104, 96, 89, 81, 74, 67, 60, 53, 47, 41, 35, 30, 25, 20, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 0, 2, 3, 5, 8, 11, 15, 19, 23, 28, 33, 39, 44, 51, 57, 64, 71, 78, 86, 94, 101, 109, 117 };
int a5[] = {51, 127, 143, 159, 174, 188, 202, 214, 225, 234, 242, 248, 252, 254, 254, 252, 248, 243, 235, 226, 215, 203, 189, 175, 160, 144, 128, 112, 96, 81, 67, 53, 41, 30, 20, 12, 6, 2, 0, 0, 2, 5, 11, 19, 28, 39, 51, 64, 78, 94, 109 };
int a4_length = 100;
int a5_length = 50;
int empty[] = {101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int * testsong[] = {(int*) 13, a4, a5, empty, a5, a4, a5, a4, a5, empty, a4, a5, a4};



/* Functions. */
//setup and begin playing song
//send constant sample length to DAC. based on SAMPLE_PERIOD?
	//Will jumps be noticed? if we end note A in its upper cycle and immediately switch to note B in its lower?
//set volume

/* Creates a simple, naive triangle-wave. */
void runDAC(uint16_t sampleAmount)
{
	uint16_t SAMPLE_MIN = 0x400;	//Appears to be 'dont-care' value. This is the min-value of the wave. 
	uint16_t amplitudeStep = 0x1;	//Volume-ish. Actually the step added sampleAmount times to the min-value. 
	//uint16_t sampleAmount = 800;          //Frequency, somewhat. Approximate min: 30. Approximate max: 1500. will make ugly noise further than 1500. 

	for (int i = 0; i < sampleAmount; i++) {
		*DAC0_CH0DATA = SAMPLE_MIN + (i * amplitudeStep);
		//*DAC0_CH1DATA = SAMPLE_MEDIAN + 0x200 + (i * 2 * amplitudeStep);
	}
	for (int i = sampleAmount; i > 0; i--) {
		*DAC0_CH0DATA = SAMPLE_MIN + (i * amplitudeStep);
		//*DAC0_CH1DATA = SAMPLE_MEDIAN + 0x200 + (i * 2 * amplitudeStep);
	}
}

void runTimerOnce(uint16_t DAC_Freq, int sample_period)
{
	*TIMER1_CTRL = 0x9000000;	//Prescale clock for slower counting. 
	*TIMER1_CNT = 0x0000;	//Ensures we get a full Timer1 delay. 

	while (*TIMER1_CNT < (sample_period * 4 / 5)) {
		runDAC(DAC_Freq);
	}

	*TIMER1_CTRL = 0x0000000;	//Restore unscaled counting. 
}



/* INIT */



/* Functions to use after testing. May remove those above. */
void nextNote()
{
	current_note++;
	if (current_note >= (int) current_song[0])
	{
		current_song = 0;
	}
	int tempFix = 1.4;
	current_note_duration = tempFix * SAMPLE_FREQUENCY / (int)current_song[current_note][0];
}

void setSong(int ** input_song)
{
	current_note = 0;
	current_song = testsong;//input_song;
	current_sample = 0;
	nextNote();
//	*GPIO_PA_DOUT = 0xFE00;
	//enable something timer?
}

void playSampleList(int * note)
{
	//for (int i=0; i<length; i++)	//Yields faster freq.
	for (int i=1; i<note[0]; i++)
	{
		*DAC0_CH0DATA = note[i];
	}
}

void playNote()
{
	playSampleList(current_song[current_note]);
	current_note_duration--;
	if (current_note_duration <= 0)
	{
		nextNote();
	}
}


/* more temp functions. */
void playSong()
{
	if (current_song <= 0)
	{
		//*GPIO_PA_DOUT = 0x3C00;
		return;
	}
	playNote();
}


void playEntireSong()
{
	while (!(current_song == 0)){
		playNote();
	}
	//*GPIO_PA_DOUT = 0x3C00;	
}

void playSample()
{
	/* Varies a lot from TIMER1 period and other program functions. */
	*DAC0_CH0DATA = a4[current_sample++ % a4_length];
	//current_sample++;
	/*if (current_sample >= a4_length)
	{
		current_sample = 0;
	}*/
	
	
	
	/* Sounds good, but reduces program flexibility. */
	/*for (int i=1; i<a4[0]; i++)
	{
		*DAC0_CH0DATA = a4[i];
	}*/
}

void runThis()
{
	//sendSample();
	
	//playSampleList(a4);
	
	playEntireSong();
	
	//playSong();
}

