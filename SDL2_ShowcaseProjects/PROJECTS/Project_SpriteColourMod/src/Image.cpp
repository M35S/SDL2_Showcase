#include "Image.h"
#include "MathHelper.h"

#include <JSON/json.hpp>

#include <fstream>

Image::Image() :
    m_texture(nullptr),
    m_surface(nullptr),
    m_width(0),
    m_height(0),
    m_red(0),
    m_green(0),
    m_blue(0),
    m_elapsedTime(0.0f)
{

}


Image::~Image()
{
    if (this->m_texture != nullptr)
    {
        SDL_DestroyTexture(this->m_texture);
        this->m_texture = nullptr;
        this->m_width = 0;
        this->m_height = 0;
    }

    if (this->m_surface != nullptr)
    {
        SDL_FreeSurface(this->m_surface);
        m_surface = nullptr;
    }
}



void Image::SetupImage(SDL_Texture* texture)
{
    m_texture = texture;
    if (m_texture) 
    {
        SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height); // Get texture width and height

        // Extract original color (assuming the texture's default modulation is white)
        SDL_GetTextureColorMod(m_texture, &m_red, &m_green, &m_blue);

    }
    else 
    {
        std::cerr << "Failed to setup texture!";
    }
}


void Image::SetupSurface(SDL_Renderer* renderer_p, const char* filePath)
{
    this->m_surface = IMG_Load(filePath);
    this->m_texture = SDL_CreateTexture(renderer_p, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
        this->m_surface->w, m_surface->h);
    this->m_surface = SDL_ConvertSurfaceFormat(this->m_surface, SDL_PIXELFORMAT_ARGB8888, 0);
}



void Image::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //  Modulate texture rgb
    SDL_SetTextureColorMod(this->m_texture, red, green, blue);
}

void Image::SetBlendMode(SDL_BlendMode blending)
{
    //  Set blending function
    SDL_SetTextureBlendMode(this->m_texture, blending);
}

void Image::SetAlpha(Uint8 alpha)
{
    //  Modulate texture alpha
    SDL_SetTextureAlphaMod(this->m_texture, alpha);
}



int Image::GetWidth() const
{
    return m_width;
}


int Image::GetHeight() const
{
    return m_height;
}


SDL_Texture* Image::GetTexture() const
{
    return m_texture;
}



void Image::UpdateColorModulation(float deltaTime_p, COLOUR_MOD_E colourMod_p)
{
    // Accumulate delta time for total elapsed time
    m_elapsedTime += deltaTime_p;

    Uint8 red_t = 0;
    Uint8 green_t = 0;
    Uint8 blue_t = 0;

    // Modulation logic
    const float interval_t = 1.5f;  // 1.5 seconds for a full cycle
    float phase_t = std::fmod(m_elapsedTime / interval_t, 1.0f);

    switch (colourMod_p)
    {
    case COLOUR_MOD_E::DARK:
        if (phase_t <= 0.5f)
        {
            float t = phase_t / 0.5f;
            t = easeInOutQuad(t);
            red_t = static_cast<Uint8>((1.0f - t) * m_red);
            green_t = static_cast<Uint8>((1.0f - t) * m_green);
            blue_t = static_cast<Uint8>((1.0f - t) * m_blue);
        }
        else
        {
            float t = (phase_t - 0.5f) / 0.5f;
            t = easeInOutQuad(t);
            red_t = static_cast<Uint8>(t * m_red);
            green_t = static_cast<Uint8>(t * m_green);
            blue_t = static_cast<Uint8>(t * m_blue);
        }
        break;

    case COLOUR_MOD_E::LIGHT:
        if (phase_t <= 0.5f)
        {
            float t = phase_t / 0.5f;
            t = easeInOutQuad(t);
            red_t = static_cast<Uint8>(m_red + t * (255 - m_red));
            green_t = static_cast<Uint8>(m_green + t * (255 - m_green));
            blue_t = static_cast<Uint8>(m_blue + t * (255 - m_blue));
        }
        else
        {
            float t = (phase_t - 0.5f) / 0.5f;
            t = easeInOutQuad(t);
            red_t = static_cast<Uint8>(255 - t * (255 - m_red));
            green_t = static_cast<Uint8>(255 - t * (255 - m_green));
            blue_t = static_cast<Uint8>(255 - t * (255 - m_blue));
        }
        break;
    }

    std::cout << "Phase: " << phase_t << " | Red: " << (int)red_t << " | Green: " << (int)green_t << " | Blue: " << (int)blue_t << std::endl;


    // Apply the new color
    SetColor(red_t, green_t, blue_t);
}



void Image::ConvertToGrayscale(SDL_Renderer* renderer_p)
{
    if (m_surface == nullptr)
    {
        SDL_Log("No image surface to process texture.");
        return;
    }

    // Access pixel data and apply grayscale
    Uint32* pixels = static_cast<Uint32*>(this->m_surface->pixels);
    SDL_PixelFormat* pixelFormat = this->m_surface->format;

    for (int y = 0; y < this->m_surface->h; y++)
    {
        for (int x = 0; x < this->m_surface->w; x++)
        {
            Uint32 pixel = pixels[y * this->m_surface->w + x];

            // Extract RGB values
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, pixelFormat, &r, &g, &b, &a);

            // Calculate grayscale value using luminance formula
            Uint8 gray = static_cast<Uint8>(0.212671f * r + 0.715160f * g + 0.072169f * b);

            // Reconstruct the pixel with grayscale values
            pixel = SDL_MapRGBA(pixelFormat, gray, gray, gray, a);
            pixels[y * this->m_surface->w + x] = pixel;
        }
    }

    // Update the texture with the new surface
    SDL_UpdateTexture(m_texture, nullptr, this->m_surface->pixels, this->m_surface->pitch);

    //SDL_FreeSurface(this->m_surface);

    SDL_Log("Image converted to grayscale.");
}







void Image::Render(SDL_Renderer* renderer_p)
{
    if (m_texture == NULL)
    {
        SDL_Log("Texture not found!");
    }

    int windowWidth_t = 0;
    int windowHeight_t = 0;
    SDL_GetRendererOutputSize(renderer_p, &windowWidth_t, &windowHeight_t);

    SDL_Rect drawLocation;
    drawLocation.w = windowWidth_t;
    drawLocation.h = windowHeight_t;
    drawLocation.x = 0;
    drawLocation.y = 0;

    //SDL_Log("Drawing at x: %d, y: %d, w: %d, h: %d", drawLocation.x, drawLocation.y, drawLocation.w, drawLocation.h);
    SDL_RenderCopy(renderer_p, m_texture, nullptr, &drawLocation);
}


void Image::Render(SDL_Renderer* renderer_p, int x_p, int y_p)
{
    if (m_texture == NULL)
    {
        SDL_Log("Texture not found!");
    }

    int windowWidth_t = 0;
    int windowHeight_t = 0;
    SDL_GetRendererOutputSize(renderer_p, &windowWidth_t, &windowHeight_t);

    SDL_Rect drawLocation;
    drawLocation.w = windowWidth_t;
    drawLocation.h = windowHeight_t;
    drawLocation.x = x_p;
    drawLocation.y = y_p;

    //SDL_Log("Drawing at x: %d, y: %d, w: %d, h: %d", drawLocation.x, drawLocation.y, drawLocation.w, drawLocation.h);
    SDL_RenderCopy(renderer_p, m_texture, nullptr, &drawLocation);
}


/*
if (mTexture == NULL)
    {
        SDL_Log("Texture not found!");
        return false;
    }

    SDL_Rect drawLocation;
    drawLocation.w = this->m_game->m_window.GetWidth();
    drawLocation.h = this->m_game->m_window.GetHeight();
    drawLocation.x = this->m_position.x;
    drawLocation.y = this->m_position.y;

    //SDL_Log("Drawing at x: %d, y: %d, w: %d, h: %d", drawLocation.x, drawLocation.y, drawLocation.w, drawLocation.h);
    SDL_RenderCopy(game_p->m_window.GetRenderer(), mTexture.get(), nullptr, &drawLocation);

*/



Sprite::Sprite() :
    m_type(ANIM_E::NONE),
    m_frameCounter(0),
    m_frameTotal(0),
    m_frameTime(0),
    m_frameWidth(0),
    m_frameHeight(0),
    m_elapsedTime(0),
    m_currentFrame(0)
{


}



void Sprite::SetAnim(ANIM_E type_p, int frameWidth_p, int frameHeight_p)
{
    this->m_type = type_p;
    this->m_frameWidth = frameWidth_p;
    this->m_frameHeight = frameHeight_p;
    this->m_frameTime = 1000; // low value == fast anim, high value == slow anim
}



void Sprite::SetAnim(const std::string& animName_p, int frameWidth_p, int frameHeight_p)
{
    this->m_type = ANIM_E::LOOPING;
    this->m_frameWidth = frameWidth_p;
    this->m_frameHeight = frameHeight_p;
    this->m_frameTime = 200; // low value == fast anim, high value == slow anim

    // Retrieve the frame indices for the specified animation type
    if (m_animFramesMap.find(animName_p) != m_animFramesMap.end())
    {
        m_frameIndices = m_animFramesMap[animName_p];
        m_frameTotal = m_frameIndices.size();
        m_currentFrame = m_frameIndices[0];
        m_elapsedTime = 0.0f;
    }
    else
    {
        std::cerr << "Animation type not found: " << animName_p << std::endl;
    }
}



void Sprite::SetImage(ResourceManager& resourceManager, ASSETS_E assetID)
{
    SDL_Texture* texture = resourceManager.Get(assetID);
    if (texture)
    {
        m_image.SetupImage(texture); // Use the existing `SetupImage` method in the `Image` class
    }
    else
    {
        std::cerr << "Failed to retrieve texture for asset ID!" << std::endl;
    }
}




void Sprite::LoadAnimationFromFile(const std::string& jsonFileName, const std::string& animType)
{
    std::ifstream file(jsonFileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open JSON file: " << jsonFileName << std::endl;
        return;
    }

    nlohmann::json jsAnim;
    file >> jsAnim; // Read JSON data from file

    if (jsAnim.contains(animType))
    {
        auto& anim = jsAnim[animType];

        // Load frame sequence
        if (anim.contains("frames"))
        {
            m_frameTotal = anim["frames"].size();
            m_frameIndices = anim["frames"].get<std::vector<int>>(); // Store frame order
        }

        // Reset current frame and elapsed time
        m_currentFrame = 0;
        m_elapsedTime = 0.0f;
    }
    else
    {
        std::cerr << "Animation type not found: " << animType << std::endl;
    }

    file.close();
}




void Sprite::LoadAnimationFromFile(const std::string& jsonFileName)
{
    std::ifstream file(jsonFileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open JSON file: " << jsonFileName << std::endl;
        return;
    }

    nlohmann::json jsAnim;
    file >> jsAnim; // Read JSON data from file

    // Iterate over all animation types in the JSON file
    for (auto& [animType, animData] : jsAnim.items())
    {
        if (animData.contains("frames"))
        {
            // Load frame sequence and store it in the map
            std::vector<int> frames = animData["frames"].get<std::vector<int>>();
            m_animFramesMap[animType] = frames;
        }
        else
        {
            std::cerr << "Animation type '" << animType << "' is missing frame data!" << std::endl;
        }
    }

    file.close();
}




void Sprite::Update(float deltaTime)
{
    if (m_type == ANIM_E::NONE || m_frameTotal <= 1)
        return;

    m_elapsedTime += deltaTime * 1000; // Convert deltaTime to milliseconds

    if (m_elapsedTime >= m_frameTime)
    {
        m_elapsedTime -= m_frameTime;

        // Update the current frame
        m_frameCounter++;

        if (m_type == ANIM_E::LOOPING)
        {
            // Wrap back to the first frame when exceeding the frame count
            if (m_frameCounter >= m_frameTotal)
            {
                m_frameCounter = 0;
            }
        }
        else if (m_type == ANIM_E::SINGULAR && m_frameCounter >= m_frameTotal)
        {
            // Stop the animation if it's a singular animation
            m_frameCounter = m_frameTotal - 1; // Stay on the last frame
            m_type = ANIM_E::NONE;            // Mark the animation as finished
        }

        m_currentFrame = m_frameIndices[m_frameCounter];
    }
}



void Sprite::Render(SDL_Renderer* renderer, int x, int y)
{
    if (m_image.GetTexture() == nullptr)
    {
        std::cerr << "No texture is set for rendering!" << std::endl;
        return;
    }

    // Get the width of the entire texture
    int textureWidth = this->m_image.GetWidth();

    // Calculate srcRect's x and y positions based on the current frame
    SDL_Rect srcRect;
    srcRect.x = (m_currentFrame * m_frameWidth) % textureWidth; // Wrap horizontally
    srcRect.y = (m_currentFrame * m_frameWidth) / textureWidth * m_frameHeight; // Move to the correct row
    srcRect.w = m_frameWidth;
    srcRect.h = m_frameHeight;

    // Set the destination rectangle on the screen
    SDL_Rect dstRect = { x, y, m_frameWidth + 100, m_frameHeight + 100 };

    // Render the current frame
    SDL_RenderCopy(renderer, m_image.GetTexture(), &srcRect, &dstRect);
}
