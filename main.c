// By izanbf1803		-		http://izanbf.es/

#define TITLE "Conway's game of life   -   izanbf.es"

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "screen.h"


void update(screen_t* screen)
{
	#pragma omp parallel for
	for (int x = 0; x < screen->W; x++) {
		for (int y = 0; y < screen->H; y++) {
			int n = 0;	// Number of neighbours
			int to_iterate[8][2] = {
				{ x-1 , y+1 },
				{ x   , y+1 },
				{ x+1 , y+1 },
				{ x-1 , y   },
				{ x+1 , y   },
				{ x-1 , y-1 },
				{ x   , y-1 },
				{ x+1 , y-1 }
			};
			for (int i = 0; i < 8; i++) {
				if (to_iterate[i][0] >= 0 && to_iterate[i][0] < screen->W
					&& to_iterate[i][1] >= 0 && to_iterate[i][1] < screen->H
					&& screen->pixels[to_iterate[i][0]][to_iterate[i][1]] == 1)
				{
					n++;
				}
			}
			if (screen->pixels[x][y]) {
				screen->pixels_next[x][y] = (n == 2) || (n == 3);
			}
			else {
				screen->pixels_next[x][y] = (n == 3);
			}
		}
	}
	#pragma omp parallel for
	for (int x = 0; x < screen->W; x++)	{	// Copy temp data to real pointer (used on draw())
		memcpy(screen->pixels[x], screen->pixels_next[x], screen->H * sizeof(unsigned char));
	}
}

void draw(screen_t* screen, SDL_Renderer* renderer)
{
	for (int x = 0; x < screen->W; x++) {
		for (int y = 0; y < screen->H; y++) {
			if (screen->pixels[x][y]) {
				SDL_Rect rect;
				rect.x = x * screen->point_size;
				rect.y = y * screen->point_size;
				rect.w = screen->point_size;
				rect.h = screen->point_size;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

int main(int argc, char** argv)		// Inizialize all values
{
	SDL_Init(SDL_INIT_VIDEO);

	screen_t* screen = init_game(argc, argv);

	SDL_Window* win = SDL_CreateWindow(TITLE, (screen->info.w >> 1)-(screen->W >> 1), (screen->info.h >> 1)-(screen->H >> 1),
										screen->W * screen->point_size, screen->H * screen->point_size, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event ev;

	if (argc < 3 || (argv[1][0] == '.' && argv[2][0] == '.'))
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

	unsigned char quit = 0, pause = 0, mouseDownLeft = 0, mouseDownRight = 0;
	while (!quit) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym) {
						case SDLK_ESCAPE:
						case SDLK_q:
							quit = 1;
							break;
						case SDLK_p:
							pause = !pause;
							break;
						case SDLK_DOWN:
							screen->delay <<= 1;
							if (screen->delay > 500) screen->delay = 500;
							break;
						case SDLK_UP:
							screen->delay >>= 1;
							if (screen->delay < 1) screen->delay = 1;
							break;
					}
					break;						
				case SDL_MOUSEBUTTONDOWN:
					if (ev.button.button == SDL_BUTTON_LEFT) mouseDownLeft = 1;
					if (ev.button.button == SDL_BUTTON_RIGHT) mouseDownRight = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (ev.button.button == SDL_BUTTON_LEFT) mouseDownLeft = 0;
					if (ev.button.button == SDL_BUTTON_RIGHT) mouseDownRight = 0;
					break;
				case SDL_MOUSEMOTION: {
					int x_ = ev.button.x / screen->point_size;
					int y_ = ev.button.y / screen->point_size;
					if (x_ < screen->W && y_ < screen->H) {
						if (mouseDownLeft) {
							screen->pixels[x_][y_] = 1;
						}
						else if (mouseDownRight) {
							screen->pixels[x_][y_] = 0;
						}
					}
					break;
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);		// Set color = white
		if (!pause) update(screen);
		draw(screen, renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);			// Reset color to black
		SDL_RenderPresent(renderer);
		if (pause) {
			SDL_Delay(1);
		}
		else {
			SDL_Delay(screen->delay);
		}
	}

	end_game(screen);			// Free all screen allocations
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}