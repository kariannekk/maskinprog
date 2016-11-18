#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "movement.h"
#include "display.h"

/* Racket */
#define UPPER_POSITION 9
#define LOWER_POSITION 0


/* Ball */
#define SIXTY_DEG 		4
#define FORTYFIVE_DEG	3
#define THIRTY_DEG		2
#define ZERO_DEG		1

#define BALL_UP		-1
#define BALL_DOWN	 1
#define BALL_INIT	 0
#define BALL_RIGHT	 1
#define BALL_LEFT	-1

#define BALL_STEP1 5
#define BALL_STEP2 4



struct ballInfo{
	int xCoord;
	int yCoord;
	int speed;
	int xDir;
	int yDir;
} ball;

int currentLeftPosition;
int currentRightPosition;

void moveStep(int xStep, int yStep)
{
//	if(ball.xDir == BALL_LEFT){
//		ball.xCoord = ball.xCoord - xStep;
//	}
//	else{
//		ball.xCoord = ball.xCoord + xStep;
//	}
			
//	if(ball.yDir == BALL_UP){
//		ball.yCoord = ball.yCoord - yStep;
//	}
//	else{
//		ball.yCoord = ball.yCoord + yStep;
//	}
	ball.xCoord = ball.xCoord + ball.xDir*xStep;
	ball.yCoord = ball.yCoord + ball.yDir*yStep;
}

void moveBall(){
	switch(ball.speed){
		case ZERO_DEG:
			moveStep(BALL_STEP1, 0);
			break;
		case THIRTY_DEG:
			moveStep(BALL_STEP1, BALL_STEP2);
			break;
		case FORTYFIVE_DEG:
			moveStep(BALL_STEP1, BALL_STEP1);
			break;
		case SIXTY_DEG:
			moveStep(BALL_STEP2, BALL_STEP1);
			break;
		default:
			return;
	}
	
	/* Check if ball has reached a wall and needs to change direction */
	if(ball.yCoord <= BALL_UPPER_WALL){
		ball.yCoord = BALL_UPPER_WALL;
		ball.yDir = BALL_DOWN;
		printf("1\n");
	}
	else if(ball.yCoord >= BALL_LOWER_WALL){
		ball.yCoord = BALL_LOWER_WALL;
		ball.yDir = BALL_UP;
		printf("2\n");	
	}
	
	/* Check if ball has reach a racket or it is a goal */
	if(ball.xCoord <= LEFT_RACKET_WALL){
		if(pixelRacetLeft() > (ball.yCoord + PIXEL_BALL_DIAMETER)){
			printf("Goal player 2\n");
			startBall();
			printf("3\n");
		}
		else if((pixelRacetLeft() + PIXEL_RACKET_HEIGHT) < ball.yCoord){
			ball.xCoord = LEFT_RACKET_WALL;
			printf("Goal player 2\n");
			startBall();
			printf("4\n");
		}
		else{
			ball.xCoord = LEFT_RACKET_WALL;
			printf("5\n");
			ball.xDir = BALL_RIGHT;
			//if((ball.yCoord != BALL_UPPER_WALL) || (ball.yCoord != BALL_LOWER_WALL)){
			//	ball.yDir = -ball.yDir;
			//}
			newBallSpeed();
		}
	}
	else if(ball.xCoord >= RIGHT_RACKET_WALL){
		if(pixelRacetRight() > (ball.yCoord + PIXEL_BALL_DIAMETER)){
			printf("Goal player 1\n");
			startBall();
			printf("6\n");
		}
		else if((pixelRacetRight() + PIXEL_RACKET_HEIGHT) < ball.yCoord){
			ball.xCoord = RIGHT_RACKET_WALL;
			printf("Goal player 1\n");
			startBall();
			printf("7\n");
		}
		else{
			ball.xCoord = RIGHT_RACKET_WALL;
			printf("8\n");
			ball.xDir = BALL_LEFT;
			//if((ball.yCoord != BALL_UPPER_WALL) || (ball.yCoord != BALL_LOWER_WALL)){
			//	ball.yDir = -ball.yDir;
			//}
			newBallSpeed();
		}
	}
	
	//REMOVE
	//printBall();
	
	move_ball(ball.xCoord, ball.yCoord);
	
}


void startBall()
{
	ball.speed = ZERO_DEG;
	ball.xCoord = PIXEL_BALL_INITIAL_X;
	ball.yCoord = PIXEL_BALL_INITIAL_Y;
	ball.xDir = BALL_INIT;
	ball.yDir = BALL_INIT;

	randomBallDirection();
	newBallSpeed();
	
	//REMOVE
	//printBall();
}

void randomBallDirection(){
	int randNumb = rand() % 4;
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
	int randNumb = rand() % 12;
	switch(ball.speed){
		case ZERO_DEG :
			if(randNumb > 3){ //4-11
				ball.speed = THIRTY_DEG;
			}
			//0-3 unchanged
			break;
		case THIRTY_DEG :
			if(randNumb < 4){ //0-3
				ball.speed = ZERO_DEG;
			}
			else if(randNumb > 6){ //7-11
				ball.speed = FORTYFIVE_DEG;
			}
			//4-6 unchanged
			break;
		case FORTYFIVE_DEG :
			if(randNumb < 4){ //0-3
				ball.speed = SIXTY_DEG;
			}
			else if(randNumb > 6){ //7-11
				ball.speed = THIRTY_DEG;
			}
			//4-6 unchanged
			break;
		case SIXTY_DEG :
			if(randNumb > 3){//4-11
			 ball.speed = FORTYFIVE_DEG;
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
			if((tempPosition <= UPPER_POSITION) && (tempPosition >= LOWER_POSITION)){
				currentLeftPosition = tempPosition;
				//Display
				move_left_racket(currentLeftPosition);
			}
			break;
		case RIGHT:
			tempPosition = currentRightPosition + direction;
			if((tempPosition <= UPPER_POSITION) && (tempPosition >= LOWER_POSITION)){
				currentRightPosition = tempPosition;
				//Display
				move_right_racket(currentRightPosition);
			}
			break;
		default:
		 return;
	}

	//REMOVE
	//printStatusRacket();
}

void setupGame()
{
	currentLeftPosition = RACKET_INITIAL_POSITION;
	currentRightPosition = RACKET_INITIAL_POSITION;

	startBall();

	srand(time(NULL));

	//Remove
	//printStatusRacket();
}
