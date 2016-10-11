#include <stdio.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "notes.h"

/* temp define. */
#define SAMPLES_PER_SECOND 44100

/* Global variables for tracking. */
int **current_song;
int current_note;
int current_sample;
int current_note_duration;
//volume



/* INIT */
void disableTimer();	//From file "timer.c".
void enableTimer();

/* Functions to use after testing. May remove those above. */

/* Track the song note, duration and end of the song. */
void nextNote()
{
	current_note++;
	if (current_note > (int)current_song[0]) {
		current_song = 0;
		*TIMER1_CMD = 0x2;	//Stops the timer. 
		return;
	}
	int tempFix = 8;	//TODO.
	current_note_duration =
	    (SAMPLES_PER_SECOND / tempFix) / (int)current_song[current_note][0];
}

/* Add a song to the now playing queue (queue size = one song). */
void setSong(int **input_song)
{
	current_note = 0;
	current_sample = 1;
	current_song = input_song;
	nextNote();
	*TIMER1_CMD = 0x1;	//Starts the timer.
}

/* Send one sample to the DAC, and track the sample list length. */
void playSample()
{
	*DAC0_CH0DATA = current_song[current_note][current_sample] * 10;	//TODO move volume constant someplace else. Actually possible to hear without it, in interrupt mode. May create a function for this. 
	
	*DAC0_CH1DATA = current_song[current_note][current_sample] * 10;	//TODO Same for ch1 as well?
	current_sample = (current_sample + 1);
	if (current_sample > current_song[current_note][0]) {
		current_note_duration--;
		current_sample = 1;
	}
}

/* Track the duration of repeating a note. */
void playNote()
{
	playSample();
	if (current_note_duration <= 0) {
		nextNote();
	}
}

/* Play one sample of a song if there is any song. */
int playSong()
{
	if (current_song == 0) {
		return 0;
	}
	playNote();
	return 1;
}

/* Set song corresponding to button press, without overwriting a song that is already playing. */
int selectSongFromButton(int input_button)
{
	if (current_song != 0) {
		return 0;
	}
	switch (input_button) {
	case 2:
		setSong(SONG_UP);
		return 1;
	case 4:
		setSong(SONG_UP);
		return 1;
	case 5:
		setSong(SONG_LEFT);
		return 1;
	case 6:
		setSong(SONG_UP);
		return 1;
	case 7:
		setSong(SONG_RIGHT);
		return 1;
	case 8:
		setSong(SONG_DOWN);
		return 1;
	case 9:
		setSong(SONG_INTRO);
		return 1;
	default:
		return 0;
	}
}

/* Test function. Send entire wave list sequentially. */
void playSampleList(int *input_note)
{
	for (int i = 1; i < (input_note[0] + 1); i++) {
		*DAC0_CH0DATA = input_note[i];
	}
}

/* Test function. Track the duration of repeating a note. */
void playNoteList()
{
	playSampleList(current_song[current_note]);
	current_note_duration--;
	if (current_note_duration <= 0) {
		nextNote();
	}
}

/* Test function. Sequentially run the song, non-stop. */
void playEntireSong()
{	//Remember to set tempFix = 1 for current_note_duration. 
	while ((current_song != 0)) {
		playNoteList();
	}
}
