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



#define ERROR -1
#define SUCCESS 1

/* 320x240 display, 0-indexed. */
#define PIXEL_MAX_LINE				238  //239
#define PIXEL_MAX_COLUMN			318  //319

/* Two bytes per pixel. */
#define PIXEL_BYTES					152960  //153600
#define PIXEL_BYTE_LAST_POSITION	152958  //<- actual. Said to be: 153592.
#define PIXEL_BYTE_NEXT_LINE_FACTOR		640
#define PIXEL_BYTE_NEXT_COLUMN_OFFSET	2

/* Pixel colour setup. */
#define COLOUR_BLACK	((char*) 0x01)	//memcpy() didn't like NULL. This value is impossible to tell apart from 0x00.
#define COLOUR_WHITE	((char*) 0xFF)
#define COLOUR_GREEN	((char*) 0x5A)

/* Wall setup */
#define WALL_THICKNESS		13	//In pixel lines.

/* Formulas; to reduce time spent calculating. */
#define PIXEL_BYTE_UPPER_WALL_END (WALL_THICKNESS * PIXEL_BYTE_NEXT_LINE_FACTOR) //Start at 0.
#define PIXEL_LOWER_WALL_START (PIXEL_MAX_LINE - WALL_THICKNESS)
#define PIXEL_BYTE_LOWER_WALL_START ((PIXEL_MAX_LINE - WALL_THICKNESS) * PIXEL_BYTE_NEXT_LINE_FACTOR) //End at max.





struct fb_copyarea rect;
int gamepadDevice;



int setupGamepadDriver();
int display();



int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");
	
	if (setupGamepadDriver() == ERROR){
		return 0;
	}
	
	display();
	
	/*while(1){
		//Test if interrupt works 
	}*/
	
	close(gamepadDevice);
	
	return 0;
}



int setupGamepadDriver()
{
	gamepadDevice = open("dev/gamepad", O_RDONLY);
	if(!gamepadDevice){
		printf("Could not open gamepad device\n");
		return ERROR;
	}
	return SUCCESS;
}








/*
void test1(){
//Test writing other than {echo "ZZZZZZZZZ" > /dev/fb0}.
//It appears we must write an odd number of letters, e.g. "ZZZ" or "ZZZZZ" etc. In terminal -> echo there was a sh-error if the number was even. 
	int fd = open("/dev/fb0", O_RDWR);
	printf("fd: %i\n", fd);
	int offset = lseek(fd, 100960, SEEK_SET); //Set position near middle of screen, towards bottom right corner.
	printf("offset: %i\n", offset);
	int ok = write(fd, "ZZZZZZZZZZZZZZZZZZZZZZZZZ", 40);
	printf("ok: %i\n", ok);
	close(fd);
	printf("fd: %i\n", fd);
}*/

/*
void test2(){
//Test writing as memory mapped instead of direct.
	int fd = open("/dev/fb0", O_RDWR);
	printf("fd: %i\n", fd);
	char * address = NULL;
	address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	printf("address: %i\n", address == MAP_FAILED);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	// *(address + 50) = "ZZZ"; //Writes only one pixel.
	// *(address + PIXEL_BYTE_LAST_POSITION - 700) = "ZZZ"; //Writes only one pixel.
	*(address + 152960) = "ZZZ"; //Writes only one pixel.
	int noe = ioctl(fd, 0x4680, &rect);
	printf("noe: %i\n", noe);
	close(fd);
}*/

/*
void test3(){
//Test writing memory mapped, but more than one pixel.
	int fd = open("/dev/fb0", O_RDWR);
	printf("fd: %i\n", fd);
	char * address = NULL;
	address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	printf("address: %i\n", address == MAP_FAILED);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	memcpy(address+34000,  //Start somewhere other than the upper left corner. 
	"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"
	, 3000);
	int noe = ioctl(fd, 0x4680, &rect);
	printf("noe: %i\n", noe);
	close(fd);
}*/

/*
void test4(){
//Test writing colours. 
//Found 0xFF to be white, and 0x00 to be black. 'Z'=0x5A appears blue. Found 0x5A to be green later, in mmap.
	int fd = open("/dev/fb0", O_RDWR);
	printf("fd: %i\n", fd);
	char * address = NULL;
	address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	printf("address: %i\n", address == MAP_FAILED);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	char temp[3000];
	int i = 0;
	for (i=0; i<3000; i++){	//Create long string for easier viewing. 
		temp[i] = 0x00; //Select colour. 
	}
	memcpy(address+34000, temp, 3000); //Start somewhere other than the upper left corner. 
	int noe = ioctl(fd, 0x4680, &rect);
	printf("noe: %i\n", noe);
	close(fd);
}*/

/*
void test5(){
//Test writing to entire display.
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = NULL;
	address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = PIXEL_MAX_COLUMN ;
	rect.height = PIXEL_MAX_LINE ;
	
	char display_colour[PIXEL_BYTE_NEXT_LINE_FACTOR];
	for (i=0; i<(PIXEL_BYTE_NEXT_LINE_FACTOR - PIXEL_BYTE_NEXT_COLUMN_OFFSET); i++){
		display_colour[i] = 0x00; //Select black colour. 
	}
	
	for (i=0; i<PIXEL_MAX_LINE; i++){
		memcpy(address + (i * PIXEL_BYTE_NEXT_LINE_FACTOR), display_write, PIXEL_BYTE_NEXT_LINE_FACTOR);
		ioctl(fd, 0x4680, &rect);
	}
	
	close(fd);
}*/

/*
void test6(){
//Test writing entire display with less memory usage.
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 100;
	rect.dy = 0;
	rect.width = 10;//PIXEL_MAX_COLUMN;
	rect.height = 10;//PIXEL_MAX_LINE;
	
	int i = 0;
	for (i=0; i<PIXEL_BYTE_LAST_POSITION; i++){
		memcpy(address + i, COLOUR_WHITE, 1);
	}
	
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}*/

void clear_display(){
//sjekk fb_fillrect.
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = mmap(NULL, 153600, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	for (i=0; i<PIXEL_BYTE_LAST_POSITION; i++){
		memcpy(address + i, COLOUR_BLACK, 1);
	}
	
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}

/*
void test7(){
//Test of writing walls.
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = WALL_THICKNESS;
	
	int i = 0;
	for (i=0; i<PIXEL_BYTE_UPPER_WALL_END; i++){
		memcpy(address + i, COLOUR_WHITE, 1);
	}
	for (i=PIXEL_BYTE_LOWER_WALL_START; i<PIXEL_BYTE_LAST_POSITION; i++){
		memcpy(address + i, COLOUR_WHITE, 1);
	}
	ioctl(fd, 0x4680, &rect);
	
	rect.dx = 0;
	rect.dy = PIXEL_LOWER_WALL_START;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = WALL_THICKNESS;
	
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}*/

void draw_walls(){
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = 0;
	rect.dy = 0;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = WALL_THICKNESS;
	
	int i = 0;
	for (i=0; i<PIXEL_BYTE_UPPER_WALL_END; i++){
		memcpy(address + i, COLOUR_WHITE, 1);
	}
	for (i=PIXEL_BYTE_LOWER_WALL_START; i<PIXEL_BYTE_LAST_POSITION; i++){
		memcpy(address + i, COLOUR_WHITE, 1);
	}
	ioctl(fd, 0x4680, &rect);
	
	rect.dx = 0;
	rect.dy = PIXEL_LOWER_WALL_START;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = WALL_THICKNESS;
	
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}

#define PIXEL_RACKET_WIDTH		10
#define PIXEL_RACKET_HEIGHT		40 
#define PIXELS_BEHIND_RACKETS	20 
#define PIXEL_MEDIAN_HEIGHT		(PIXEL_BYTE_LAST_POSITION / (PIXEL_BYTE_NEXT_LINE_FACTOR * 2)) //119

#define PIXEL_BYTE_RACKET_WIDTH		(PIXEL_RACKET_WIDTH * PIXEL_BYTE_NEXT_COLUMN_OFFSET)
#define PIXEL_BYTE_RACKET_HEIGHT	(PIXEL_RACKET_HEIGHT * PIXEL_BYTE_NEXT_LINE_FACTOR)

#define PIXEL_RACKET_LEFT_X_START		PIXELS_BEHIND_RACKETS
#define PIXEL_RACKET_RIGHT_X_START		(PIXEL_MAX_COLUMN - PIXELS_BEHIND_RACKETS - PIXEL_RACKET_WIDTH)
#define PIXEL_BYTE_RACKET_LEFT_X_START	(PIXEL_RACKET_LEFT_X_START * PIXEL_BYTE_NEXT_COLUMN_OFFSET)
#define PIXEL_BYTE_RACKET_RIGHT_X_START	(PIXEL_RACKET_RIGHT_X_START * PIXEL_BYTE_NEXT_COLUMN_OFFSET)
#define PIXEL_BYTE_RACKET_LEFT_X_END	(PIXEL_BYTE_RACKET_LEFT_X_START + PIXEL_BYTE_RACKET_WIDTH)
#define PIXEL_BYTE_RACKET_RIGHT_X_END	(PIXEL_BYTE_RACKET_RIGHT_X_START + PIXEL_BYTE_RACKET_WIDTH)

#define PIXEL_RACKET_INITIAL_POSITION	(PIXEL_MEDIAN_HEIGHT - (PIXEL_RACKET_HEIGHT / 2))

#define PIXEL_RACKET_LEFT_CURRENT_POSITION	10//		PIXEL_RACKET_LEFT_INITIAL_POSITION
#define PIXEL_RACKET_RIGHT_CURRENT_POSITION			PIXEL_RACKET_RIGHT_INITIAL_POSITION
#define PIXEL_BYTE_RACKET_LEFT_CURRENT_POSITION		(PIXEL_RACKET_LEFT_CURRENT_POSITION * PIXEL_BYTE_NEXT_LINE_FACTOR)
#define PIXEL_BYTE_RACKET_RIGHT_CURRENT_POSITION	(PIXEL_RACKET_RIGHT_CURRENT_POSITION * PIXEL_BYTE_NEXT_LINE_FACTOR)
#define PIXEL_BYTE_RACKET_LEFT_CURRENT_END			(PIXEL_BYTE_RACKET_LEFT_CURRENT_POSITION + PIXEL_BYTE_RACKET_HEIGHT)

/*
void test8(){
//Test writing a racket.
	int fd = open("/dev/fb0", O_RDWR);
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	//rect.dx = PIXEL_RACKET_LEFT_X_START;
	//rect.dy = PIXEL_RACKET_LEFT_CURRENT_POSITION;
	//rect.width = PIXEL_RACKET_WIDTH;
	//rect.height = PIXEL_RACKET_HEIGHT;
	
	//int i = 0;
	//for (i=0; i<PIXEL_BYTE_LAST_POSITION; i++){
	//	memcpy(address + i, COLOUR_GREEN, 1);
	//}
	//ioctl(fd, 0x4680, &rect);
	
	rect.dx = 0;
	rect.dy = 0;
	rect.width = PIXEL_MAX_COLUMN;
	rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	
	//for (i=(PIXEL_RACKET_LEFT_CURRENT_POSITION * PIXEL_BYTE_NEXT_LINE_FACTOR) + PIXEL_BYTE_RACKET_LEFT_X_START; i<((PIXEL_RACKET_LEFT_CURRENT_POSITION + PIXEL_RACKET_HEIGHT) * PIXEL_BYTE_NEXT_LINE_FACTOR) + PIXEL_BYTE_RACKET_LEFT_X_START; i=i+640){
	////for (i=6418; i<38418; i=i+640){	
		//for (j=i; j<i + 20;//PIXEL_BYTE_RACKET_WIDTH; j++){
		
		
	for (i=PIXEL_BYTE_RACKET_LEFT_X_START; i<PIXEL_BYTE_RACKET_LEFT_X_END; i++){
		for (j=i+ PIXEL_BYTE_RACKET_LEFT_CURRENT_POSITION; j<PIXEL_BYTE_RACKET_LEFT_CURRENT_END; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_WHITE, 1);
		}
	}
	
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}*/


unsigned int previous_pixel_racket_left = 0;
unsigned int previous_pixel_racket_right = 0;

void draw_racket_left(unsigned int current_pixel){
	int fd = open("/dev/fb0", O_RDWR);
	
	double current_byte = current_pixel * PIXEL_BYTE_NEXT_LINE_FACTOR;
	previous_pixel_racket_left = current_pixel;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = PIXEL_RACKET_LEFT_X_START;
	rect.dy = current_pixel; 
	rect.width = PIXEL_RACKET_WIDTH;
	rect.height = PIXEL_RACKET_HEIGHT;
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=PIXEL_BYTE_RACKET_LEFT_X_START; i<PIXEL_BYTE_RACKET_LEFT_X_END; i++){
		for (j=i+ current_byte; j<current_byte + PIXEL_BYTE_RACKET_HEIGHT; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_WHITE, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}

void draw_racket_right(unsigned int current_pixel){
	int fd = open("/dev/fb0", O_RDWR);
	
	double current_byte = current_pixel * PIXEL_BYTE_NEXT_LINE_FACTOR;
	previous_pixel_racket_right = current_pixel;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = PIXEL_RACKET_RIGHT_X_START;
	rect.dy = current_pixel; 
	rect.width = PIXEL_RACKET_WIDTH;
	rect.height = PIXEL_RACKET_HEIGHT;
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=PIXEL_BYTE_RACKET_RIGHT_X_START; i<PIXEL_BYTE_RACKET_RIGHT_X_END; i++){
		for (j=i+ current_byte; j<current_byte + PIXEL_BYTE_RACKET_HEIGHT; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_WHITE, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}



void clear_racket_left(){
	int fd = open("/dev/fb0", O_RDWR);
	
	double previous_byte = previous_pixel_racket_left * PIXEL_BYTE_NEXT_LINE_FACTOR;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = PIXEL_RACKET_LEFT_X_START;
	rect.dy = previous_pixel_racket_left; 
	rect.width = PIXEL_RACKET_WIDTH;
	rect.height = PIXEL_RACKET_HEIGHT;
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=PIXEL_BYTE_RACKET_LEFT_X_START; i<PIXEL_BYTE_RACKET_LEFT_X_END; i++){
		for (j=i+ previous_byte; j<previous_byte + PIXEL_BYTE_RACKET_HEIGHT; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_BLACK, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}

void clear_racket_right(){
	int fd = open("/dev/fb0", O_RDWR);
	
	double previous_byte = previous_pixel_racket_right * PIXEL_BYTE_NEXT_LINE_FACTOR;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	rect.dx = PIXEL_RACKET_RIGHT_X_START;
	rect.dy = previous_pixel_racket_right; 
	rect.width = PIXEL_RACKET_WIDTH;
	rect.height = PIXEL_RACKET_HEIGHT;
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=PIXEL_BYTE_RACKET_RIGHT_X_START; i<PIXEL_BYTE_RACKET_RIGHT_X_END; i++){
		for (j=i+ previous_byte; j<previous_byte + PIXEL_BYTE_RACKET_HEIGHT; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_BLACK, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}

#define PIXEL_BALL_RADIUS			3
#define PIXEL_BALL_DIAMETER			(PIXEL_BALL_RADIUS * 2)
#define PIXEL_BYTE_BALL_DIAMETER	(PIXEL_BALL_DIAMETER * PIXEL_BYTE_NEXT_COLUMN_OFFSET)
#define PIXEL_BYTE_BALL_Y_END		(PIXEL_BALL_DIAMETER * PIXEL_BYTE_NEXT_LINE_FACTOR)
#define PIXEL_BALL_INITIAL_X		(PIXEL_MAX_COLUMN / 2)
#define PIXEL_BALL_INITIAL_Y		(PIXEL_MAX_LINE / 2)

unsigned int previous_pixel_ball_x = 0;
unsigned int previous_pixel_ball_y = 0;


void draw_ball(unsigned int current_pixel_x, unsigned int current_pixel_y){
	int fd = open("/dev/fb0", O_RDWR);
	
	current_pixel_x -= PIXEL_BALL_RADIUS;
	current_pixel_y -= PIXEL_BALL_RADIUS;
	
	double current_byte_x = current_pixel_x * PIXEL_BYTE_NEXT_COLUMN_OFFSET;
	double current_byte_y = current_pixel_y * PIXEL_BYTE_NEXT_LINE_FACTOR;
	previous_pixel_ball_x = current_pixel_x;
	previous_pixel_ball_y = current_pixel_y;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	
	rect.dx = current_pixel_x;
	rect.dy = current_pixel_y; 
	rect.width = PIXEL_BALL_DIAMETER;
	rect.height = PIXEL_BALL_DIAMETER;
	
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=current_byte_x; i<current_byte_x + PIXEL_BYTE_BALL_DIAMETER; i++){
		for (j=i+ current_byte_y; j<current_byte_y + PIXEL_BYTE_BALL_Y_END; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_WHITE, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}


void clear_ball(){
	int fd = open("/dev/fb0", O_RDWR);
	
	double previous_byte_x = previous_pixel_ball_x * PIXEL_BYTE_NEXT_COLUMN_OFFSET;
	double previous_byte_y = previous_pixel_ball_y * PIXEL_BYTE_NEXT_LINE_FACTOR;
	
	char * address = mmap(NULL, PIXEL_BYTE_LAST_POSITION, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	//If rect.width or rect.height are higher than pixels available, the buffer will not be updated. Equal to the pixel amount is fine, and should update the entire display. I think rect.dx and rect.dy are start positions, and width/height offset. 
	
	rect.dx = previous_pixel_ball_x;
	rect.dy = previous_pixel_ball_y; 
	rect.width = PIXEL_BALL_DIAMETER;
	rect.height = PIXEL_BALL_DIAMETER;
	
	//rect.dx = 0; rect.dy = 0; rect.width = PIXEL_MAX_COLUMN; rect.height = PIXEL_MAX_LINE;
	
	int i = 0;
	int j = 0;
	for (i=previous_byte_x; i<previous_byte_x + PIXEL_BYTE_BALL_DIAMETER; i++){
		for (j=i+ previous_byte_y; j<previous_byte_y + PIXEL_BYTE_BALL_Y_END; j=j+PIXEL_BYTE_NEXT_LINE_FACTOR){
			memcpy(address + j, COLOUR_BLACK, 1);
		}
	}
	ioctl(fd, 0x4680, &rect);
	
	close(fd);
}





void display_init(){
	//Remove linux penguin.
	clear_display();
	
	//Print upper and lower wall. 
	draw_walls();
	
	//Print rackets at their start position.
	draw_racket_left(PIXEL_RACKET_INITIAL_POSITION);
	draw_racket_right(PIXEL_RACKET_INITIAL_POSITION);
	
	//Print ball at start position. 
	draw_ball(PIXEL_BALL_INITIAL_X, PIXEL_BALL_INITIAL_Y);
}


int display()
{
	printf("Display driver started.\n");
	//We should open the file as short as possible (to allow several processes access). 

	//TODO:
	//Print racket at position Y.
	//Print ball at position.
	//Move racket (remove entire + redraw OR remove section + redraw section). 
	//Move ball.
	//Print score. 
	//Print winner (e.g. "Player 1 wins!").
	//Add border box around text, so it is easier to read if ball/racket overlaps. In practice: create empty rectangle before writing to an area. 
	//Colour? Cool animations? Animated rainbow colours with unicorns??
	//End TODO. 
	
	
	
	display_init();
	
	//test8();
	
	//clear_racket_right();
	//draw_racket_right(30);
	
	//clear_ball();
	//draw_ball(100,100);
	
	
	
	
	
	exit(EXIT_SUCCESS);
	
	
	return 0;
}

