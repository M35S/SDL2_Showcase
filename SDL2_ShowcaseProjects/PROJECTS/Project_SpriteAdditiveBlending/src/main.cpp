#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char** argv)
{
    SDL_Event event;
    int quit = 0;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "2D Light test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Layers
    SDL_Texture* backgroundLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_Texture* lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_Texture* resultLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);

    // "Game" graphics rendered
    SDL_Surface* bgSurface = IMG_Load("backgroundPic.png");
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Light spot
    SDL_Surface* lightSurface = IMG_Load("spotWhite.png");
    SDL_Texture* lightTexture = SDL_CreateTextureFromSurface(renderer, lightSurface);
    SDL_FreeSurface(lightSurface);

    // New textures for the suggested blending mode
    SDL_Texture* lights = lightTexture;
    SDL_SetTextureBlendMode(lights, SDL_BLENDMODE_ADD);
    SDL_Texture* shadow = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_MOD);

    while (quit != 1)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // Fake game tiles, objects, players rendering
        SDL_SetRenderTarget(renderer, backgroundLayer);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_Rect gameRect = { 0, 0, 640, 480 };
        SDL_RenderCopy(renderer, bgTexture, NULL, &gameRect);

        SDL_SetRenderTarget(renderer, NULL);

        // Draw shadow and lights
        SDL_SetRenderTarget(renderer, shadow);
        SDL_SetRenderDrawColor(renderer, 60, 0, 100, 255); // example tint color
        SDL_RenderClear(renderer);

        SDL_Rect spot1 = { 10, 10, 200, 200 };
        SDL_Rect spot2 = { 240, 240, 200, 200 };

        SDL_RenderCopy(renderer, lights, NULL, &spot1);
        SDL_RenderCopy(renderer, lights, NULL, &spot2);

        SDL_SetRenderTarget(renderer, NULL);

        // Merge all layers
        SDL_SetRenderTarget(renderer, resultLayer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_SetTextureBlendMode(resultLayer, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, backgroundLayer, NULL, &gameRect);
        SDL_RenderCopy(renderer, shadow, NULL, &gameRect);

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, resultLayer, NULL, &gameRect);

        SDL_RenderPresent(renderer); // show everything
    }

    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(lightTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

