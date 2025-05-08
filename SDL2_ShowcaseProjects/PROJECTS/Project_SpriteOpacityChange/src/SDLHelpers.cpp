#include "SDLHelpers.h"

namespace SDLColors
{
	// define the most used colors
	SDL_Color WHITE = { 255, 255, 255, 0 };
	SDL_Color BLACK = { 0, 0, 0, 0 };
	SDL_Color RED = { 255, 0, 0, 0 };
	SDL_Color BLUE = { 0, 0, 255, 0 };
	SDL_Color GREEN = { 0, 255, 0, 0 };
	SDL_Color YELLOW = { 255, 255, 0, 0 };
	SDL_Color CYAN = { 0, 255, 255, 0 };
	SDL_Color PINK = { 255, 0, 127, 0 };
	SDL_Color ORANGE = { 255, 127, 0, 0 };
	SDL_Color DCYAN = { 0, 127, 127, 0 };
	SDL_Color GREY = { 63, 63, 63, 0 };
	SDL_Color LTGREY = { 191, 191, 191, 0 };
	SDL_Color DKBLUE = { 0, 0, 127, 0 };
	SDL_Color DKRED = { 127, 0, 0, 0 };
	SDL_Color DKGREEN = { 0 ,127, 0, 0 };
}

bool InitSDL(SDL_Window*& window_p, SDL_Renderer*& renderer_p)
{
	//  Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	
	//  Initialization flag
	bool success = true;

	//  Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//  Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//  Create window
		window_p =
			SDL_CreateWindow(
				"SDL Tutorial",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN
			);

		if (window_p == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//  Create vsynced renderer for window
			renderer_p =
				SDL_CreateRenderer(
					window_p,
					-1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
				);

			if (renderer_p == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//  Initialize renderer color
				SDL_SetRenderDrawColor(renderer_p, 0xFF, 0xFF, 0xFF, 0xFF);

				//  Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//  Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}


bool LoadMedia(TTF_Font*& font_p)
{
	//  Loading success flag
	bool success = true;

	//  Open the font
	font_p = TTF_OpenFont("./ARIAL.TTF", 28);
	if (font_p == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

void CloseSDL(SDL_Renderer*& renderer_p, SDL_Window*& window_p)
{
	//  Destroy window	
	SDL_DestroyRenderer(renderer_p);
	SDL_DestroyWindow(window_p);
	window_p = NULL;
	renderer_p = NULL;

	//  Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

/////////////////////////////////////////////////////////////////////////////////////
// TEXTURE
////////////////////////////////////////////////////////////////////////////////////
LTexture::LTexture()
{
	//  Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//  Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer*& renderer_p)
{
	//  Get rid of preexisting texture
	free();

	//  The final texture
	SDL_Texture* newTexture = NULL;

	//  Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//  Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//  Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer_p, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//  Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//  Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//  Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer*& renderer_p, TTF_Font*& font_p)
{
	//  Get rid of preexisting texture
	free();

	//  Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font_p, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//  Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer_p, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//  Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//  Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//  Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//  Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//  Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//  Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//  Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer*& renderer_p, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//  Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//  Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//  Render to screen
	SDL_RenderCopyEx(renderer_p, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}



/////////////////////////////////////////////////////////////////////////////////////
// TIMER
////////////////////////////////////////////////////////////////////////////////////
LTimer::LTimer()
{
	//  Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//  Start the timer
	mStarted = true;

	//  Unpause the timer
	mPaused = false;

	//  Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//  Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//  Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//  If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//  Pause the timer
		mPaused = true;

		//  Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//  If the timer is running and paused
	if (mStarted && mPaused)
	{
		//  Unpause the timer
		mPaused = false;

		//  Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//  Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//  The actual timer time
	Uint32 time = 0;

	//  If the timer is running
	if (mStarted)
	{
		//  If the timer is paused
		if (mPaused)
		{
			//  Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//  Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//  Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//  Timer is running and paused
	return mPaused && mStarted;
}