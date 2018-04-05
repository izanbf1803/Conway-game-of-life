// By izanbf1803		-		http://izanbf.es/

#define POINT_SIZE 4 // [NOT IMPLEMENTED]
#define DEF_W screen->info.w;
#define DEF_H screen->info.h;
#define DEF_DELAY 10
#define DEF_SPAWN_PROBABILITY 10

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "screen.h"

unsigned int g_seed;

unsigned int fastrand() { 
	g_seed = 214013 * g_seed + 2531011; 
	return (g_seed >> 16) & 0x7FFF; 
} 

unsigned int fast_mod(const unsigned int n, const unsigned int m)
{
	return (n < m ? n : n % m);
}

unsigned int rand_range(const unsigned int min, const unsigned int max)
{
    unsigned int r = fastrand();
    return fast_mod(r, max - min + 1) + min;
}

screen_t* init_game(int argc, char** argv)
{
	srand(time(NULL));
	g_seed = rand();

	screen_t* screen = (screen_t*) malloc(sizeof(screen_t));	// Allocate screen as pointer
	SDL_GetCurrentDisplayMode(0, &screen->info);				// Get screen info
	screen->point_size  = POINT_SIZE; // [NOT IMPLEMENTED]
	screen->W			= DEF_W;
	screen->H			= DEF_H;
	screen->delay		= DEF_DELAY;
	screen->sp			= DEF_SPAWN_PROBABILITY;

	if (argc >= 2 && argv[1][0] != '.') screen->W = atoi(argv[1]);
	if (argc >= 3 && argv[2][0] != '.') screen->H = atoi(argv[2]);
	if (argc >= 4 && argv[3][0] != '.') screen->delay = atoi(argv[3]);
	if (argc >= 5 && argv[4][0] != '.') screen->sp = atoi(argv[4]);

	screen->pixels      = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	screen->pixels_next = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));

	for (int x = 0; x < screen->W; x++) {
		screen->pixels[x]	   = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		screen->pixels_next[x] = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		for (int y = 0; y < screen->H; y++) {
			screen->pixels[x][y] = (rand_range(1, 100) <= screen->sp ? 1 : 0);
			screen->pixels_next[x][y] = screen->pixels[x][y];
		}
	}

	return screen;
}

void end_game(screen_t* screen)
{
	for (int i = 0; i < screen->H; i++) {
		free(screen->pixels[i]);
		free(screen->pixels_next[i]);
	}
	free(screen->pixels);
	free(screen->pixels_next);
	free(screen);
}