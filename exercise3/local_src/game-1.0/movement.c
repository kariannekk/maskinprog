#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "movement.h"
#include "display.h"

/* Racket */
#define UPPER_POSITION		0
#define LOWER_POSITION		(RACKET_STEP_AMOUNT - 1)


/* Ball */
#define SIXTY_DEG 			4
#define FORTYFIVE_DEG		3
#define THIRTY_DEG			2
#define ZERO_DEG			1

#define BALL_UP			   -1
#define BALL_DOWN			1
#define BALL_INIT			0
#define BALL_RIGHT			1
#define BALL_LEFT		   -1

#define BALL_STEP1			5
#define BALL_STEP2			4



struct ballInfo{
	int xCoord;
	int yCoord;
	int offsetAngle;
	int xDir;
	int yDir;
} ball;

int currentLeftPosition;
int currentRightPosition;

int scoreRacketLeft;
int scoreRacketRight;

void scoreBoard(){
	printf("--------------------------------------\n");
	printf("   PLAYER 1                PLAYER 2   \n");
	printf("   SCORE: %d               SCORE: %d  \n", scoreRacketLeft, scoreRacketRight);
	printf("--------------------------------------\n");
}

void moveStep(int xStep, int yStep)
{
	ball.xCoord = ball.xCoord + ball.xDir*xStep;
	ball.yCoord = ball.yCoord + ball.yDir*yStep;
}

void moveBall(){
	switch(ball.offsetAngle){
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
	}
	else if(ball.yCoord >= BALL_LOWER_WALL){
		ball.yCoord = BALL_LOWER_WALL;
		ball.yDir = BALL_UP;
	}
	
	/* Check if ball has reached a racket or it is a goal */
	if(ball.xCoord <= LEFT_RACKET_WALL){
		ball.xCoord = LEFT_RACKET_WALL;
		if ((getPixelRacketLeft() > (ball.yCoord + PIXEL_BALL_DIAMETER))
		|| ((getPixelRacketLeft() + PIXEL_RACKET_HEIGHT) < ball.yCoord)){
			printf("GOAL PLAYER 2\n");
			scoreRacketRight += 1;
			startBall();
		}
		else{
			ball.xDir = BALL_RIGHT;
			newBallOffsetAngle();
		}
	}
	else if(ball.xCoord >= RIGHT_RACKET_WALL){
		ball.xCoord = RIGHT_RACKET_WALL;
		if ((getPixelRacketRight() > (ball.yCoord + PIXEL_BALL_DIAMETER))
		|| ((getPixelRacketRight() + PIXEL_RACKET_HEIGHT) < ball.yCoord)){
			printf("GOAL PLAYER 1\n");
			scoreRacketLeft += 1;
			startBall();
		}
		else{
			ball.xDir = BALL_LEFT;
			newBallOffsetAngle();
		}
	}

	displayMoveBall(ball.xCoord, ball.yCoord);
	
}


void startBall()
{
	scoreBoard();
	
	ball.offsetAngle = ZERO_DEG;
	ball.xCoord = PIXEL_BALL_INITIAL_X;
	ball.yCoord = PIXEL_BALL_INITIAL_Y;
	ball.xDir = BALL_INIT;
	ball.yDir = BALL_INIT;

	randomBallDirection();
	newBallOffsetAngle();
}

void randomBallDirection(){
	srand(time(NULL));
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


void newBallOffsetAngle(){
	int randNumb = rand() % 12;
	switch(ball.offsetAngle){
		case ZERO_DEG :
			if(randNumb > 3){ //4-11
				ball.offsetAngle = THIRTY_DEG;
			}
			//0-3 unchanged
			break;
		case THIRTY_DEG :
			if(randNumb < 1){ //0
				ball.offsetAngle = ZERO_DEG;
			}
			else if(randNumb > 5){ //6-11
				ball.offsetAngle = FORTYFIVE_DEG;
			}
			//1-5 unchanged
			break;
		case FORTYFIVE_DEG :
			if(randNumb < 4){ //0-3
				ball.offsetAngle = SIXTY_DEG;
			}
			else if(randNumb > 7){ //8-11
				ball.offsetAngle = THIRTY_DEG;
			}
			//4-7 unchanged
			break;
		case SIXTY_DEG :
			if(randNumb > 6){//7-11
			 ball.offsetAngle = FORTYFIVE_DEG;
			}
			//0-6 unchanged
			break;
		default :
			return;
	}
}


void moveRightRacket(int direction)
{
	int tempPosition = currentRightPosition + direction;
	if((tempPosition >= UPPER_POSITION) && (tempPosition <= LOWER_POSITION)){
		currentRightPosition = tempPosition;
		displayMoveRightRacket(currentRightPosition);
	}
}

void moveLeftRacket(int direction)
{
	int tempPosition = currentLeftPosition + direction;
	if((tempPosition >= UPPER_POSITION) && (tempPosition <= LOWER_POSITION)){
		currentLeftPosition = tempPosition;
		displayMoveLeftRacket(currentLeftPosition);
	}
}

void setupGame()
{
	currentLeftPosition = RACKET_INITIAL_POSITION;
	currentRightPosition = RACKET_INITIAL_POSITION;

	scoreRacketLeft = 0;
	scoreRacketRight = 0;

	startBall();

	//srand(time(NULL));
}
