#pragma once

#include "ResourceManager.h"

#include <vector>


enum class ANIM_E
{
    NONE,
    LOOPING,
    SINGULAR
};

enum class COLOUR_MOD_E
{
    DARK,
    LIGHT
};


class Image
{
public:
    Image();
    ~Image();

    void SetupImage(SDL_Texture* texture);
    void SetupSurface(SDL_Renderer* renderer_p, const char* filePath);

    void UpdateColorModulation(float deltaTime_p, COLOUR_MOD_E colourMod_p);
    void ConvertToGrayscale(SDL_Renderer* renderer_p);

    void Render(SDL_Renderer* renderer_p);
    void Render(SDL_Renderer* renderer_p, int x, int y);

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetBlendMode(SDL_BlendMode blending);
    void SetAlpha(Uint8 alpha);

    int GetWidth() const;
    int GetHeight() const;
    SDL_Texture* GetTexture() const;

private:
    SDL_Texture* m_texture;
    SDL_Surface* m_surface;
    int m_width;
    int m_height;


    Uint8 m_red, m_green, m_blue;
    float m_elapsedTime;
};


class Sprite 
{
public:
    Sprite();

    void SetImage(ResourceManager& resourceManager, ASSETS_E assetID);
    void SetAnim(ANIM_E type_p, int frameWidth_p, int frameHeight_p);
    void SetAnim(const std::string& animType_p, int frameWidth_p, int frameHeight_p);
    void LoadAnimationFromFile(const std::string& jsonFileName, const std::string& animType);
    void LoadAnimationFromFile(const std::string& jsonFileName);
    
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, int x, int y);

private:
    std::vector<int> m_frameIndices;
    std::map<std::string, std::vector<int>> m_animFramesMap;
    Image m_image;
    ANIM_E m_type;
    int m_frameCounter;
    int m_frameTotal;
    int m_frameTime;
    int m_frameWidth;
    int m_frameHeight;
    float m_elapsedTime;
    int m_currentFrame;
};