#pragma once

#include <cmath>

// Easing function for a smooth transition
// Formula: f(t) = t < 0.5 ? 2t^2: -1 + (4 - 2t) x t
float easeInOutQuad(float t) 
{
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

// Starts slowly, then accelerates
// Formula: f(t) = t^2
float easeInQuad(float t) 
{
    return t * t;
}

// Starts quickly, then decelerates
// Formula: f(t) = t x (2 - t)
float easeOutQuad(float t) 
{
    return t * (2.0f - t);
}

// Starts very slowly, then accelerates rapidly
// Formula: f(t) = 2^10(t - 1) // Note, "^10(t - 1)" all on on one line
float easeInExpo(float t) 
{
    return t == 0 ? 0 : std::pow(2.0f, 10.0f * (t - 1));
}

// Starts rapidly and then decelerates
float easeOutExpo(float t) 
{
    return t == 1 ? 1 : 1 - std::pow(2.0f, -10.0f * t);
}

// Combines both ease-in & out effects for a smooth transition
float easeInOutCubic(float t) 
{
    return t < 0.5f ? 4.0f * t * t * t : (t - 1) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1;
}

// Transitions at constant speed
// Formula: f(t) = t
float linear(float t) 
{
    return t;
}