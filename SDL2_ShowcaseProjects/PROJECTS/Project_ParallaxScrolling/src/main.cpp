#define SDL_MAIN_HANDLED

#include "Screen.h"
#include "ResourceManager.h"
#include "ScrollingManager.h"
#include "DeltaTime.h"

enum class ASSETS_E
{
    FOREGROUND,
    BACKGROUND,
    MIDGROUND, 
    MOUNTAIN, // add more as needed
    SKY
};

const int screenWidth_t = 840;
const int screenHeight_t = 600;

int main()
{
    Screen screen_t("Parallax Scrolling Demo", screenWidth_t, screenHeight_t);

    ResourceManager resourceManager_t(screen_t.GetRenderer());
    resourceManager_t.Load(ASSETS_E::SKY, "assets/images/Mountain/5sky.png");
    resourceManager_t.Load(ASSETS_E::MOUNTAIN, "assets/images/Mountain/4mountain.png");
    resourceManager_t.Load(ASSETS_E::BACKGROUND, "assets/images/Mountain/3background.png"); 
    resourceManager_t.Load(ASSETS_E::MIDGROUND, "assets/images/Mountain/2middleground.png");
    resourceManager_t.Load(ASSETS_E::FOREGROUND, "assets/images/Mountain/1foreground.png");

    //resourceManager_t.Load(ASSETS_E::SKY, "assets/images/Industrial/4sky.png");
    //resourceManager_t.Load(ASSETS_E::BACKGROUND, "assets/images/Industrial/3farBuildings.png"); 
    //resourceManager_t.Load(ASSETS_E::MIDGROUND, "assets/images/Industrial/2midBuildings.png");
    //resourceManager_t.Load(ASSETS_E::FOREGROUND, "assets/images/Industrial/1fgBuildings.png");

    //resourceManager_t.Load(ASSETS_E::SKY, "assets/images/Skies/2sky.png");
    //resourceManager_t.Load(ASSETS_E::FOREGROUND, "assets/images/Skies/1buildings.png");

    ScrollManager scrollManager_t(screen_t);

    ScrollLayer* sky_t = new ScrollLayer(screen_t, 0); // Zero speed for sky
    sky_t->SetupImage(resourceManager_t.Get(ASSETS_E::SKY));
    scrollManager_t.AddLayer(sky_t);

    ScrollLayer* mountain_t = new ScrollLayer(screen_t, 0); // Zero speed for background
    mountain_t->SetupImage(resourceManager_t.Get(ASSETS_E::MOUNTAIN));
    scrollManager_t.AddLayer(mountain_t);

    ScrollLayer* background_t = new ScrollLayer(screen_t, 40); // Slower speed for background
    background_t->SetupImage(resourceManager_t.Get(ASSETS_E::BACKGROUND));
    scrollManager_t.AddLayer(background_t);

    ScrollLayer* midground_t = new ScrollLayer(screen_t, 80); // Medium speed for midground
    midground_t->SetupImage(resourceManager_t.Get(ASSETS_E::MIDGROUND));
    scrollManager_t.AddLayer(midground_t);

    ScrollLayer* foreground_t = new ScrollLayer(screen_t, 120); // Faster speed for foreground
    foreground_t->SetupImage(resourceManager_t.Get(ASSETS_E::FOREGROUND));
    scrollManager_t.AddLayer(foreground_t);

    bool quit = false;
    SDL_Event event;

    DeltaTime deltaTime_t;
    deltaTime_t.start();

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        deltaTime_t.update();

        // Update scroll manager (updates all layers)
        scrollManager_t.Update(deltaTime_t.getDeltaTime());

        // Clear the screen (e.g., to black)
        SDL_SetRenderDrawColor(screen_t.GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(screen_t.GetRenderer());

        // Render scroll layers (order matters for parallax effect - sky first, foreground last)
        scrollManager_t.RenderLayer(screen_t.GetRenderer());

        // Update the screen
        SDL_RenderPresent(screen_t.GetRenderer());
    }

    return 0;
}
