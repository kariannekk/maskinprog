#define _GNU_SOURCE

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
#include <signal.h>
#include <stdint.h>
#include <poll.h>

/* Setup/init returns */
#define ERROR -1
#define SUCCESS 1

/* Game status */
#define PLAYING 1
#define QUIT 0
#define PAUSE 2

struct fb_copyarea rect;
int gamepadDevice;

int gameStatus;
struct sigaction act;
char buffer[4] = {'0', '1', '2', '3'};


void signalHandler(int signal, siginfo_t *info, void *ptr);

int setupGamepadDriver();

void ButtonHandler(uint8_t button);

int initGame();

void printIntructions();
