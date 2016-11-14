#include <stdio.h>

/*
#include "defines.h"
#include "driver-gamepad.c"
#include "efm32GG.h"
#include "game_to_display.c"
#include "game.c"
*/


#define MOVEMENT_DISTANCE_RAQUETS	10
#define PIXEL_BALL_INITIAL_X	100
#define PIXEL_BALL_INITIAL_Y	100
#define INITIAL_BALL_DIRECTION_X	3
#define INITIAL_BALL_DIRECTION_Y	3
#define UPPER_WALL	10
#define LOWER_WALL	200

#define PIXEL_MAX_LINE				240  //239
#define PIXEL_MAX_COLUMN			320  //319
#define PIXEL_AMOUNT				(PIXEL_MAX_LINE * PIXEL_MAX_COLUMN) //(240*320)  //152960  //153600

#define PIXEL_RACKET_WIDTH		10
#define PIXEL_RACKET_HEIGHT		40 
#define PIXELS_BEHIND_RACKETS	20 

#define PIXEL_RACKET_LEFT_X_START		PIXELS_BEHIND_RACKETS
#define PIXEL_RACKET_RIGHT_X_START		(PIXEL_MAX_COLUMN - PIXELS_BEHIND_RACKETS - PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_LEFT_X_END			(PIXEL_RACKET_LEFT_X_START + PIXEL_RACKET_WIDTH)
#define PIXEL_RACKET_RIGHT_X_END		(PIXEL_RACKET_RIGHT_X_START + PIXEL_RACKET_WIDTH)

#define PIXEL_ARRAY_RACKET_HEIGHT		(PIXEL_RACKET_HEIGHT * PIXEL_MAX_COLUMN)

#define PIXEL_RACKET_INITIAL_POSITION	((PIXEL_MAX_LINE / 2) - (PIXEL_RACKET_HEIGHT / 2))
#define BALL_STEP_LENGTH	

/*
#define LEFT_RAQUET_TOP_START = 10; //Må endre tall pga pixeloppsett på skjerm, bruke samme konstanter i alle filer
#define RIGHT_RAQUET_TOP_START = 10; //Må endre tall pga pixeloppsett på skjerm, bruke samme konstanter i alle filer
#define MOVEMENT_DISTANCE_RAQUETS = 12; //Må endre tall pga pixeloppsett på skjerm, bruke samme konstanter i alle filer
#define UPPER_BOUNDARY = 12; //Må endre tall pga pixeloppsett på skjerm, bruke samme konstanter i alle filer
#define LOWER_BOUNDARY = 150000; //Må endre tall pga pixeloppsett på skjerm, bruke samme konstanter i alle filer
#define ZONE_IMPACT_ON_BALL = 20 grader;
#define START_BALL_DIR_X	3
#define START_BALL_DIR_Y	3
*/

	/*
	skal kontrollere raquetene med knappetrykk
	knapp opp skal føre til ett stepp opp for valgt raquet
	knapp ned skal føre til ett stepp ned for valgt raquet
	må sørge for at raquets ikke kan gå utenfor toppvegg eller bunnvegg
	*/

int ball_direction_x = INITIAL_BALL_DIRECTION_X;
int ball_direction_y = INITIAL_BALL_DIRECTION_Y;
int x_position = PIXEL_BALL_INITIAL_X;
int y_position = PIXEL_BALL_INITIAL_Y;
int score_A = 0;
int score_B = 0;
int current_upper_pixel_position = PIXEL_RACKET_INITIAL_POSITION;
int current_lower_pixel_position = PIXEL_RACKET_INITIAL_POSITION + PIXEL_RACKET_HEIGHT;
int piksel2 = PIXEL_RACKET_INITIAL_POSITION;

void move_racket_left_up(int pixel_racket_initial_position);
void move_racket_left_down(int pixel_racket_initial_position);
void move_racket_right_up(int pixel_racket_initial_position);
void move_racket_right_down(int pixel_racket_initial_position);
void move_ball(int x_position, int y_position);


int main(void) {
	move_racket_left_up(piksel2);
	move_racket_left_down(piksel2);
	move_racket_right_up(piksel2);
	move_racket_right_down(piksel2);
	int x = PIXEL_BALL_INITIAL_X;
	int y = PIXEL_BALL_INITIAL_Y;
//for(int t=0;t<100;t++) {
	move_ball(x, y);
//}
	return 0;
}

void move_racket_left_up(int pixel_racket_initial_position) {
	int current_upper_pixel_position = pixel_racket_initial_position;
	int current_lower_pixel_position = pixel_racket_initial_position + PIXEL_RACKET_HEIGHT;

	if(current_upper_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = current_upper_pixel_position;
		current_lower_pixel_position = current_lower_pixel_position;
		printf("Racket can not move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}
	else {
		current_upper_pixel_position -= MOVEMENT_DISTANCE_RAQUETS;
		current_lower_pixel_position -= MOVEMENT_DISTANCE_RAQUETS;
		printf("Racket can move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}

}

void move_racket_left_down(int pixel_racket_initial_position) {
	int current_upper_pixel_position = pixel_racket_initial_position;
	int current_lower_pixel_position = pixel_racket_initial_position + PIXEL_RACKET_HEIGHT;

	if(current_lower_pixel_position == LOWER_WALL) {
		current_upper_pixel_position = current_upper_pixel_position;
		current_lower_pixel_position = current_lower_pixel_position;
		printf("Racket can not move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
		current_lower_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
		printf("Racket can move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}

}

void move_racket_right_up(int pixel_racket_initial_position) {
	int current_upper_pixel_position = pixel_racket_initial_position;
	int current_lower_pixel_position = pixel_racket_initial_position + PIXEL_RACKET_HEIGHT;

	if(current_upper_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = current_upper_pixel_position;
		current_lower_pixel_position = current_lower_pixel_position;
		printf("Racket can not move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}
	else {
		current_upper_pixel_position -= MOVEMENT_DISTANCE_RAQUETS;
		current_lower_pixel_position -= MOVEMENT_DISTANCE_RAQUETS;
		printf("Racket can move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}

}

void move_racket_right_down(int pixel_racket_initial_position) {
	int current_upper_pixel_position = pixel_racket_initial_position;
	int current_lower_pixel_position = pixel_racket_initial_position + PIXEL_RACKET_HEIGHT;

	if(current_lower_pixel_position == LOWER_WALL) {
		current_upper_pixel_position = current_upper_pixel_position;
		current_lower_pixel_position = current_lower_pixel_position;
		printf("Racket can not move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
		current_lower_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
		printf("Racket can move, current_upper_pos = %d, current_lower_pos = %d, UPPER_WALL = %d\n", current_upper_pixel_position, current_lower_pixel_position, UPPER_WALL);
	}

}

void move_ball(int x_position, int y_position) {
for(int t=0;t<150;t++) {
	if(x_position <= UPPER_WALL) {
		//ball_direction_y = ball_direction_y - 2*ball_direction_y;
		ball_direction_y = ~ball_direction_y;
	}
	if(x_position >= LOWER_WALL) {
		//ball_direction_y = ball_direction_y - 2*ball_direction_y;
		ball_direction_y = ~ball_direction_y;
	}
	if(x_position == PIXEL_RACKET_LEFT_X_END) {
		ball_direction_x = ~ball_direction_x;
		if(y_position >= current_upper_pixel_position || y_position <= current_lower_pixel_position) {
			score_B ++;
			printf("score_B: %d\n\n", score_B);
			ball_direction_x = INITIAL_BALL_DIRECTION_X;
			ball_direction_y = INITIAL_BALL_DIRECTION_Y;
			x_position = PIXEL_BALL_INITIAL_X;
			y_position = PIXEL_BALL_INITIAL_Y;
		}
	}
	else if(x_position == PIXEL_RACKET_RIGHT_X_START) {
		ball_direction_x = ~ball_direction_x;
		if(y_position >= current_upper_pixel_position || y_position <= current_lower_pixel_position) {
			score_A ++;
			printf("score_A: %d\n\n", score_A);
			ball_direction_x = INITIAL_BALL_DIRECTION_X;
			ball_direction_y = INITIAL_BALL_DIRECTION_Y;
			x_position = PIXEL_BALL_INITIAL_X;
			y_position = PIXEL_BALL_INITIAL_Y;
		}
	}

	x_position = x_position + ball_direction_x;
	y_position = y_position + ball_direction_y;

	printf("x_position: %d, y_position: %d\n\n", x_position, y_position);
}
}

/*
void move_racket_left_up(PIXEL_RACKET_INITIAL_POSITION) {
	//Tegn opp øverst som initial position
	//Draw racket at 3201 + 
	*/
	/*
	if(up button)
		current pixel = initial + racket step
	if(upper pos == upper_wall || lower pos == lower wall)
		draw racket left
	else if(up button) {
		upper pos == upper pos - racket_step
	}
	else if(down button) {
		lower pos == lower pos + racket_step
	}
	*/

/*
	if(current_upper_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = MOVEMENT_DISTANCE_RAQUETS;
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
	}

	
}

void move_racket_left_down(PIXEL_RACKET_INITIAL_POSITION) {
	if(current_lower_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = MOVEMENT_DISTANCE_RAQUETS;
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
	}
}

void move_racket_right_up(PIXEL_RACKET_INITIAL_POSITION) {
	if(current_upper_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = MOVEMENT_DISTANCE_RAQUETS;
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
	}
}

void move_racket_right_down(PIXEL_RACKET_INITIAL_POSITION) {
	if(current_upper_pixel_position == UPPER_WALL) {
		current_upper_pixel_position = MOVEMENT_DISTANCE_RAQUETS;
	}
	else {
		current_upper_pixel_position += MOVEMENT_DISTANCE_RAQUETS;
	}
}



void move_racket_right(PIXEL_RACKET_INITIAL_POSITION) {

}
*/




/*
void check_space_and_move_left_raquet(unsigned int button_direction) {
	if(MOVEMENT_DISTANCE_RAQUETS >= UPPER_BOUNDARY + MOVEMENT_DISTANCE_RAQUETS) {
		if(button_direction == UP) {
			uint32_t new_raquet_pixel = current_pixel + MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(new_raquet_pixel)
		}
	}
	if(MOVEMENT_DISTANCE_RAQUETS >= LOWER_BOUNDARY - MOVEMENT_DISTANCE_RAQUETS) {
		if(button_direction == DOWN) {
			uint32_t new_raquet_pixel = current_pixel - MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(new_raquet_pixel)
		}
	}
}

void check_space_and_move_right_raquet(unsigned int button_direction) {
	if(MOVEMENT_DISTANCE_RAQUETS >= UPPER_BOUNDARY + MOVEMENT_DISTANCE_RAQUETS) {
		if(button_direction == UP) {
			uint32_t new_raquet_pixel = current_pixel + MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(new_raquet_pixel)
		}
	}
	if(MOVEMENT_DISTANCE_RAQUETS >= LOWER_BOUNDARY - MOVEMENT_DISTANCE_RAQUETS) {
		if(button_direction == DOWN) {
			uint32_t new_raquet_pixel = current_pixel - MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(new_raquet_pixel)
		}
	}
}
*/

/*
void move_ball(unsigned int current_ball_pixel_x, unsigned int current_ball_pixel_y) {
//	ball drawn

//	check for hitting wall on next xy coordinates
//	check for hitting raquets
//	if hitting walls or raquets, x dir = same, y dir = inverted

//	call (move ball) every tick of timer aka refresh ball in new position every timer tick
	//uint32_t current_ball_dir_x = START_BALL_DIR_X;
	//uint32_t current_ball_dir_y = START_BALL_DIR_Y;
	uint32_t next_x_pixel = current_pixel_x + current_ball_dir_x;
	uint32_t next_y_pixel = current_pixel_y + current_ball_dir_y;
	if(current_ball_pixel_y >= UPPER_BOUNDARY_Y) {
		if(current_ball_pixel_y == UPPER_BOUNDARY_Y) {
			next_y_pixel = ~next_y_pixel;
		}

		draw_ball(next_x_pixel, next_y_pixel);
	}

	if(current_ball_pixel_x < RACKET_BOUNDARY) {
		score_A
	}

}
*/
/*
bool check_ball_passed_raquet(unsigned int current_ball_pixel_x, unsigned int current_ball_pixel_y) {
	if(current_ball_pixel_x < ) {
	return true;
	}
	else 
		return false;
}
*/
/*
	bool space_above = false;
	switch(button_value) {
	case up_button :
		check space available
		if(MOVEMENT_DISTANCE_RAQUETS >= UPPER_BOUNDARY + MOVEMENT_DISTANCE_RAQUETS)
			space_above = true;
		break;
		return space_above;
	}
*/
/*
void left_raquet_move(int new_raquet_position) {
	//if tests
	//case


	clear_racket_left(); //Delete current raquet // kanskje ikke nødvendig
	int left_raquet_top = LEFT_RAQUET_TOP_START;
	if(new_raquet_position >= UPPER_BOUNDARY + MOVEMENT_DISTANCE_RAQUETS) { //Prevent raquet moving out of bounds, also when not enough room to move raquet standard movement distance
		if(new_raquet_position < left_raquet_top) {
			left_raquet_top = left_raquet_top - MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(left_raquet_top);
		}
		else
			left_raquet_top = left_raquet_top; //Prevent program crashes //break
	}

	else
		left_raquet_top = left_raquet_top; //Prevent program crashes //break


	else if(new_raquet_position <= LOWER_BOUNDARY - MOVEMENT_DISTANCE_RAQUETS) { //Prevent raquet moving out of bounds, also when not enough room to move raquet standard movement distance
		if(new_raquet_position > left_raquet_top) {
			left_raquet_top = left_raquet_top + MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_left(left_raquet_top);
		}
		else
			left_raquet_top = left_raquet_top; //Prevent program crashes //break

	}
	else
		left_raquet_top = left_raquet_top; //Prevent program crashes //break

}
*/

/*
bool move_up()
{
    bool r = false;
    for (int i = 4; i < 16; i++) {
        int j = i;
        while (j >= 4) {
            if (b[j] != 0 && b[j-4] == 0) {
                b[j-4] = b[j];
                b[j] = 0;
                j -= 4;
                r = true;
            } else break;
        }
    }
    return r;
}


up = +1
down = -1

move_left(dir)
previous = previous + dir
draw_rack_left(previous)


bool move_down()
{
    bool r = false;
    for (int i = 11; i >= 0; i--) {
        int j = i;
        while (j <= 11) {
            if (b[j] != 0 && b[j+4] == 0) {
                b[j+4] = b[j];
                b[j] = 0;
                j += 4;
                r = true;
            } else break;
        }
    }
    return r;
}
*/

/*
void move_left(dir) {
	previous = previous + dir;
	draw_rack_left(previous);
}
*/


/*
void right_raquet_move(int new_raquet_position) {
	//if tests
	//case
	clear_racket_right(); //Delete current raquet
	int right_raquet_top = RIGHT_RAQUET_TOP_START;
	if(new_raquet_position >= UPPER_BOUNDARY + MOVEMENT_DISTANCE_RAQUETS) { //Prevent raquet moving out of bounds, also when not enough room to move raquet standard movement distance
		if(new_raquet_position < right_raquet_top) {
			right_raquet_top = right_raquet_top - MOVEMENT_DISTANCE_RAQUETS;
			draw_racket_right(right_raquet_top);
		}
	}
}
*/



//////ZONE_VALUES//////
/*
Zone A = 1 = inverse direction + up_tilt
Zone B = 2 = inverse direction
Zone C = 3 = inverse direction + down_tilt
*/

/*
int adjust_ball_dir(int current_ball_direction, int current_tilt) {
	//while()
	int zone = decide_zone_for_raquets(ball_edge, which_raquet);
	switch(zone) {
	case 1 :
		current_tilt = ~current_tilt - ZONE_IMPACT_ON_BALL;
		current_ball_direction = ~current_ball_direction - current_tilt; //Antagelig feil, bare pseudo-tanke
		break;
	case 2 :
		current_ball_direction
	case 3 :
		printf("Button pushed: SW3\n");
		// Perform action
		break;
	case 0 :
		printf("Button pushed: SW4\n");
		// Perform action
		break;
	default:
		break;
	}
	return current_ball_direction;
}
*/

/*
int decide_zone_for_raquets(int ball_edge, int which_raquet) {//Må inn i raquetmovement //char-funksjon?
	int zone_raquet_L = 0;
	int zone_raquet_R = 0;

	A_zone_raquet_L = raquet_y  to  raquet_y+3
	B_zone_raquet_L = raquet_y+4  to  raquet_y+7
	C_zone_raquet_L = raquet_y+8  to  raquet_y+11

	if(which_raquet == 1) { //Left raquet has the ball
		if(ball_edge == A_zone_raquet_L) //Zone A hit
			zone_raquet_L = 1;
		else if(ball_edge == B_zone_raquet_L)
			zone_raquet_L = 2;
		else if(ball_edge == C_zone_raquet_L)
			zone_raquet_L = 3;
		else
			return zone_raquet_L;

		return zone_raquet_L;
	}
	else if(which_raquet == 2) { //Right raquet has the ball
		if(ball_edge == A_zone_raquet_R)
			return 1;
		else if(ball_edge == B_zone_raquet_R)
			return 2;
		else if(ball_edge == C_zone_raquet_R)
			return 3;
		else
			return 0;
	}
	else
		return 0;
}
*/

//fullføre
//ball
//score
//sluttfunksjon
