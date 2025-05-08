#pragma once

#include <SDL_ttf.h>
#include <string>

class Font
{
public:
    Font(SDL_Renderer* renderer);
    ~Font();

    bool LoadFont(const std::string& fontPath, int fontSize);

    void RenderText(const std::string& text, int x, int y, SDL_Color color);

private:
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;

    std::string m_cachedText;
    SDL_Texture* m_cachedTexture;
    int m_cachedWidth;
    int m_cachedHeight;
};