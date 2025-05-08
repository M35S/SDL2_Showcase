#include "Screen.h"
#include <iostream>

Screen::Screen(const std::string& title, int width, int height)
    : m_window(nullptr), m_renderer(nullptr), m_width(width), m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        // Handle error, perhaps throw an exception or exit
        return;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) 
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        // Handle error
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) 
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        // Handle error
        return;
    }
}

Screen::~Screen()
{
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

SDL_Renderer* Screen::GetRenderer() const { return m_renderer; }

int Screen::GetWidth() const { return m_width; }

int Screen::GetHeight() const { return m_height; }