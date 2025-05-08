#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <string>

class Screen
{
public:
    Screen(const std::string& title, int width, int height);
    ~Screen();

    SDL_Renderer* GetRenderer() const { return m_renderer; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;
};

#endif