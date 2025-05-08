#pragma once

#include "Assets.h"

#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <map>


// Forward declaration
//enum class ASSETS_E;

enum class IMAGE_E;
enum class MUSIC_E;
enum class SOUND_E;

class ResourceManager
{
public:
    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    // For Images
    bool Load(IMAGE_E assetID, const std::string& filePath);
    SDL_Texture* Get(IMAGE_E assetID) const;
    bool Unload(IMAGE_E assetID);
    void UnloadAllImages();

    // For Music
    bool Load(MUSIC_E assetID, const std::string& filePath);
    Mix_Music* Get(MUSIC_E assetID) const;
    bool Unload(MUSIC_E assetID);
    void UnloadAllMusic();

    // For Sound Effects
    bool Load(SOUND_E assetID, const std::string& filePath);
    Mix_Chunk* Get(SOUND_E assetID) const;
    bool Unload(SOUND_E assetID);
    void UnloadAllSounds();

private:
    SDL_Renderer* m_renderer;
    std::map<IMAGE_E, SDL_Texture*> m_textureMap;
    std::map<MUSIC_E, Mix_Music*> m_musicMap;
    std::map<SOUND_E, Mix_Chunk*> m_soundMap;
};