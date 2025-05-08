#pragma once

#include "ResourceManager.h"
#include "Geometry.h"
#include "MathHelper.h"

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

    void UpdateColorModulation(float deltaTime_p, COLOUR_MOD_E colourMod_p);

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
    int m_width;
    int m_height;


    Uint8 m_red, m_green, m_blue;
    float m_elapsedTime;
};


class Sprite 
{
public:
    Sprite();

    void SetImage(ResourceManager& resourceManager, IMAGE_E assetID);
    //void SetAnim(ANIM_E type_p, int frameWidth_p, int frameHeight_p);
    void SetAnim(const std::string& animType_p, int frameWidth_p, int frameHeight_p);
    void LoadAnimationFromFile(const std::string& jsonFileName, const std::string& animType);
    void LoadAnimationFromFile(const std::string& jsonFileName);

    void SetupCollision();
    CollisionObject GetCollisionBox() const;
    bool CheckCollision(const CollisionObject& objOne, const CollisionObject& objTwo);

    void SetAnimationState(const std::string& state, int frameWidth, int frameHeight);
    
    void Update(float deltaTime);
    void UpdatePosition(float deltaTime, float velocityX, float velocityY);
    void Render(SDL_Renderer* renderer, float x, float y);

    void FlipSpriteLeft(bool flip_p);

    void SetPosition(float x, float y);
    float GetPosX() const;
    float GetPosY() const;

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

    float m_posX;
    float m_posY;
    bool m_flipHorizontal;
    std::string m_currentAnimState;

public:
    CollisionObject m_collision;
};