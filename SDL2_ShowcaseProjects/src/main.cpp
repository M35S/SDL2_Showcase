#define SDL_MAIN_HANDLED

#include <SDL.h>

#include <iostream>



int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;

		return 1;
	}

	int width_t = 640;
	int height_t = 480;
	SDL_Window* window_t = nullptr;
	window_t = SDL_CreateWindow("SDL2 Test App", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_t, height_t, SDL_WINDOW_SHOWN);
	if (window_t == nullptr)
	{
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		
		return 1;
	}

	SDL_Renderer* renderer_t = nullptr;
	renderer_t = SDL_CreateRenderer(window_t, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer_t == nullptr)
	{
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;

		SDL_DestroyWindow(window_t);
		SDL_Quit();
		
		return 1;
	}

	SDL_Surface* screenSurface_t = nullptr;
	screenSurface_t = SDL_GetWindowSurface(window_t);

	//	Fill the surface white
	SDL_FillRect(
		screenSurface_t,
		NULL,
		SDL_MapRGB(
			screenSurface_t->format,
			0xFF,
			0xFF,
			0xFF
		)
	);

	SDL_UpdateWindowSurface(window_t);

	SDL_Delay(2000);

	SDL_FreeSurface(screenSurface_t);
	SDL_DestroyRenderer(renderer_t);
	SDL_DestroyWindow(window_t);

	SDL_Quit();

	return 0;
}