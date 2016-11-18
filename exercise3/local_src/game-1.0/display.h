
/* 320x240 display, 0-indexed. */
#define PIXEL_MAX_LINE				240  //239
#define PIXEL_MAX_COLUMN			320  //319

/* Wall setup. */
#define WALL_THICKNESS		10	//In pixel lines.

/* Racket setup */
#define PIXEL_RACKET_WIDTH		10
#define PIXEL_RACKET_HEIGHT		40 
#define PIXELS_BEHIND_RACKETS	20 

#define PIXEL_RACKET_LEFT_X_START		PIXELS_BEHIND_RACKETS
#define PIXEL_RACKET_RIGHT_X_START		(PIXEL_MAX_COLUMN - PIXELS_BEHIND_RACKETS - PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_LEFT_X_END			(PIXEL_RACKET_LEFT_X_START + PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_RIGHT_X_END		(PIXEL_RACKET_RIGHT_X_START + PIXEL_RACKET_WIDTH)

#define RACKET_STEP_AMOUNT		10
#define RACKET_STEP_LENGTH		((PIXEL_MAX_LINE - (2 * WALL_THICKNESS) - (PIXEL_RACKET_HEIGHT / 2)) / RACKET_STEP_AMOUNT)

#define RACKET_INITIAL_POSITION 5
#define PIXEL_RACKET_INITIAL_POSITION	(RACKET_INITIAL_POSITION * RACKET_STEP_LENGTH) + WALL_THICKNESS

/* Ball */
#define PIXEL_BALL_DIAMETER			6
#define PIXEL_BALL_INITIAL_X		(PIXEL_MAX_COLUMN / 2)
#define PIXEL_BALL_INITIAL_Y		(PIXEL_MAX_LINE / 2)


/******************/
/* User interface */
/******************/

void refresh_display();

void display_init();

void move_left_racket(unsigned int position);

void move_right_racket(unsigned int position);

void move_ball(unsigned int pixel_x, unsigned int pixel_y);

void game_over();

void restart();

unsigned int pixelRacetLeft();

unsigned int pixelRacetRight();

/**************************/
/* Only used by display.c */
/**************************/
void open_display();

void close_display();

void refresh_rectangle(int startX, int startY, int lengthX, int lengthY);

void clear_display();

void draw_walls();

void clear_racket_left();

void clear_racket_right();

void draw_racket_left(unsigned int current_pixel);

void draw_racket_right(unsigned int current_pixel);

void clear_ball();

void draw_ball(unsigned int current_pixel_x, unsigned int current_pixel_y);







