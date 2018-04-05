// By izanbf1803		-		http://izanbf.es/

#ifndef SCREEN_H
#define SCREEN_H


#include <SDL2/SDL.h>

typedef struct {
	unsigned char** pixels;			// Pixels dynamic array of arrays (Ex: pixels[500][500])
	unsigned char** pixels_next;	// A copy of pixels for update() blocks
	int W, H;						// With, Height
	SDL_DisplayMode info;			// Screen info: Freq, width, height
	int delay;						// SDL Delay
	int sp;							// Block spawn probability (x / 100) 
	int point_size;					// [NOT IMPLEMENTED] In pixels
} screen_t;


unsigned int rand_range(const unsigned int min, const unsigned int max);
screen_t* init_game(int argc, char** argv);
void end_game(screen_t* screen);


#endif