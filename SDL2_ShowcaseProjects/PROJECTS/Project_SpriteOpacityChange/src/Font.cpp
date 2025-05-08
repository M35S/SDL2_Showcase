#include "Font.h"
#include <iostream>

Font::Font(SDL_Renderer* renderer) : 
    m_renderer(renderer), m_font(nullptr), m_cachedTexture(nullptr), m_cachedText(""), m_cachedWidth(0), m_cachedHeight(0)
{
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }
}

Font::~Font()
{
    if (m_cachedTexture)
    {
        SDL_DestroyTexture(m_cachedTexture);
        m_cachedTexture = nullptr;
    }

    if (m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}


bool Font::LoadFont(const std::string& fontPath, int fontSize)
{
    // Destroy cached texture if it exists
    if (m_cachedTexture)
    {
        SDL_DestroyTexture(m_cachedTexture);
        m_cachedTexture = nullptr;
    }

    // Close the previous font if it exists
    if (m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }

    // Load the new font
    m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!m_font)
    {
        std::cerr << "Failed to load font: " << fontPath << " Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}


void Font::RenderText(const std::string& text, int x, int y, SDL_Color color)
{
    if (!m_font)
    {
        std::cerr << "Font not loaded!" << std::endl;
        return;
    }

    // Check if the text has changed
    if (text != m_cachedText)
    {
        // Free previous texture
        if (m_cachedTexture)
        {
            SDL_DestroyTexture(m_cachedTexture);
            m_cachedTexture = nullptr;
        }

        // Create a new surface and texture
        SDL_Surface* surface = TTF_RenderText_Blended(m_font, text.c_str(), color);
        if (!surface)
        {
            std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
            return;
        }

        m_cachedTexture = SDL_CreateTextureFromSurface(m_renderer, surface);
        m_cachedWidth = surface->w;
        m_cachedHeight = surface->h;
        SDL_FreeSurface(surface);

        if (!m_cachedTexture)
        {
            std::cerr << "Failed to create texture from rendered text: " << SDL_GetError() << std::endl;
            return;
        }

        // Cache the text
        m_cachedText = text;
    }

    // Render the cached texture
    SDL_Rect dstRect = { x, y, m_cachedWidth, m_cachedHeight };
    SDL_RenderCopy(m_renderer, m_cachedTexture, nullptr, &dstRect);
}