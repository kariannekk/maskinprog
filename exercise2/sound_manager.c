#include <stdio.h>
#include <stdbool.h>

#include "efm32gg.h"

/* temp define. removed 'D' to prevent multi-instance. */
#define SAMPLE_PERIO 0xFFF0

void runDAC(uint16_t sampleAmount);

/* Global variables for tracking. */
int current_song;
int current_note;
//current period of note
//volume

/* Functions. */
//setup and begin playing song
//send constant sample length to DAC. based on SAMPLE_PERIOD?
	//Will jumps be noticed? if we end note A in its upper cycle and immediately switch to note B in its lower?
//set volume

void playSong(int input_song)
{
	current_note = 0;
	current_song = input_song;
	
	
}

void poll_the_timer()
{
	if (*TIMER1_CNT >= (SAMPLE_PERIO - 100))
	{
		runDAC(300);
	}
}



/* temp inputs. */
/*
uint32_t a4[] = { 127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 234, 239, 242, 245, 248, 250, 252, 253, 254, 254, 254, 254, 252, 251, 248, 246, 243, 239, 235, 231, 226, 220, 215, 209, 203, 196, 189, 182, 175, 167, 160, 152, 144, 136, 128, 120, 112, 104, 96, 89, 81, 74, 67, 60, 53, 47, 41, 35, 30, 25, 20, 16, 12, 9, 6, 4, 2, 1, 0, 0, 0, 0, 2, 3, 5, 8, 11, 15, 19, 23, 28, 33, 39, 44, 51, 57, 64, 71, 78, 86, 94, 101, 109, 117 };
uint32_t a5[] = { 127, 143, 159, 174, 188, 202, 214, 225, 234, 242, 248, 252, 254, 254, 252, 248, 243, 235, 226, 215, 203, 189, 175, 160, 144, 128, 112, 96, 81, 67, 53, 41, 30, 20, 12, 6, 2, 0, 0, 2, 5, 11, 19, 28, 39, 51, 64, 78, 94, 109 };
uint16_t a_length = 50; //100;
*/

