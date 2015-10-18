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
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderDrawPoint(renderer, 400, 300); //Renders on middle of screen.
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}