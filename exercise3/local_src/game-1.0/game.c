#include "game.h"
#include "movement.h"
#include "display.h"


//write(gamepadDevice, &buffer[0], 1); --Stop timer3
//write(gamepadDevice, &buffer[1], 1); --Start timer3
//write(gamepadDevice, &buffer[2], 1); --Start kernel timer (ball timer)
//write(gamepadDevice, &buffer[3], 1); --Stop kernel timer


void printIntructions(){
	printf("**************************************\n");
	printf("Welcome to the Ping Pong Game!\n");
	printf("**************************************\n\n");

	printf("--------------------------------------\n");
	printf("Instructions!\n");
	printf("--------------------------------------\n");
	printf("Player 1:\n");
	printf("			Move racket up: SW2\n");
	printf("			Move raceket down: SW4\n");
	printf("--------------------------------------\n");
	printf("Player 2:\n");
	printf("			Move racket up: SW6\n");
	printf("			Move raceket down: SW8\n");	
	printf("--------------------------------------\n");
	printf("Pause game: SW5\n");
	printf("--------------------------------------\n");
	printf("Continue game: SW1\n");
	printf("--------------------------------------\n");
	printf("Stop/Quit game: SW3\n");
	printf("--------------------------------------\n");
	printf("PS: when game is paused, only\n");
	printf("continue(SW1) and stop(SW3) is enabled")
	printf("--------------------------------------\n");
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
		printf("Could not set the signal with info, F_SETSIG!\n");
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
		//printf("Button pushed: SW1\n");
		// Perform action
		write(gamepadDevice, &buffer[1], 1); // Continue game
		break;
	case 0x0D :
		//printf("Button pushed: SW2\n");
		moveRacket(UP, LEFT);
		break;
	case 0x0B :
		//printf("Button pushed: SW3\n");
		// Perform action
		gameStatus = STOP; //Quit game
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
		//printf("Button pushed: SW5\n");
		// Perform action
		write(gamepadDevice, &buffer[0], 1); // Pause game
		break;
	case 0xD0 :
		//printf("Button pushed: SW6\n");
		moveRacket(UP, RIGHT);
		break;
	case 0xB0 :
		//printf("Button pushed: SW7\n");
		// Perform action
		write(gamepadDevice, &buffer[3], 1); //--Stop kernel timer
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
	uint8_t button;
	printf("Code %d\n", info->si_code);
	//printf("POLL_IN %d\n", POLL_IN);
	switch (info->si_code){
		case POLL_IN: //GPIO interrupt
			read(gamepadDevice, &button, 1);
			ButtonHandler(button);
			break;
		case POLL_OUT: //Timer3 interrupt
			// Play sound sample
			break;
		case POLL_MSG: //Kernel timer interrupt
			// Update ball
			moveBall();
			break;
		default:
			return;
	}
}



int initGame()
{
	/* Set up game features */
	setupGame();
	
	/* Connect to framework - print board */
	display_init();

	/* Connect to gamepad driver - get access to hardware */
	if(setupGamepadDriver() == ERROR){
		return ERROR;
	}
	
	gameStatus = PLAYING;
	
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	printIntructions()
	
	
	if (initGame() == ERROR){
		return 0;
	}

	write(gamepadDevice, &buffer[2], 1); //--Start kernel timer (ball timer)


	while(gameStatus){
		pause(); //Sleep and wait for the next SIGIO
	}
	
	game_over();
	close(gamepadDevice);
	
	return 0;
}


