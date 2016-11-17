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

#include "game.h"
//#include 


//write(gamepadDevice, &buffer[0], 1); --Stop timer3
//write(gamepadDevice, &buffer[1], 1); --Start timer3
//write(gamepadDevice, &buffer[2], 1); --Start kernel timer (ball timer)
//write(gamepadDevice, &buffer[3], 1); --Stop kernel timer

void printStatusRacket(){
	printf("Left-- Postion: %d Pixel: %d \n", currentLeftPosition, previous_pixel_racket_left);
	printf("Right-- Postion: %d Pixel: %d \n", currentRightPosition, previous_pixel_racket_right);
}

void printBall(){
	printf("BALL-- Cord(%d, %d) Dir(%d, %d) Speed: %d \n", ball.xCoord, ball.yCoord, ball.xDir, ball.yDir, ball.speed);
}

void moveStep(int yStep, int xStep)
{
	if(ball.xDir == BALL_LEFT){
		ball.xCoord = ball.xCoord - xStep;
	}
	else{
		ball.xCoord = ball.xCoord + xStep;
	}
			
	if(ball.yDir == BALL_UP){
		ball.yCoord = ball.yCoord + yStep;
	}
	else{
		ball.yCoord = ball.yCoord - yStep;
	}
}

void moveBall(){
	switch(ball.speed){
		case 0DEG:
			moveStep(0, BALL_STEP1)
			break;
		case 30DEG:
			moveStep(BALL_STEP1, BALL_STEP2)
			break;
		case 45DEG:
			moveStep(BALL_STEP1, BALL_STEP1)
			break;
		case 60DEG:
			moveStep(BALL_STEP2, BALL_STEP1)
			break;
		default:
			return;
	}
	
	/* Check if ball has reached a wall and needs to change direction */
	if(ball.yCoord >= UPPER_WALL){
		ball.yCoord = UPPER_WALL;
		ball.yDir = ~ball.yDir;
	}
	else if(ball.yCoord <= LOWER_WALL){
		ball.yCoord = LOWER_WALL;
		ball.yDir = ~ball.yDir;		
	}
	
	/* Check is ball has reach a racket or it is a goal */
	if(ball.xCoord <= LEFT_RACKET_WALL){
		if(previous_pixel_racket_left < (ball.yCoord + PIXEL_BALL_DIAMETER){
			printf("Goal player 1\n");
			startBall();
		}
		else if((previous_pixel_racket_left + PIXEL_RACKET_HEIGHT) > ball.yCoord){
			ball.xCoord = LEFT_RACKET_WALL;
			printf("Goal player 1\n");
			startBall();
		}
		else{
			ball.xCoord = LEFT_RACKET_WALL;
		}
	}
	else if(ball.xCoord >= RIGHT_RACKET_WALL){
		if(previous_pixel_racket_right < (ball.yCoord + PIXEL_BALL_DIAMETER){
			printf("Goal player 2\n");
			startBall();
		}
		else if((previous_pixel_racket_right + PIXEL_RACKET_HEIGHT) > ball.yCoord){
			ball.xCoord = LEFT_RACKET_WALL;
			printf("Goal player 2\n");
			startBall();
		}
		else{
			ball.xCoord = RIGHT_RACKET_WALL;
		}
	}
	else{
		ball.xDir = ~ball.xDir;
		ball.yDir = ~ball.yDir;
		ball.speed = newBallSpeed();
	}
}


void startBall()
{
	ball.speed = 0DEG;
	ball.xCoord = PIXEL_X_INIT_BALL;
	ball.yCoord = PIXEL_Y_INIT_BALL;
	ball.xDir = BALL_INIT;
	ball.yDir = BALL_INIT;

	randomBallDirection();
	randomBallSpeed();
	//print ball;
}

void randomBallDirection(){
	int randNumb = getRandomNumber() % 4;
	switch(randNumb){
		case 0:
			ball.xDir = BALL_RIGHT;
			ball.yDir = BALL_DOWN;
		case 1:
			ball.xDir = BALL_RIGHT;
			ball.yDir = BALL_UP;
		case 2:
			ball.xDir = BALL_LEFT;
			ball.yDir = BALL_DOWN;
		case 3:
			ball.xDir = BALL_LEFT;
			ball.yDir = BALL_UP;
		default:
		 return;
	}
}

void newBallSpeed(){
	int randNumb = getRandomNumber();
	switch(ball.speed){
		case 0DEG :
			if(randNumb > 3){ //4-11
				ball.speed = 30DEG;
			}
			//0-3 unchanged
			break;
		case 30DEG :
			if(randNumb < 4){ //0-3
				ball.speed = 0DEG;
			}
			else if(randNumb > 6){ //7-11
				ball.speed = 45DEG
			}
			//4-6 unchanged
			break;
		case 45DEG :
			if(randNumb < 4){ //0-3
				ball.speed = 60DEG;
			}
			else if(randNumb > 6){ //7-11
				ball.speed = 30DEG
			}
			//4-6 unchanged
			break;
		case 60DEG :
			if(randNumb > 3){//4-11
			 ball.speed = 45DEG;
			}
			//0-3 unchanged
			break;
		default :
			return;
	}
	
}

void moveRacket(int direction, int racket)
{
	int tempPosition;
	switch(racket){
		case LEFT:
			tempPosition = currentLeftPosition + direction;
			if(tempPosition > UPPER_POSITION) || (tempPosition < LOWER_POSITION){
			currentLeftPosition = tempPosition;
			previous_pixel_racket_left = (currentLeftPosition * RACKET_STEP_LENGTH) + WALL_THICKNESS;
			//send to display;
			}
			break;
		case RIGHT:
			tempPosition = currentRightPosition + direction;
			if(tempPosition > UPPER_POSITION) || (tempPosition < LOWER_POSITION){
			currentRightPosition = tempPosition;
			previous_pixel_racket_right = (currentRightPosition * RACKET_STEP_LENGTH) + WALL_THICKNESS;
			//send to display;
			}
			break;
		default:
		 return;
	}

}


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
	/* Perform actions from left sided buttons */
	switch(button & 0x0F){
	case 0x0E :
		printf("Button pushed: SW1\n");
		// Perform action
		break;
	case 0x0D :
		//printf("Button pushed: SW2\n");
		moveRacket(UP, LEFT);
		break;
	case 0x0B :
		printf("Button pushed: SW3\n");
		// Perform action
		break;
	case 0x07 :
		//printf("Button pushed: SW4\n");
		moveRacket(DOWN, LEFT);
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
		//printf("Button pushed: SW6\n");
		moveRacket(UP, RIGHT);
		break;
	case 0xB0 :
		printf("Button pushed: SW7\n");
		// Perform action
		break;
	case 0x70 :
		//printf("Button pushed: SW8\n");
		moveRacket(DOWN, RIGHT);
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
			moveBall();
		default:
			return;
	}
}

void setupGame()
{
	gameStatus = PLAYING;
	currentLeftPosition = 5;
	currentRightPosition = 5;
	startBall();
	previous_pixel_racket_left = (currentLeftPosition * RACKET_STEP_LENGTH) + WALL_THICKNESS;
	previous_pixel_racket_right = (currentRightPosition * RACKET_STEP_LENGTH) + WALL_THICKNESS;
}

int initGame()
{
	/* Set up game features */
	setupGame();
	
	/* Connect to framework - print board */
/*	if(initFramework == ERROR){
		return ERROR;
	}*/

	/* Connect to gamepad driver - get access to hardware */
	if(setupGamepadDriver() == ERROR){
		return ERROR;
	}
	
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	if (initGame() == ERROR){
		return 0;
	}

	//write(gamepadDevice, &buffer[2], 1); --Start kernel timer (ball timer)

	while(gameStatus){
		
	}
	
	close(gamepadDevice);
	
	return 0;
}


