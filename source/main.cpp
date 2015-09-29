#include "graphics.h"
#include <SDL.h>

int main()
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	//SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
	window = SDL_CreateWindow("Prata", 10, 10, 800, 600, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	while (1)
	{
		SDL_RenderDrawPoint(renderer, 400, 300); //Renders on middle of screen.
		SDL_RenderPresent(renderer);
	}
}