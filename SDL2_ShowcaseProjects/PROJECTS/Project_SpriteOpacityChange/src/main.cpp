#define SDL_MAIN_HANDLED

#include "DeltaTime.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Image.h"
#include "Font.h"

#include <iostream>


// Global variables ////////////////////////
//
bool isRunning_G;
Sprite spriteTest;

// Image components
Uint8 alpha_G = 255;
Uint8 red_G = 255;
Uint8 green_G = 255;
Uint8 blue_G = 255;

// Window
const int screenWidth_G = 840;
const int screenHeight_G = 600;

// Global funcs
void HandleKeyDown(SDL_Keysym key_p);
void HandleEvents(SDL_Event& event_p);


int main()
{
    Screen screen_t("Change Image Opacity Test", screenWidth_G, screenHeight_G);

    ResourceManager resourceManager_t(screen_t.GetRenderer());
    resourceManager_t.Load(IMAGE_E::ARROW, "arrow.png");

    Image arrowImage_t;
    arrowImage_t.SetupImage(resourceManager_t.Get(IMAGE_E::ARROW));

    //arrowImage_t.SetupSurface(screen_t.GetRenderer(), "arrow.png");

    // Main loop flag
    isRunning_G = true;

    // Timer variables
    Uint32 startTime = 0;
    Uint32 interval = 3000; // 3 seconds for a complete cycle from visible to invisible and back to visible

    // Start timer
    startTime = SDL_GetTicks();

    // Event handler
    SDL_Event event_t;
    
    while (isRunning_G)
    {
        HandleEvents(event_t);

        // Calculate the elapsed time and determine the phase
        Uint32 elapsed = SDL_GetTicks() - startTime;
        float phase = std::fmod(elapsed / static_cast<float>(interval), 1.0f);

        // Calculate alpha modulation based on the phase
        if (phase <= 0.5f)
        {
            // Fading out with ease-out effect
            float t = phase / 0.5f;
            t = easeInOutQuad(t);
            alpha_G = static_cast<Uint8>((1.0f - t) * 255);
        }
        else
        {
            // Fading in with ease-in effect
            float t = (phase - 0.5f) / 0.5f;
            t = easeInOutQuad(t);
            alpha_G = static_cast<Uint8>(t * 255);
        }


        // Clear screen
        SDL_SetRenderDrawColor(screen_t.GetRenderer(), 0, 0, 0, 255); // WHITE == 255, 255, 255, 255, BLACK == 0,0,0,255
        SDL_RenderClear(screen_t.GetRenderer());

        // Update texture
        arrowImage_t.SetAlpha(alpha_G);

        // Render texture
        arrowImage_t.Render(screen_t.GetRenderer(), 10, 10);

        // Update screen
        SDL_RenderPresent(screen_t.GetRenderer());

    }

    return 0;
}


void HandleEvents(SDL_Event& event_p)
{
    while (SDL_PollEvent(&event_p))
    {
        switch (event_p.type)
        {
        case SDL_QUIT: isRunning_G = false; break;

        case SDL_KEYDOWN: HandleKeyDown(event_p.key.keysym); break;
        }
    }
}


void HandleKeyDown(SDL_Keysym key_p)
{
    switch (key_p.sym) // Use key_p.sym to identify the pressed key
    {
    case SDLK_ESCAPE:
        std::cout << "ESCAPE key pressed!" << std::endl;
        isRunning_G = false;
        break;

    default:
        std::cout << "Unhandled key!" << std::endl;
        break;
    }
}
