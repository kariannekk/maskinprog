/* Setup/init returns */
#define ERROR -1
#define SUCCESS 1

/* State machine */
#define PLAYING 1
#define STOPP 0
#define PAUSE 2

/* Direction */
#define UP 1
#define DOWN -1

/* Racket */
#define UPPER_POSITION 9
#define LOWER_POSITION 0
#define RIGHT 1
#define LEFT 0
#define HIGHT 0

/* Ball */
#define 60DEG 4
#define 45DEG 3
#define 30DEG 2
#define 0DEG 1
#define BALL_UP 1
#define BALL_DOWN -1
#define BALL_INIT 0
#define BALL_RIGHT 1
#define BALL_LEFT 0
#define BALL_STEP1 5
#define BALL_STEP2 4

#define PIXEL_RACKET_WIDTH		10
#define PIXEL_RACKET_HEIGHT		40 
#define PIXELS_BEHIND_RACKETS	20 

#define PIXEL_RACKET_LEFT_X_START		PIXELS_BEHIND_RACKETS
#define PIXEL_RACKET_RIGHT_X_START		(PIXEL_MAX_COLUMN - PIXELS_BEHIND_RACKETS - PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_LEFT_X_END			(PIXEL_RACKET_LEFT_X_START + PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_RIGHT_X_END		(PIXEL_RACKET_RIGHT_X_START + PIXEL_RACKET_WIDTH)

#define PIXEL_BALL_RADIUS			3

#define PIXEL_BALL_DIAMETER			(PIXEL_BALL_RADIUS * 2)
#define PIXEL_BALL_Y_END			(PIXEL_BALL_DIAMETER * PIXEL_MAX_COLUMN)

#define PIXEL_BALL_INITIAL_X		(PIXEL_MAX_COLUMN / 2)
#define PIXEL_BALL_INITIAL_Y		(PIXEL_MAX_LINE / 2)

/* 320x240 display, 0-indexed. */
#define PIXEL_MAX_LINE				240  //239
#define PIXEL_MAX_COLUMN			320  //319
#define PIXEL_AMOUNT				(PIXEL_MAX_LINE * PIXEL_MAX_COLUMN) //(240*320)  //152960  //153600

/* Wall setup. */
#define WALL_THICKNESS		10	//In pixel lines.

/* Formulas; to reduce time spent calculating. */
#define PIXEL_UPPER_WALL_END	(WALL_THICKNESS * PIXEL_MAX_COLUMN) //Start at 0.
#define PIXEL_LOWER_WALL_START	(PIXEL_AMOUNT - (WALL_THICKNESS * PIXEL_MAX_COLUMN)) //End at last pixel.

/* Board */
#define UPPER_WALL PIXEL_UPPER_WALL_END
#define LOWER_WALL PIXEL_LOWER_WALL_START
#define LEFT_RACKET_WALL PIXEL_RACKET_LEFT_X_END
#define RIGHT_RACKET_WALL PIXEL_RACKET_RIGHT_X_START

struct fb_copyarea rect;
int gamepadDevice;

int gameStatus;
struct sigaction act;
char buffer[4] = {'0', '1', '2', '3'};

int currentLeftPosition;
int currentRightPosition;
int previous_pixel_racket_left;
int previous_pixel_racket_right;

struct ballInfo{
	int xCoord;
	int yCoord;
	int speed;
	int xDir;
	int yDir;
} ball;


void signalHandler(int signal, siginfo_t *info, void *ptr);

int setupGamepadDriver();

void ButtonHandler(uint8_t button);

void setupGame();

int initGame();

void moveRightRacket(int direction);

void moveRacket(int direction, int racket);

void startBall();

void randomBallDirection();

void newBallSpeed();

void moveStep(int yStep, int xStep);

void moveBall();

void printBall()

void printStatusRacket();