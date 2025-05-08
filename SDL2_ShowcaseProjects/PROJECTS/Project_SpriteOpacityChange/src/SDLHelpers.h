#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <sstream>

namespace SDLColors
{
	// define the most used colors
	extern SDL_Color WHITE;
	extern SDL_Color BLACK;
	extern SDL_Color RED;
	extern SDL_Color BLUE;
	extern SDL_Color GREEN;
	extern SDL_Color YELLOW;
	extern SDL_Color ORANGE;
	extern SDL_Color CYAN;
	extern SDL_Color DCYAN;
	extern SDL_Color PINK;
	extern SDL_Color GREY;
	extern SDL_Color LTGREY;
	extern SDL_Color DKBLUE;
	extern SDL_Color DKRED;
	extern SDL_Color DKGREEN;
}

class LTexture;

bool InitSDL(SDL_Window*& window_p, SDL_Renderer*& renderer_p);
bool LoadMedia(TTF_Font*& font_p);
void CloseSDL(SDL_Renderer*& renderer_p, SDL_Window*& window_p);

//Texture wrapper class
class LTexture
{
public:
	//  Initializes variables
	LTexture();

	//  Deallocates memory
	~LTexture();

	//  Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer*& renderer_p);

#if defined(SDL_TTF_MAJOR_VERSION)
	//  Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer*& renderer_p, TTF_Font*& font_p);
#endif

	//  Deallocates texture
	void free();

	//  Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//  Set blending
	void setBlendMode(SDL_BlendMode blending);

	//  Set alpha modulation
	void setAlpha(Uint8 alpha);

	//  Renders texture at given point
	void render(SDL_Renderer*& renderer_p, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//  Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//  The actual hardware texture
	SDL_Texture* mTexture;

	//  Image dimensions
	int mWidth;
	int mHeight;
};


//  The application time based timer
class LTimer
{
public:
	//  Initializes variables
	LTimer();

	//  The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//  Gets the timer's time
	Uint32 getTicks();

	//  Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//  The clock time when the timer started
	Uint32 mStartTicks;

	//  The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//  The timer status
	bool mPaused;
	bool mStarted;
};