#include "ResourceManager.h"

#include <iostream>

ResourceManager::ResourceManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
    // Initialize SDL_image for PNG support
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
    }

    // Initialize SDL_mixer for audio support
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
    }
}

ResourceManager::~ResourceManager()
{
    // Clean up all resources
    UnloadAllImages();
    UnloadAllMusic();
    UnloadAllSounds();

    // Quit SDL subsystems
    Mix_CloseAudio();
    IMG_Quit();
}


bool ResourceManager::Load(IMAGE_E assetID, const std::string& filePath)
{
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, filePath.c_str());
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << filePath << " Error: " << IMG_GetError() << std::endl;
        return false;
    }
    m_textureMap[assetID] = texture;
    std::cout << "Image loaded: " << filePath << std::endl;
    return true;
}


SDL_Texture* ResourceManager::Get(IMAGE_E assetID) const
{
    auto it = m_textureMap.find(assetID);
    if (it != m_textureMap.end()) {
        return it->second;
    }
    std::cerr << "Texture not found for asset ID: " << static_cast<int>(assetID) << std::endl; // Cast for enum to int output
    return nullptr; // Or handle not found scenario as needed
}


bool ResourceManager::Unload(IMAGE_E assetID)
{
    auto it = m_textureMap.find(assetID);
    if (it != m_textureMap.end()) {
        SDL_DestroyTexture(it->second); // Destroy the associated texture
        m_textureMap.erase(it);        // Remove it from the map
        return true;                   // Indicate success
    }

    std::cerr << "Failed to unload texture for asset ID: " << static_cast<int>(assetID) << std::endl;
    return false; // Indicate failure if the asset wasn't found
}


void ResourceManager::UnloadAllImages()
{
    for (auto const& [assetID, texture] : m_textureMap) {
        SDL_DestroyTexture(texture); // Destroy each texture
    }
    m_textureMap.clear(); // Clear the map
}


bool ResourceManager::Load(MUSIC_E assetID, const std::string& filePath)
{
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << filePath << " Error: " << Mix_GetError() << std::endl;
        return false;
    }
    m_musicMap[assetID] = music;
    std::cout << "Music loaded: " << filePath << std::endl;
    return true;
}


Mix_Music* ResourceManager::Get(MUSIC_E assetID) const
{
    auto it = m_musicMap.find(assetID);
    if (it != m_musicMap.end()) {
        return it->second;
    }
    std::cerr << "Music not found for asset ID: " << static_cast<int>(assetID) << std::endl;
    return nullptr;
}


bool ResourceManager::Unload(MUSIC_E assetID)
{
    auto it = m_musicMap.find(assetID);
    if (it != m_musicMap.end()) {
        Mix_FreeMusic(it->second); // Free the music resource
        m_musicMap.erase(it);      // Remove it from the map
        return true;
    }
    std::cerr << "Failed to unload music for asset ID: " << static_cast<int>(assetID) << std::endl;
    return false;
}


void ResourceManager::UnloadAllMusic()
{
    for (auto const& [assetID, music] : m_musicMap) {
        Mix_FreeMusic(music); // Free each music resource
    }
    m_musicMap.clear();
}


bool ResourceManager::Load(SOUND_E assetID, const std::string& filePath)
{
    Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
    if (!chunk) {
        std::cerr << "Failed to load sound: " << filePath << " Error: " << Mix_GetError() << std::endl;
        return false;
    }
    m_soundMap[assetID] = chunk;
    std::cout << "Sound loaded: " << filePath << std::endl;
    return true;
}


Mix_Chunk* ResourceManager::Get(SOUND_E assetID) const
{
    auto it = m_soundMap.find(assetID);
    if (it != m_soundMap.end()) {
        return it->second;
    }
    std::cerr << "Sound not found for asset ID: " << static_cast<int>(assetID) << std::endl;
    return nullptr;
}


bool ResourceManager::Unload(SOUND_E assetID)
{
    auto it = m_soundMap.find(assetID);
    if (it != m_soundMap.end()) {
        Mix_FreeChunk(it->second); // Free the sound resource
        m_soundMap.erase(it);      // Remove it from the map
        return true;
    }
    std::cerr << "Failed to unload sound for asset ID: " << static_cast<int>(assetID) << std::endl;
    return false;
}



void ResourceManager::UnloadAllSounds()
{
    for (auto const& [assetID, chunk] : m_soundMap) {
        Mix_FreeChunk(chunk); // Free each sound effect
    }
    m_soundMap.clear();
}
