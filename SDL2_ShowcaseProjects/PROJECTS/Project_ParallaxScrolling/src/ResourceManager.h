#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>

#include <string>
#include <map>

// Forward declaration for ASSETS_E enum (you'll define this in your main file or a separate enum header)
enum class ASSETS_E;

class ResourceManager
{
public:
    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();

    bool Load(ASSETS_E assetID, const std::string& filePath);
    SDL_Texture* Get(ASSETS_E assetID) const;

private:
    SDL_Renderer* m_renderer;
    std::map<ASSETS_E, SDL_Texture*> m_textureMap;
};

#endif