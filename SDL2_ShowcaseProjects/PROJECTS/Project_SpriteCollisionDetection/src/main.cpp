#define SDL_MAIN_HANDLED

#include "DeltaTime.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "Image.h"
#include "Font.h"

#include <iostream>

namespace SDLColors
{
    static SDL_Color NONE = { 0, 0, 0, 0 };

    static SDL_Color WHITE = { 255, 255, 255, 255 };
    static SDL_Color BLACK = { 0, 0, 0, 255 };

    static SDL_Color RED = { 255, 0, 0, 255 };
    static SDL_Color DKRED = { 127, 0, 0, 255 };

    static SDL_Color GREEN = { 0, 255, 0, 255 };
    static SDL_Color DKGREEN = { 0 ,127, 0, 255 };

    static SDL_Color BLUE = { 0, 0, 255, 255 };
    static SDL_Color DKBLUE = { 0, 0, 127, 255 };

    static SDL_Color PINK = { 255, 0, 127, 255 };
    static SDL_Color ORANGE = { 255, 127, 0, 255 };
    static SDL_Color YELLOW = { 255, 255, 0, 255 };
    static SDL_Color CYAN = { 0, 255, 255, 255 };
    static SDL_Color DCYAN = { 0, 127, 127, 255 };

    static SDL_Color GREY = { 63, 63, 63, 255 };
    static SDL_Color LTGREY = { 191, 191, 191, 255 };
    static SDL_Color DKGREY = { 150, 150, 150, 255 };
}


enum class CONTROL_E
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    CTRL_MAX
};


// Global variables ////////////////////////
//
bool isRunning_G;
Sprite spriteTest, spriteDummy;
const Uint8* keyState = SDL_GetKeyboardState(NULL);

int keyControls[static_cast<int>(CONTROL_E::CTRL_MAX)];


// Global funcs ///////////////////////////
//
void initializeKeyMappings();
void handleEvent(SDL_Event& event);
void handleInput(const Uint8* keyState);
void handleContinuousInput(const Uint8* keyState);

void HandleKeyDown(SDL_Keysym key_p, float dt_p);

void DoKeyUp(SDL_KeyboardEvent* event);

void HandleEvents(SDL_Event& event_p, const Uint8* key_p, float dt_p);
void HandleEvents(SDL_Event& event_p, float dt_p);

unsigned char ClampValue(unsigned char member_p, int value_p);





int main()
{
    int screenWidth_t = 800;
    int screenHeight_t = 600;

    Screen screen_t("Sprite Anim Demo", screenWidth_t, screenHeight_t);

    ResourceManager resourceManager_t(screen_t.GetRenderer());
    resourceManager_t.Load(IMAGE_E::TAILS, "assets/image/tails.png");

    
    spriteTest.SetImage(resourceManager_t, IMAGE_E::TAILS);

    spriteTest.LoadAnimationFromFile("data/anim/player.json");
    spriteTest.SetAnim("walk", 48, 48);

    spriteTest.SetupCollision();


    spriteDummy.SetImage(resourceManager_t, IMAGE_E::TAILS);
    spriteDummy.LoadAnimationFromFile("data/anim/player.json");
    spriteDummy.SetAnim("idle", 48, 48);
    spriteDummy.SetPosition(200.0f, 200.0f);
    spriteDummy.SetupCollision();

    Font font_t(screen_t.GetRenderer());
    font_t.LoadFont("assets/font/arial.ttf", 24);

    Font fontPos_t(screen_t.GetRenderer());
    fontPos_t.LoadFont("assets/font/arial.ttf", 24);

    isRunning_G = true;
    SDL_Event event_t;

    DeltaTime deltaTime_t;
    deltaTime_t.start();

    while (isRunning_G)
    {
        deltaTime_t.update();

        keyState = SDL_GetKeyboardState(NULL);
        
        HandleEvents(event_t, keyState, deltaTime_t.getDeltaTime());

        // Calculate FPS
        int fps_t = static_cast<int>(1.0f / deltaTime_t.getDeltaTime());

        // Update sprite anim
        spriteTest.Update(deltaTime_t.getDeltaTime());

        // Clear the screen (e.g., to black)
        SDL_SetRenderDrawColor(screen_t.GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(screen_t.GetRenderer());

        if (spriteDummy.CheckCollision(spriteDummy.m_collision, spriteTest.m_collision))
        {
            spriteDummy.m_collision.colour.red = 255;
            spriteDummy.m_collision.colour.green = 0;

            spriteTest.m_collision.colour.red = 255;
            spriteTest.m_collision.colour.green = 0;
        }
        else
        {
            spriteDummy.m_collision.colour.red = 0;
            spriteDummy.m_collision.colour.green = 255;

            spriteTest.m_collision.colour.red = 0;
            spriteTest.m_collision.colour.green = 255;
        }

        // render test sprite
        spriteDummy.Render(screen_t.GetRenderer(), spriteDummy.GetPosX(), spriteDummy.GetPosY());

        // Render sprite anim
        spriteTest.Render(screen_t.GetRenderer(), spriteTest.GetPosX(), spriteTest.GetPosY());

        // Render pos text
        int x_t = spriteTest.GetPosX();
        int y_t = spriteTest.GetPosY();
        fontPos_t.RenderText("Pos: X = " + std::to_string(x_t) + ", Y = " + std::to_string(y_t), 10, 40, SDLColors::RED);

        // Render FPS counter
        font_t.RenderText("FPS: " + std::to_string(fps_t), 10, 10, SDLColors::WHITE);

        // Update the screen
        SDL_RenderPresent(screen_t.GetRenderer());
    }
}



// Global Function implementations

void initializeKeyMappings()
{
    keyControls[static_cast<int>(CONTROL_E::LEFT)] = SDL_SCANCODE_A;
    keyControls[static_cast<int>(CONTROL_E::RIGHT)] = SDL_SCANCODE_D;
    keyControls[static_cast<int>(CONTROL_E::UP)] = SDL_SCANCODE_W;
    keyControls[static_cast<int>(CONTROL_E::DOWN)] = SDL_SCANCODE_S;
}

void handleEvent(SDL_Event& event)
{
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            std::cout << "Jump triggered!" << std::endl;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_F) {
            std::cout << "Fire triggered!" << std::endl;
        }
        break;

    case SDL_KEYUP:
        std::cout << "Key Released: " << event.key.keysym.scancode << std::endl;
        break;

    default:
        break;
    }
}

void handleInput(const Uint8* keyState)
{
    if (keyState[keyControls[static_cast<int>(CONTROL_E::LEFT)]]) {
        std::cout << "Moving Left!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::RIGHT)]]) {
        std::cout << "Moving Right!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::UP)]]) {
        std::cout << "Moving up!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::DOWN)]]) {
        std::cout << "Moving up!" << std::endl;
    }
    // Add more actions as needed
}

void handleContinuousInput(const Uint8* keyState)
{
    if (keyState[keyControls[static_cast<int>(CONTROL_E::LEFT)]]) {
        std::cout << "Moving Left!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::RIGHT)]]) {
        std::cout << "Moving Right!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::UP)]]) {
        std::cout << "Moving Up!" << std::endl;
    }
    if (keyState[keyControls[static_cast<int>(CONTROL_E::DOWN)]]) {
        std::cout << "Moving Down!" << std::endl;
    }
}

unsigned char ClampValue(unsigned char member_p, int value_p)
{
    // Add or subtract the value and clamp to the range [0, 255]
    unsigned char min_t = 0;
    unsigned char max_t = 255;

    return (member_p + value_p > max_t) ? max_t :
        ((member_p + value_p < min_t) ? min_t : member_p + value_p);
}


void HandleKeyDown(SDL_Keysym key_p, float dt_p)
{
    int velocityX = 0; // Horizontal velocity
    int velocityY = 0; // Vertical velocity

    switch (key_p.sym) // Use key_p.sym to identify the pressed key
    {
    case SDLK_LEFT:
        std::cout << "Left key pressed! Moving left" << std::endl;
        // Add logic
        velocityX = -800;
        std::cout << "Pos X = " << spriteTest.GetPosX() << std::endl;
        break;

    case SDLK_RIGHT:
        std::cout << "Left key pressed! Moving left" << std::endl;
        // Add logic
        velocityX = 800;
        std::cout << "Pos X = " << spriteTest.GetPosX() << std::endl;
        break;

    case SDLK_UP:
        std::cout << "Left key pressed! Moving left" << std::endl;
        // Add logic
        velocityY = -800;
        std::cout << "Pos Y = " << spriteTest.GetPosY() << std::endl;
        break;

    case SDLK_DOWN:
        std::cout << "Left key pressed! Moving left" << std::endl;
        // Add logic
        velocityY = 800;
        std::cout << "Pos Y = " << spriteTest.GetPosY() << std::endl;
        break;



    case SDLK_ESCAPE:
        std::cout << "ESCAPE key pressed!" << std::endl;
        isRunning_G = false;
        break;


    default:
        std::cout << "Unhandled key!" << std::endl;
        break;
    }

    spriteTest.UpdatePosition(dt_p, velocityX, velocityY);
}


void HandleEvents(SDL_Event& event_p, float dt_p)
{
    while (SDL_PollEvent(&event_p))
    {
        switch (event_p.type)
        {
        case SDL_QUIT: isRunning_G = false; break;

        case SDL_KEYDOWN: HandleKeyDown(event_p.key.keysym, dt_p); break;

        case SDL_KEYUP: HandleKeyDown(event_p.key.keysym, dt_p); break;
        }
    }
}



void HandleEvents(SDL_Event& event_p, const Uint8* key_p, float dt_p)
{
    while (SDL_PollEvent(&event_p))
    {
        switch (event_p.type)
        {
        case SDL_QUIT: isRunning_G = false; break;
        }
    }

    float velocityX = 0.0f; // Horizontal velocity
    float velocityY = 0.0f; // Vertical velocity

    if (keyState[SDL_SCANCODE_LEFT]) // Moving left
    {
        std::cout << "Left key pressed! Moving left" << std::endl;
        velocityX = -180.0f;

        spriteTest.FlipSpriteLeft(true); // Flip left
        spriteTest.SetAnimationState("walk", 48, 48); // Set walk animation
    }
    else if (keyState[SDL_SCANCODE_RIGHT]) // Moving right
    {
        std::cout << "Right key pressed! Moving right" << std::endl;
        velocityX = 180.0f;

        spriteTest.FlipSpriteLeft(false); // Flip right
        spriteTest.SetAnimationState("walk", 48, 48); // Set walk animation
    }
    else // Idle (no left/right movement)
    {
        spriteTest.SetAnimationState("idle", 48, 48); // Set idle animation
    }

    // Up/down movement can be included if needed
    if (keyState[SDL_SCANCODE_UP])
    {
        velocityY = -180.0f;
    }
    if (keyState[SDL_SCANCODE_DOWN])
    {
        velocityY = 180.0f;
    }

    // Update sprite position
    spriteTest.UpdatePosition(dt_p, velocityX, velocityY);
}



