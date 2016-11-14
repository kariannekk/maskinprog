#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <signal.h>
#include <stdint.h>
#include <poll.h>

/* Setup/init returns */
#define ERROR -1
#define SUCCESS 1

/* State machine */
#define PLAYING 1
#define STOPP 0
#define PAUSE 2

struct fb_copyarea rect;
int gamepadDevice;

int gameStatus;
struct sigaction act;
char buffer[4] = {'0', '1', '2', '3'};
//write(gamepadDevice, &buffer[0], 1); --Stop timer3
//write(gamepadDevice, &buffer[1], 1); --Start timer3
//write(gamepadDevice, &buffer[2], 1); --Start kernel timer (ball timer)
//write(gamepadDevice, &buffer[3], 1); --Stop kernel timer

void signalHandler(int signal, siginfo_t *info, void *ptr);


int setupGamepadDriver()
{
	/* Open device driver as a "file" */
	gamepadDevice = open("dev/gamepad", O_RDWR);
	if(!gamepadDevice){
		printf("Could not open gamepad device!\n");
		return ERROR;
	}
	
	/* Set up signal handling */
	struct sigaction sigHandler;
	sigHandler.sa_flags = SA_SIGINFO;
	sigHandler.sa_sigaction = signalHandler;
	
	if((sigaction(SIGIO, &sigHandler, NULL) < 0)){
		printf("Could not register the signal handler!\n");
		return ERROR;
	}
	
	if((fcntl(gamepadDevice, F_SETOWN, getpid( )) < 0)){
		printf("Could not set pid as owner, F_SETOWN!\n");
		return ERROR;
	}
	
	if((fcntl(gamepadDevice, F_SETSIG, SIGIO) < 0)){
		printf("F_SETSIG!\n");
		return ERROR;
	}
	
	long oflags = fcntl(gamepadDevice, F_GETFL);
	
	if((fcntl(gamepadDevice, F_SETFL, oflags | FASYNC) < 0)){
		printf("Could not set FASYNC flag!\n");
		return ERROR;
	}
	
	printf("Success\n");
	
	return SUCCESS;
}

void ButtonHandler(uint8_t button)
{
	int pixel_racket_initial_position = PIXEL_RACKET_INITIAL_POSITION;
	
	/* Perform actions from left sided buttons */
	switch(button & 0x0F){
	case 0x0E :
		printf("Button pushed: SW1\n");
		// Perform action
		break;
	case 0x0D :
		printf("Button pushed: SW2\n");
		// Perform action
		//move_racket_left_up(pixel_racket_initial_position);
		break;
	case 0x0B :
		printf("Button pushed: SW3\n");
		// Perform action
		break;
	case 0x07 :
		printf("Button pushed: SW4\n");
		// Perform action
		//move_racket_left_down(pixel_racket_initial_position);
		break;
	default:
		break;
	}

	/* Perform actions from right sided buttons */
	switch(button & 0xF0){
	case 0xE0 :
		printf("Button pushed: SW5\n");
		// Perform action
		break;
	case 0xD0 :
		printf("Button pushed: SW6\n");
		// Perform action
		//move_racket_right_up(pixel_racket_initial_position);
		break;
	case 0xB0 :
		printf("Button pushed: SW7\n");
		// Perform action
		break;
	case 0x70 :
		printf("Button pushed: SW8\n");
		// Perform action 
		//move_racket_right_down(pixel_racket_initial_position);
		break;
	default :
		break;
	}
}

void signalHandler(int signal, siginfo_t *info, void *ptr)
{
	switch (info->si_code){
		case POLL_IN: //GPIO interrupt
			uint8_t button;
			read(gamepadDevice, &button, 1);
		case POLL_OUT: //Timer3 interrupt
			// Play sound sample
		case POLL_MSG: //Kernel timer interrupt
			// Update ball
		default:
			return;
	}
}

void setupGame()
{
	gameStatus = PLAYING;
}

int initGame()
{
	/* Connect to gamepad driver - get access to hardware */
	if(setupGamepadDriver() == ERROR){
		return ERROR;
	}
	
	/* Connect to framework - print board */
/*	if(initFramework == ERROR){
		return ERROR;
	}*/

	/* Set up game features */
	setupGame();
	
	
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	if (initGame() == ERROR){
		return 0;
	}


	while(gameStatus){
		//Test if interrupt works 
		/*
		if(timer) {
			move_ball();
		}
		*/
	}
	
	close(gamepadDevice);
	
	return 0;
}


