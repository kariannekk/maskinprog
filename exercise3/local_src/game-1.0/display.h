
/* 320x240 display. */
#define PIXEL_MAX_LINE					240
#define PIXEL_MAX_COLUMN				320

/* Wall setup. */
#define WALL_THICKNESS					10	//In pixels.

/* Racket setup */
#define PIXEL_RACKET_WIDTH				10
#define PIXEL_RACKET_HEIGHT				40 
#define PIXELS_BEHIND_RACKETS			20 

#define PIXEL_RACKET_LEFT_X_START		PIXELS_BEHIND_RACKETS
#define PIXEL_RACKET_RIGHT_X_START		(PIXEL_MAX_COLUMN - PIXELS_BEHIND_RACKETS - PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_LEFT_X_END			(PIXEL_RACKET_LEFT_X_START + PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_RIGHT_X_END		(PIXEL_RACKET_RIGHT_X_START + PIXEL_RACKET_WIDTH)

#define RACKET_STEP_AMOUNT				10
#define RACKET_STEP_LENGTH				((PIXEL_MAX_LINE - (2 * WALL_THICKNESS) - (PIXEL_RACKET_HEIGHT / 2)) / RACKET_STEP_AMOUNT)

#define RACKET_INITIAL_POSITION			(RACKET_STEP_AMOUNT / 2)
#define PIXEL_RACKET_INITIAL_POSITION	(RACKET_INITIAL_POSITION * RACKET_STEP_LENGTH) + WALL_THICKNESS

/* Ball */
#define PIXEL_BALL_DIAMETER				6
#define PIXEL_BALL_INITIAL_X			(PIXEL_MAX_COLUMN / 2)
#define PIXEL_BALL_INITIAL_Y			(PIXEL_MAX_LINE / 2)


/******************/
/* User interface */
/******************/

void refreshDisplay();

void displayInit();

//Takes upper side of the racket as input.
void displayMoveLeftRacket(unsigned int position);

//Takes upper side of the racket as input.
void displayMoveRightRacket(unsigned int position);

//Takes upper left corner of the ball as input.
void displayMoveBall(unsigned int pixel_x, unsigned int pixel_y);

void displayGameOver();

unsigned int getPixelRacketLeft();

unsigned int getPixelRacketRight();

/**************************/
/* Only used by display.c */
/**************************/
void openDisplay();

void closeDisplay();

void refreshRectangle(int startX, int startY, int lengthX, int lengthY);

void clearDisplay();

void drawWalls();

void clearRacketLeft();

void clearRacketRight();

//Takes upper side of the racket as input.
void drawRacketLeft(unsigned int current_pixel);

//Takes upper side of the racket as input.
void drawRacketRight(unsigned int current_pixel);

void clearBall();

//Takes upper left corner of the ball as input.
void drawBall(unsigned int current_pixel_x, unsigned int current_pixel_y);







