#pragma once

#include <cmath>


// Easing function for a smooth transition
// Formula: f(t) = t < 0.5 ? 2t^2: -1 + (4 - 2t) x t
float easeInOutQuad(float t);

// Starts slowly, then accelerates
// Formula: f(t) = t^2
float easeInQuad(float t);

// Starts quickly, then decelerates
// Formula: f(t) = t x (2 - t)
float easeOutQuad(float t);

// Starts very slowly, then accelerates rapidly
// Formula: f(t) = 2^10(t - 1) // Note, "^10(t - 1)" all on on one line
float easeInExpo(float t);

// Starts rapidly and then decelerates
float easeOutExpo(float t);

// Combines both ease-in & out effects for a smooth transition
float easeInOutCubic(float t);

// Transitions at constant speed
// Formula: f(t) = t
float linear(float t);