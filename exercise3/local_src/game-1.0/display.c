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
#include <string.h>
#include <stdint.h>

#include "display.h"

/* 320x240 display, 0-indexed. */
#define PIXEL_AMOUNT				(PIXEL_MAX_LINE * PIXEL_MAX_COLUMN) //(240*320)  //152960  //153600

/* Pixel colour setup. */
#define COLOUR_BLACK	0x0000
#define COLOUR_WHITE	0xFFFF
#define COLOUR_RED		0x3800	//0xF800
#define COLOUR_GREEN	0x07E0
#define COLOUR_BLUE		0x001F

/* Wall setup. */
#define PIXEL_UPPER_WALL_END	(WALL_THICKNESS * PIXEL_MAX_COLUMN) //Start at 0.
#define PIXEL_LOWER_WALL_START	(PIXEL_AMOUNT - (WALL_THICKNESS * PIXEL_MAX_COLUMN)) //End at last pixel.


/* Racket */
#define PIXEL_ARRAY_RACKET_HEIGHT (PIXEL_RACKET_HEIGHT * PIXEL_MAX_COLUMN)


/* Ball */
#define PIXEL_BALL_Y_END			(PIXEL_BALL_DIAMETER * PIXEL_MAX_COLUMN)

unsigned int previous_pixel_racket_left;// = PIXEL_RACKET_INITIAL_POSITION;
unsigned int previous_pixel_racket_right; // = PIXEL_RACKET_INITIAL_POSITION;
uint32_t previous_array_pos_racket_left = PIXEL_RACKET_INITIAL_POSITION * PIXEL_MAX_COLUMN;
uint32_t previous_array_pos_racket_right = PIXEL_RACKET_INITIAL_POSITION * PIXEL_MAX_COLUMN;

unsigned int previous_pixel_ball_x = PIXEL_BALL_INITIAL_X;
unsigned int previous_pixel_ball_y = PIXEL_BALL_INITIAL_Y;
uint32_t previous_array_pos_ball_y = (PIXEL_BALL_INITIAL_Y  * PIXEL_MAX_COLUMN);

struct fb_copyarea rect;
int file_p;
uint16_t * address;

unsigned int pixelRacetLeft(){
	return previous_pixel_racket_left;
}

unsigned int pixelRacetRight(){
	return previous_pixel_racket_right;
}

void open_display(){
	file_p = open("/dev/fb0", O_RDWR);
	address = mmap(NULL, PIXEL_AMOUNT, PROT_READ|PROT_WRITE, MAP_SHARED, file_p, 0);
}

void close_display(){
	munmap(address, PIXEL_AMOUNT);	//Perhaps we do not need to repeatedly declare this? See test11()?
	close(file_p);
}

void refresh_rectangle(int startX, int startY, int lengthX, int lengthY){
	rect.dx = startX;
	rect.dy = startY;
	rect.width = lengthX;
	rect.height = lengthY;
	
	ioctl(file_p, 0x4680, &rect);
}

void refresh_display(){
	refresh_rectangle(0, 0, PIXEL_MAX_COLUMN, PIXEL_MAX_LINE);
}

void clear_display(){
	uint32_t i = 0;
	for (i=0; i<PIXEL_AMOUNT; i++){
		address[i] = COLOUR_BLACK;
	}	
//	refresh_display();
}

void draw_walls(){
	uint32_t i = 0;
	for (i=0; i<PIXEL_UPPER_WALL_END; i++){
		address[i] = COLOUR_WHITE;
	}
//	refresh_rectangle(0, 0, PIXEL_MAX_COLUMN, WALL_THICKNESS);
	
	for (i=PIXEL_LOWER_WALL_START; i<PIXEL_AMOUNT; i++){
		address[i] = COLOUR_WHITE;
	}
//	refresh_rectangle(0, PIXEL_LOWER_WALL_START, PIXEL_MAX_COLUMN, WALL_THICKNESS);
}

void clear_racket_left(){
	uint32_t i, j;
	for (i=PIXEL_RACKET_LEFT_X_START; i<PIXEL_RACKET_LEFT_X_END; i++){
		for (j=i+ previous_array_pos_racket_left; j<previous_array_pos_racket_left + PIXEL_ARRAY_RACKET_HEIGHT; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_RED;
		}
	}
//	refresh_rectangle(PIXEL_RACKET_LEFT_X_START, previous_pixel_racket_left, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT);
}

void clear_racket_right(){
	uint32_t i, j;
	for (i=PIXEL_RACKET_RIGHT_X_START; i<PIXEL_RACKET_RIGHT_X_END; i++){
		for (j=i+ previous_array_pos_racket_right; j<previous_array_pos_racket_right + PIXEL_ARRAY_RACKET_HEIGHT; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_RED;
		}
	}
//	refresh_rectangle(PIXEL_RACKET_RIGHT_X_START, previous_pixel_racket_right, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT);
}

void draw_racket_left(unsigned int current_pixel){
	uint32_t current_array_position = current_pixel * PIXEL_MAX_COLUMN;
	
	previous_pixel_racket_left = current_pixel;
	previous_array_pos_racket_left = current_array_position;
	
	uint32_t i, j;
	for (i=PIXEL_RACKET_LEFT_X_START; i<PIXEL_RACKET_LEFT_X_END; i++){
		for (j=i+ current_array_position; j<current_array_position + PIXEL_ARRAY_RACKET_HEIGHT; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_WHITE;
		}
	}
//	refresh_rectangle(PIXEL_RACKET_LEFT_X_START, current_pixel, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT);
}

void draw_racket_right(unsigned int current_pixel){
	uint32_t current_array_position = current_pixel * PIXEL_MAX_COLUMN;
	
	previous_pixel_racket_right = current_pixel;
	previous_array_pos_racket_right = current_array_position;
	
	uint32_t i, j;
	for (i=PIXEL_RACKET_RIGHT_X_START; i<PIXEL_RACKET_RIGHT_X_END; i++){
		for (j=i + current_array_position; j< current_array_position + PIXEL_ARRAY_RACKET_HEIGHT; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_WHITE;
		}
	}
//	refresh_rectangle(PIXEL_RACKET_RIGHT_X_START, current_pixel, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT);
}


void clear_ball(){
	uint32_t i, j;
	for (i=previous_pixel_ball_x; i<previous_pixel_ball_x + PIXEL_BALL_DIAMETER; i++){
		for (j=i+ previous_array_pos_ball_y; j<i + previous_array_pos_ball_y + PIXEL_BALL_Y_END; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_RED;
		}
	}
//	refresh_rectangle(previous_pixel_ball_x, previous_pixel_ball_y, PIXEL_BALL_DIAMETER, PIXEL_BALL_DIAMETER);
//	refresh_rectangle(previous_pixel_ball_x-1, previous_pixel_ball_y-1, PIXEL_BALL_DIAMETER+2, PIXEL_BALL_DIAMETER+2); //For some reason this will miss a few pixels if it is the exact size it should be. Maybe because the pixels appear to be connected to neighbouring pixel (I think it looks as if two pixels are required for one colour).
}

void draw_ball(unsigned int current_pixel_x, unsigned int current_pixel_y){
//	current_pixel_x -= PIXEL_BALL_RADIUS;	//Change from center pixels to upper left corner pixels. 
//	current_pixel_y -= PIXEL_BALL_RADIUS;	//Change from center pixels to upper left corner pixels. 
	uint32_t current_array_position_y = current_pixel_y * PIXEL_MAX_COLUMN;
	
	previous_pixel_ball_x = current_pixel_x;
	previous_pixel_ball_y = current_pixel_y;
	previous_array_pos_ball_y = current_array_position_y;
	
	uint32_t i, j;
	for (i=current_pixel_x; i<current_pixel_x + PIXEL_BALL_DIAMETER; i++){
		for (j=i+ current_array_position_y; j<i + current_array_position_y + PIXEL_BALL_Y_END; j=j+PIXEL_MAX_COLUMN){
			address[j] = COLOUR_WHITE;
		}
	}
//	refresh_rectangle(current_pixel_x, current_pixel_y, PIXEL_BALL_DIAMETER, PIXEL_BALL_DIAMETER);
}

void display_init(){
	//Open file and memorymap it.
	open_display();
	
	//Remove linux penguin.
	clear_display();
	
	//Print upper and lower wall. 
	draw_walls();
	
	//Print rackets at their start position.
	draw_racket_left(PIXEL_RACKET_INITIAL_POSITION);
	draw_racket_right(PIXEL_RACKET_INITIAL_POSITION);
	
	//Print ball at start position. 
	draw_ball(PIXEL_BALL_INITIAL_X, PIXEL_BALL_INITIAL_Y);
	
	//Update framebuffer for display. 
	refresh_display();
	
	//Close file and unmap it.
	//close_display();
}



void move_left_racket(unsigned int position){
//	position = position % RACKET_STEP_AMOUNT; //Make the game handle such asserts instead of the display? 
	int pixel_y = (position * RACKET_STEP_LENGTH) + WALL_THICKNESS;
	
	clear_racket_left();
	refresh_rectangle(PIXEL_RACKET_LEFT_X_START, previous_pixel_racket_left-1, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT+2);
	
	draw_racket_left(pixel_y);
	refresh_rectangle(PIXEL_RACKET_LEFT_X_START, pixel_y-1, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT+2);
}

void move_right_racket(unsigned int position){
//	position = position % RACKET_STEP_AMOUNT; //Make the game handle such asserts instead of the display? 
	int pixel_y = (position * RACKET_STEP_LENGTH) + WALL_THICKNESS;
	
	clear_racket_right();
	refresh_rectangle(PIXEL_RACKET_RIGHT_X_START,  previous_pixel_racket_right-1, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT+2);
	
	draw_racket_right(pixel_y);
	refresh_rectangle(PIXEL_RACKET_RIGHT_X_START, pixel_y-1, PIXEL_RACKET_WIDTH, PIXEL_RACKET_HEIGHT+2);
}

void move_ball(unsigned int pixel_x, unsigned int pixel_y){
//	pixel_x = pixel_x % PIXEL_MAX_COLUMN; //Make the game handle such asserts instead of the display? 
//	pixel_y = pixel_y % PIXEL_MAX_LINE; //Make the game handle such asserts instead of the display? 
	
//	unsigned int step_y = (PIXEL_MAX_LINE - (2 * WALL_THICKNESS) - PIXEL_BALL_DIAMETER);
//	unsigned int step_x = (PIXEL_MAX_COLUMN - (2 * PIXELS_BEHIND_RACKETS) - (2 * PIXEL_RACKET_WIDTH) - PIXEL_BALL_DIAMETER);

	clear_ball();
	refresh_rectangle(previous_pixel_ball_x-1, previous_pixel_ball_y-1, PIXEL_BALL_DIAMETER+2, PIXEL_BALL_DIAMETER+2); //For some reason this will miss a few pixels if it is the exact size it should be. Maybe because the pixels appear to be connected to neighbouring pixel (I think it looks as if two pixels are required for one colour).
	
	draw_ball(pixel_x, pixel_y);
	refresh_rectangle(pixel_x, pixel_y, PIXEL_BALL_DIAMETER, PIXEL_BALL_DIAMETER);
	
//	refresh_display();
}



void game_over(){
	open_display();
	
	clear_ball();
	
	//F.eks. printe "Game over"?
	
	refresh_display();
	close_display();
}

void restart(){
	open_display();
	
//	clear_display();	//Could be safer or necessary to use these as well. 
//	draw_walls();
	
	move_left_racket(PIXEL_RACKET_INITIAL_POSITION);
	move_right_racket(PIXEL_RACKET_INITIAL_POSITION);
	move_ball(PIXEL_BALL_INITIAL_X, PIXEL_BALL_INITIAL_Y);
	
	refresh_display();
	close_display();
}


