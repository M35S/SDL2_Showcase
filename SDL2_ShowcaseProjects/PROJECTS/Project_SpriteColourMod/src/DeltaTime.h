#pragma once

#include <SDL_stdinc.h>

class DeltaTime
{
public:
    // Constructor to initialize variables
    DeltaTime();

    // Start or reset the timer
    void start();

    // Update delta time for the current frame
    void update();

    // Get the current delta time
    float getDeltaTime() const;

private:
    Uint64 m_beginTicks;    // Time at the start of the frame
    float m_deltaTime;      // Time difference between frames in seconds
    float m_frequency;      // Performance frequency for high-precision timing
};