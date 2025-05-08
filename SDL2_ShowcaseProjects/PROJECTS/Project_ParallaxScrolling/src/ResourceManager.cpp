#include "ResourceManager.h"
#include <SDL_image.h> // For IMG_LoadTexture
#include <iostream>

ResourceManager::ResourceManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { // Initialize SDL_image for PNG support
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        // Handle error
    }
}

ResourceManager::~ResourceManager()
{
    for (auto const& [assetID, texture] : m_textureMap) {
        SDL_DestroyTexture(texture); // Clean up textures
    }
    IMG_Quit(); // Quit SDL_image
}

bool ResourceManager::Load(ASSETS_E assetID, const std::string& filePath)
{
    SDL_Texture* texture = IMG_LoadTexture(m_renderer, filePath.c_str());
    if (texture == nullptr) {
        std::cerr << "Failed to load texture: " << filePath << " Error: " << IMG_GetError() << std::endl;
        return false;
    }
    m_textureMap[assetID] = texture;
    return true;
}

SDL_Texture* ResourceManager::Get(ASSETS_E assetID) const
{
    auto it = m_textureMap.find(assetID);
    if (it != m_textureMap.end()) {
        return it->second;
    }
    std::cerr << "Texture not found for asset ID: " << static_cast<int>(assetID) << std::endl; // Cast for enum to int output
    return nullptr; // Or handle not found scenario as needed
}