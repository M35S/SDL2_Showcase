#include "DeltaTime.h"

DeltaTime::DeltaTime() : 
    m_beginTicks(0), 
    m_deltaTime(1.0f / 60.0f), 
    m_frequency(SDL_GetPerformanceFrequency()) 
{

}



void DeltaTime::start()
{
    m_beginTicks = SDL_GetPerformanceCounter();
}



void DeltaTime::update()
{
    Uint64 t_endTicks = SDL_GetPerformanceCounter();
    m_deltaTime = static_cast<float>(t_endTicks - m_beginTicks) / m_frequency;

    // Clamp delta time to avoid instability with extreme values
    if (m_deltaTime > 1.0f)
    {
        m_deltaTime = 1.0f / 30.0f;
    }

    // Prepare for the next frame
    m_beginTicks = t_endTicks;
}



float DeltaTime::getDeltaTime() const
{
    return m_deltaTime;
}