#include <stdio.h>
#include <stdbool.h>

#include "efm32gg.h"



/* temp define. */
#define SAMPLE_FREQUENCY 44100 //samples per second.

void startTimer();
void stopTimer();

/* Global variables for tracking. */
int ** current_song;
int current_note;
int current_sample;
int current_note_duration;
//current period of note
//volume



/* temp inputs. */
int a4[] = {100, 127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 234, 239, 242, 245, 248, 250, 252, 253, 254, 254, 254, 254, 252, 251, 248, 246, 243, 239, 235, 231, 226, 220, 215, 209, 203, 196, 189, 182, 175, 167, 160, 152, 144, 136, 128, 120, 112, 104, 96, 89, 81, 74, 67, 60, 53, 47, 41, 35, 30, 25, 20, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 0, 2, 3, 5, 8, 11, 15, 19, 23, 28, 33, 39, 44, 51, 57, 64, 71, 78, 86, 94, 101, 109, 117 };
int a5[] = {50, 127, 143, 159, 174, 188, 202, 214, 225, 234, 242, 248, 252, 254, 254, 252, 248, 243, 235, 226, 215, 203, 189, 175, 160, 144, 128, 112, 96, 81, 67, 53, 41, 30, 20, 12, 6, 2, 0, 0, 2, 5, 11, 19, 28, 39, 51, 64, 78, 94, 109 };
int a4_length = 100;
int a5_length = 50;
int empty[] = {100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int d4[] = { 150, 127, 132, 138, 143, 148, 153, 159, 164, 169, 174, 179, 184, 188, 193, 197, 202, 206, 210, 214, 218, 222, 225, 228, 232, 235, 237, 240, 242, 244, 246, 248, 250, 251, 252, 253, 254, 254, 254, 254, 254, 254, 253, 252, 251, 250, 248, 247, 245, 242, 240, 238, 235, 232, 229, 225, 222, 218, 215, 211, 206, 202, 198, 193, 189, 184, 179, 174, 169, 164, 159, 154, 149, 143, 138, 133, 127, 122, 117, 111, 106, 101, 96, 91, 86, 81, 76, 71, 66, 61, 57, 53, 48, 44, 40, 36, 33, 29, 26, 23, 20, 17, 14, 12, 10, 8, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 9, 11, 14, 16, 19, 22, 25, 28, 32, 35, 39, 43, 47, 51, 56, 60, 65, 70, 74, 79, 84, 89, 94, 100, 105, 110, 115, 121};



//int * testsong[] = {(int*) 12, a4, a5, empty, a5, a4, a5, a4, a5, empty, a4, a5, a4};
//int * testsong[] = {(int*) 12, a4, d4, empty, d4, a4, d4, a4, d4, empty, a4, d4, a4};
int * testsong[] = {(int*) 12, a4, a4, a4, a4, a4, a4, a4, a4, a4, a4, a4, a4};
//int * testsong[] = {(int*) 30, a4, a5, a5, a4, a5, a4, a5, a4, a4, a4, a4, a5, a5, a4, a5, a4, a5, a5, a4, a5, a4, a5, a5, a4, a5, a4, a5, a5, a4, a5};



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
void playSample();



/* Functions to use after testing. May remove those above. */
void nextNote()
{
	current_note++;
	if (current_note > (int) current_song[0])
	{
		current_song = 0;
		return;
	}
	double tempFix = 16;
	current_note_duration = (SAMPLE_FREQUENCY / tempFix) / (int)current_song[current_note][0];
}

void setSong(int ** input_song)
{
	current_note = 0;
	current_sample = 1;
	current_song = testsong;//input_song;
	nextNote();
	//enable something timer?
}



void playSampleList(int * input_note)
{
	for (int i=1; i<(input_note[0]+1); i++)
	{
		*DAC0_CH0DATA = input_note[i];
	}
}

void playNoteList()
{
	playSampleList(current_song[current_note]);
	current_note_duration--;
	if (current_note_duration <= 0)
	{
		nextNote();
	}
}

void playEntireSong()
{
	while (!(current_song == 0)){
		playNoteList();
	}
	//*GPIO_PA_DOUT = 0x3C00;	
}



void playSample()
{
	*DAC0_CH0DATA = current_song[current_note][current_sample] *10;
	current_sample = (current_sample + 1);
	if (current_sample > current_song[current_note][0])
	{
		current_note_duration--;
		current_sample = 1;
	}
}

void playNote()
{
	playSample();
	if (current_note_duration <= 0)
	{
		nextNote();
	}
}

void playSong()
{
	if (current_song <= 0)
	{
		//*GPIO_PA_DOUT = 0x3C00;
		return;
	}
	playNote();
}







void runThis()
{
	//playEntireSong();		//Remember to set tempFix = 1 for current_note_duration. 
	
	playSong();
}

