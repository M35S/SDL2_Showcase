#ifndef SCROLL_LAYER_H
#define SCROLL_LAYER_H

#include <SDL.h>

class Screen; // Forward declaration

class ScrollLayer
{
public:
    ScrollLayer(Screen& screen, int speed);
    ~ScrollLayer();

    void SetupImage(SDL_Texture* texture);
    void Update(float deltaTime_p);
    void Render(SDL_Renderer* renderer);

    void SetSrcRect(SDL_Rect src) { m_src = src; }
    void SetDstRect(SDL_Rect dst) { m_dst = dst; }
    void SetDstRectTwo(SDL_Rect dst) { m_dstTwo = dst; }
    SDL_Rect GetDstRect() const { return m_dst; }
    SDL_Rect GetDstRectTwo() const { return m_dstTwo; }

private:
    Screen& m_screen; // Reference to the screen
    int m_speed;       // Scrolling speed (pixels per frame, for example)
    SDL_Texture* m_texture;
    SDL_Rect m_src;
    SDL_Rect m_dst;
    SDL_Rect m_dstTwo;
    float m_scrollX;    // Current scroll position
};

#endif




#ifndef SCROLL_MANAGER_H
#define SCROLL_MANAGER_H

#include <vector>
#include "Screen.h" // Include Screen definition

class ScrollManager
{
public:
    ScrollManager(Screen& screen);
    ~ScrollManager();

    void AddLayer(ScrollLayer* layer); // Takes pointer to ScrollLayer
    void Update(float deltaTime_p);
    void RenderLayer(SDL_Renderer* renderer);

private:
    Screen& m_screen; // Reference to the Screen
    std::vector<ScrollLayer*> m_layers; // Vector to hold pointers to ScrollLayer objects
};

#endif