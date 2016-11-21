
/* Ball */
#define RIGHT 1
#define LEFT -1
#define UP -1
#define DOWN 1

/* Wall */
#define LEFT_RACKET_WALL	PIXEL_RACKET_LEFT_X_END
#define RIGHT_RACKET_WALL	(PIXEL_RACKET_RIGHT_X_START - PIXEL_BALL_DIAMETER)

#define BALL_UPPER_WALL		WALL_THICKNESS
#define BALL_LOWER_WALL 	(PIXEL_MAX_LINE - WALL_THICKNESS - PIXEL_BALL_DIAMETER)


void setupGame();

void moveRacket(int direction, int racket);

void startBall();

void randomBallDirection();

void newBallOffsetAngle();

void moveStep(int xStep, int yStep);

void moveBall();

void moveRightRacket(int direction);

void moveLeftRacket(int direction);

void scoreBoard();
