#include <stdio.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "notes.h"

/* temp define. */
#define SAMPLES_PER_SECOND 44100
#define EIGHT_NOTE (SAMPLES_PER_SECOND / 8)
#define VOLUME 10

/* Declaration of peripheral setup functions. */
void setNormalSleepMode();
void setDeepSleepMode();

/* Global variables for tracking. */
int **current_song;
int current_note;
int current_sample;
int current_note_duration;

/* Track the song note, duration and end of the song. */
void nextNote()
{
	current_note++;
	if (current_note > (int)current_song[0]) {
		current_song = 0;
		setDeepSleepMode();	//Disables Timer, etc. 
		return;
	}

	/* Make every note last for the same time duration, even when the note vectors have different lengths. */
	current_note_duration = EIGHT_NOTE / (int)current_song[current_note][0];
}

/* Add a song to the now playing queue (queue size = one song). */
void setSong(int **input_song)
{
	current_note = 0;
	current_sample = 1;
	current_song = input_song;
	nextNote();
	setNormalSleepMode();	//Enables Timer, etc. 
}

/* Send one sample to the DAC, and track the sample list length. */
void playSample()
{
	/* Upload sample to DAC. */
	*DAC0_CH0DATA = current_song[current_note][current_sample] * VOLUME;
	*DAC0_CH1DATA = current_song[current_note][current_sample] * VOLUME;

	/* Find the next sample. */
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
void playSong()
{
	if (current_song != 0) {
		playNote();
	}
}

/* Set song corresponding to button press, without overwriting a song that is already playing. (1-indexed buttons.) */
int selectSongFromButton(int input_button)
{
	if (current_song != 0) {
		return 0;
	}
	switch (input_button) {
	case 2:
		setSong(SONG_FANFARE);
		return 1;
	case 4:
		setSong(SONG_PEW);
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
	default:
		return 0;
	}
}

void playIntroSong()
{
	setSong(SONG_INTRO);
}
