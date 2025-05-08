#define SDL_MAIN_HANDLED

#include "Screen.h"
#include "ResourceManager.h"
#include "Image.h"
#include "DeltaTime.h"


// Global types
enum class ASSETS_E
{
    ARROW
};


// Global funcs

void HandleKeyDown(SDL_Keysym key_p);
void HandleEvents(SDL_Event& event_p);

unsigned char ClampValue(unsigned char member_p, int value_p);



// Global vars

bool isRunning_G;

// Image components
Uint8 alpha_G = 255;
Uint8 red_G = 255;
Uint8 green_G = 255;
Uint8 blue_G = 255;

const int screenWidth_G = 840;
const int screenHeight_G = 600;




int main()
{
    Screen screen_t("Change Image Colour Test", screenWidth_G, screenHeight_G);

    ResourceManager resourceManager_t(screen_t.GetRenderer());
    resourceManager_t.Load(ASSETS_E::ARROW, "arrow.png");

    Image arrowImage_t;
    arrowImage_t.SetupImage(resourceManager_t.Get(ASSETS_E::ARROW));

    //arrowImage_t.SetupSurface(screen_t.GetRenderer(), "arrow.png");

    // Main loop flag
    isRunning_G = true;

    // Event handler
    SDL_Event event_t;

    //arrowImage_t.ConvertToGrayscale(screen_t.GetRenderer());

    while (isRunning_G)
    {
        HandleEvents(event_t);
        
        // Clear screen
        SDL_SetRenderDrawColor(screen_t.GetRenderer(), 0, 0, 0, 255); // WHITE == 255, 255, 255, 255, BLACK == 0,0,0,255
        SDL_RenderClear(screen_t.GetRenderer());

        // Mod texture
        arrowImage_t.SetAlpha(alpha_G);
        arrowImage_t.SetColor(red_G, green_G, blue_G);
        

        arrowImage_t.Render(screen_t.GetRenderer(), 10, 10);

        // Update screen
        SDL_RenderPresent(screen_t.GetRenderer());
        
    } // while not quit

    return 0;
}



// Function defs

unsigned char ClampValue(unsigned char member_p, int value_p)
{
    // Add or subtract the value and clamp to the range [0, 255]
    unsigned char min_t = 0;
    unsigned char max_t = 255;

    return (member_p + value_p > max_t) ? max_t :
        ((member_p + value_p < min_t) ? min_t : member_p + value_p);
}


void HandleKeyDown(SDL_Keysym key_p)
{
    switch (key_p.sym) // Use key_p.sym to identify the pressed key
    {
    // RESTORE COMPONENTS
    case SDLK_r:
        std::cout << "R key pressed! Full Red value restored" << std::endl;
        // Add logic for R key
        red_G = 255;
        std::cout << "Red = " << (int)red_G << std::endl;
        break;

    case SDLK_g:
        std::cout << "G key pressed! Full Green value restored" << std::endl;
        // Add logic for G key
        green_G = 255;
        std::cout << "Green = " << (int)green_G << std::endl;
        break;

    case SDLK_b:
        std::cout << "B key pressed! Full Blue value restored" << std::endl;
        // Add logic for B key
        blue_G = 255;
        std::cout << "Blue = " << (int)blue_G << std::endl;
        break;

    case SDLK_a:
        std::cout << "A key pressed! Full Alpha value restored" << std::endl;
        // Add logic for A key
        alpha_G = 255;
        std::cout << "Alpha = " << (int)alpha_G << std::endl;
        break;


        // INCREMENT COMPONENTS

    case SDLK_1:
        std::cout << "1 key pressed! Decrease Red value" << std::endl;
        // Add logic for A key
        red_G = ClampValue(red_G, -35);

        std::cout << "Red = " << (int)red_G << std::endl;
        break;

    case SDLK_2:
        std::cout << "2 key pressed! Increase Red value" << std::endl;
        // Add logic for A key
        red_G = ClampValue(red_G, 35);

        std::cout << "Red = " << (int)red_G << std::endl;
        break;

    case SDLK_3:
        std::cout << "3 key pressed! Decrease Green value" << std::endl;
        // Add logic for A key
        green_G = ClampValue(green_G, -35);
        
        std::cout << "Green = " << (int)green_G << std::endl;
        break;

    case SDLK_4:
        std::cout << "4 key pressed! Increase Green value" << std::endl;
        // Add logic for A key
        green_G = ClampValue(green_G, 35);

        std::cout << "Green = " << (int)green_G << std::endl;
        break;

    case SDLK_5:
        std::cout << "5 key pressed! Decrease Blue value" << std::endl;
        // Add logic for A key
        blue_G = ClampValue(blue_G, -35);

        std::cout << "Blue = " << (int)blue_G << std::endl;
        break;

    case SDLK_6:
        std::cout << "6 key pressed! Increase Blue value" << std::endl;
        // Add logic for A key
        blue_G = ClampValue(blue_G, 35);

        std::cout << "Blue = " << (int)blue_G << std::endl;
        break;

    case SDLK_7:
        std::cout << "7 key pressed! Decrease Alpha value" << std::endl;
        // Add logic for A key
        alpha_G = ClampValue(alpha_G, -35);

        std::cout << "Alpha = " << (int)alpha_G << std::endl;
        break;

    case SDLK_8:
        std::cout << "8 key pressed! Increase Alpha value" << std::endl;
        // Add logic for A key
        alpha_G = ClampValue(alpha_G, 35);

        std::cout << "Alpha = " << (int)alpha_G << std::endl;
        break;

    case SDLK_ESCAPE:
        std::cout << "ESCAPE key pressed!" << std::endl;
        isRunning_G = false;
        break;

    case SDLK_l:
        std::cout << "L key pressed!" << std::endl;
        std::cout << "Red: " << (int)red_G << " | Green: " << (int)green_G << " | Blue: " << (int)blue_G << " | Alpha: " << (int)alpha_G << std::endl;
        break;

    default:
        std::cout << "Unhandled key!" << std::endl;
        break;
    }
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
