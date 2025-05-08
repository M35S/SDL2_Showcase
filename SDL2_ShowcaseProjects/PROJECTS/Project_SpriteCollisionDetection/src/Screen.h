#pragma once

#include <SDL.h>
#include <string>

class Screen
{
public:
    Screen(const std::string& title, int width, int height);
    ~Screen();

    SDL_Renderer* GetRenderer() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;
};