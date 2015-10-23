#include <vector>
#include <SDL.h>

template<typename T> inline const T abs(T const & x)
{
	return ( x < 0) ? -x : x;
}

const int SIZE_X = 800, SIZE_Y = 600;
const int MAX_PARTICLES = 200;

class Particle
{

	enum Color
	{
		RED,
		GREEN,
		BLUE
	};

	enum Direction
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	int getRed(Color color)
	{
		switch(color)
		{
		case RED:
			return 255;
		case GREEN:
		case BLUE:
			return 0;
		}
	}
	int getGreen(Color color)
	{
		switch(color)
		{
		case GREEN:
			return 255;
		case RED:
		case BLUE:
			return 0;
		}
	}
	int getBlue(Color color)
	{
		switch(color)
		{
		case BLUE:
			return 255;
		case RED:
		case GREEN:
			return 0;
		}
	}

	int x, y;
	Direction dir;
	Color color;
	int delay;
	Uint32 storedTime;

	void changeColor()
	{
		switch (rand() % 3)
		{
		case 0:
			color = RED;
			break;
		case 1:
			color = GREEN;
			break;
		case 2:
			color = BLUE;
			break;
		}
	}

	void changeDir()
	{
		switch (rand() % 4)
		{
		case 0:
			dir = LEFT;
			break;
		case 1:
			dir = RIGHT;
			break;
		case 2:
			dir = UP;
			break;
		case 3:
			dir = DOWN;
			break;
		}
	}

	bool inBounds()
	{
		return ((unsigned) x < SIZE_X)
				&& ((unsigned) y < SIZE_Y);
	}
public:

	bool update(Uint32 delta)
	{
		storedTime += delta;
		if (storedTime > delay)
		{
			x += dir == LEFT ? -1 : (dir == RIGHT ? 1 : 0);
			y += dir == UP ? -1 : (dir == DOWN ? 1 : 0);
			storedTime -= delay;
			return !inBounds();
		}
		return false;
	}

	void draw(Uint32 *pixels, int sx, Uint32 format)
	{
		pixels[y * sx + x] = SDL_MapRGB(SDL_AllocFormat(format), getRed(color), getGreen(color), getBlue(color));
	}

	Particle() : x(rand() % (SIZE_X / 4) + (SIZE_X * 3) / 8)
	, y(rand() % (SIZE_Y / 4) + (SIZE_Y * 3) / 8), storedTime(0)
	{
		bool horizontal = abs(x - SIZE_X / 2) > abs(y - SIZE_Y / 2);
		dir = horizontal ? (x < SIZE_X / 2 ? LEFT : RIGHT) : (y < SIZE_Y / 2 ? UP : DOWN);
		delay = rand() % 60 + 10;
		changeColor();
	}
};

int main()
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	window = SDL_CreateWindow("Prata Demo", 10, 10, SIZE_X, SIZE_Y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	std::vector<Particle> particles;
	Sint32 ticksTillAdd = 1000;
	Uint32 now, last = 0, delta = 0;
	//Uint32 *pixels = *(new Uint32[SIZE_X][SIZE_Y]);
	
	SDL_Texture * texture = SDL_CreateTexture(renderer,
											SDL_PIXELFORMAT_ARGB8888,
											SDL_TEXTUREACCESS_STREAMING,
											SIZE_X, SIZE_Y);

	while (1)
	{
		SDL_Surface surface(*SDL_GetWindowSurface(window));
		Uint32 *pixels = (Uint32*) surface.pixels;
		now = SDL_GetTicks();
		delta = now - last;
		last = now;
		ticksTillAdd -= delta;
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}
		if (particles.size() < MAX_PARTICLES && ticksTillAdd < 100)
		{
			ticksTillAdd = 200;
			particles.push_back(Particle());
		}
		for (auto i = 0; i < particles.size(); ++i)
		{
			if (particles[i].update(delta))
			{
				particles.erase(particles.begin() + i);
				printf("Deleted %d\nSize:%d\n", i, particles.size());
				--i;
				continue;
			}
			particles[i].draw(pixels, SIZE_X, SDL_GetWindowPixelFormat(window));
		}
		// replace the old pixels with the new ones
		SDL_UpdateTexture(texture, NULL, pixels, SIZE_X * sizeof (Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}