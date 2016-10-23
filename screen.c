// By izanbf1803		-		http://izanbf.es/

#define POINT_SIZE 4 // [NOT IMPLEMENTED]
#define DEF_W screen->info.w;
#define DEF_H screen->info.h;
#define DEF_DELAY 10
#define DEF_SPAWN_PROBABILITY 4

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "screen.h"

unsigned int rand_range(const unsigned int min, const unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;
    do
    {
        r = rand();
    } while (r >= limit);
    return min + (r / buckets);
}

screen_t* init_game(int argc, char** argv)
{
	srand(time(NULL));

	screen_t* screen = (screen_t*) malloc(sizeof(screen_t));	// Allocate screen as pointer
	SDL_GetCurrentDisplayMode(0, &screen->info);				// Get screen info
	screen->point_size  = POINT_SIZE; // [NOT IMPLEMENTED]
	screen->W			= DEF_W;
	screen->H			= DEF_H;
	screen->delay		= DEF_DELAY;
	screen->sp			= DEF_SPAWN_PROBABILITY;

	if (argc >= 2)
		screen->W = atoi(argv[1]);
	if (argc >= 3)
		screen->H = atoi(argv[2]);
	if (argc >= 4)
		screen->delay = atoi(argv[3]);
	if (argc >= 5)
		screen->sp = atoi(argv[4]);

	screen->pixels      = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));
	screen->pixels_next = (unsigned char**) malloc((screen->W) * sizeof(unsigned char*));

	int x, y;
	for (x = 0; x < screen->W; x++) {
		screen->pixels[x]	   = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		screen->pixels_next[x] = (unsigned char*) malloc((screen->H) * sizeof(unsigned char));
		for (y = 0; y < screen->H; y++) {
			screen->pixels[x][y] = (rand_range(1, 100) <= screen->sp ? 1 : 0);
			screen->pixels_next[x][y] = screen->pixels[x][y];
		}
	}

	return screen;
}

void end_game(screen_t* screen)
{
	int i;
	for (i = 0; i < screen->H; i++) {
		free(screen->pixels[i]);
		free(screen->pixels_next[i]);
	}
	free(screen->pixels);
	free(screen->pixels_next);
	free(screen);
}