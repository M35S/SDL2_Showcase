#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL_render.h>
#include <SDL_image.h>

#include <string>
#include <map>
#include <iostream>

// Forward declaration
//enum class ASSETS_E;

enum class ASSETS_E;

class ResourceManager
{
public:
    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    bool Load(ASSETS_E assetID, const std::string& filePath);
    SDL_Texture* Get(ASSETS_E assetID) const;

    bool Unload(ASSETS_E assetID);
    void UnloadAll();

private:
    SDL_Renderer* m_renderer;
    std::map<ASSETS_E, SDL_Texture*> m_textureMap;
};

#endif