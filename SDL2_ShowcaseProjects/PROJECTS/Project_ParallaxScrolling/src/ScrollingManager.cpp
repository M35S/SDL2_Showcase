#include "ScrollingManager.h"
//#include "Screen.h" // Include definition of Screen

ScrollLayer::ScrollLayer(Screen& screen, int speed)
    : m_screen(screen), m_speed(speed), m_texture(nullptr), m_scrollX(0.0f)
{
    m_src = { 0, 0, 0, 0 }; // Initialize to zero, will be set when image is setup
    m_dst = { 0, 0, screen.GetWidth(), screen.GetHeight() }; // Initial dst rect to cover the screen
    m_dstTwo = { m_dst.w, 0, screen.GetWidth(), screen.GetHeight() }; // Initial dst rect to cover the screen
}

ScrollLayer::~ScrollLayer()
{
    // Texture is managed by ResourceManager, so ScrollLayer should not destroy it.
    // If ScrollLayer was responsible for texture creation/destruction, you'd destroy m_texture here.
    SDL_DestroyTexture(this->m_texture); // Clean up textures. NOTE: Be wary of double freeing...
}

void ScrollLayer::SetupImage(SDL_Texture* texture)
{
    m_texture = texture;
    if (m_texture) {
        SDL_QueryTexture(m_texture, NULL, NULL, &m_src.w, &m_src.h); // Get texture width and height
    }
    else {
        m_src.w = m_src.h = 0; // Handle case where texture is null (error case)
    }
}

void ScrollLayer::Update(float deltaTime_p)
{
    if (m_speed != 0) // Only update for scrolling layers
    {
        //m_scrollX -= m_speed; // Scroll to the left. Adjust for direction.
        m_scrollX -= m_speed * deltaTime_p; // Multiply speed by deltaTime

        m_dst.x = static_cast<int>(m_scrollX);
        m_dstTwo.x = static_cast<int>(m_dst.x + m_dst.w); // Keep m_dst2 to the right of m_dst1

        // Example for horizontal wrapping. Adjust screen width and texture width if needed.
        if (m_dst.x <= -m_dst.w) 
        { // If layer has scrolled completely off-screen to the left
            m_scrollX += m_dst.w;  // Reset scroll position to wrap around
            m_dst.x = static_cast<int>(m_scrollX);
            m_dstTwo.x = static_cast<int>(m_dst.x + m_dst.w); // Recalculate m_dst2.x based on new m_dst1.x
        }
    }
    
    
}

void ScrollLayer::Render(SDL_Renderer* renderer)
{
    if (m_texture) 
    {
        if (m_speed != 0) // Render two images for scrolling layers
        {
            SDL_RenderCopy(renderer, m_texture, &m_src, &m_dst);
            SDL_RenderCopy(renderer, m_texture, &m_src, &m_dstTwo);
        }
        else // Render only one image for stationary layers
        {
            SDL_RenderCopy(renderer, m_texture, &m_src, &m_dst); // Still use m_dst1 as it's set by SetDstRect
        }
    }
}










ScrollManager::ScrollManager(Screen& screen)
    : m_screen(screen)
{
}

ScrollManager::~ScrollManager()
{
    // Important: Delete dynamically allocated ScrollLayer objects
    for (ScrollLayer* layer : m_layers) {
        delete layer;
    }
    m_layers.clear();
}

void ScrollManager::AddLayer(ScrollLayer* layer)
{
    m_layers.push_back(layer);
}

void ScrollManager::Update(float deltaTime_p)
{
    for (ScrollLayer* layer : m_layers) {
        layer->Update(deltaTime_p);
    }
}

void ScrollManager::RenderLayer(SDL_Renderer* renderer)
{
    for (ScrollLayer* layer : m_layers) {
        layer->Render(renderer);
    }
}