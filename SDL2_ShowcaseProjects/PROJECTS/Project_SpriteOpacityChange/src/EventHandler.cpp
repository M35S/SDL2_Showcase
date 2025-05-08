#include "EventHandler.h"

#include <iostream>

EventHandler::EventHandler(SDL_Event& event_p)
    : m_event(event_p)
{
}

void EventHandler::HandleEvents(bool& state_p)
{
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.type)
        {
        case SDL_QUIT:
            state_p = false;
            break;

        case SDL_KEYDOWN:
            HandleKeyDown(m_event.key.keysym);
            if (m_event.key.keysym.sym == SDLK_ESCAPE) // Check for Escape key here, QUICK FIX for now....
            {
                state_p = false; // Set global running state to false
            }
            break;

        case SDL_KEYUP:
            HandleKeyUp(m_event.key.keysym);
            break;

        case SDL_MOUSEBUTTONDOWN:
            HandleMouseDown(m_event.button);
            break;

        case SDL_MOUSEBUTTONUP:
            HandleMouseUp(m_event.button);
            break;

        default:
            break;
        }
    }
}


void EventHandler::HandleKeyDown(const SDL_Keysym& key_p)
{
    switch (key_p.sym)
    {
    case SDLK_r:
        std::cout << "R key pressed!" << std::endl;
        //red_G = 255; // Update red color
        //std::cout << "Red = " << (int)red_G << std::endl;
        break;

    case SDLK_g:
        std::cout << "G key pressed!" << std::endl;
        //green_G = 255; // Update green color
        //std::cout << "Green = " << (int)green_G << std::endl;
        break;

    case SDLK_b:
        std::cout << "B key pressed!" << std::endl;
        //blue_G = 255; // Update blue color
        //std::cout << "Blue = " << (int)blue_G << std::endl;
        break;

    case SDLK_ESCAPE:
        std::cout << "Escape key pressed. Exiting..." << std::endl;
        this->m_event.type = SDL_QUIT;
        //this->m_isRunning = false;
        break;


    default:
        std::cout << "Unhandled key: " << SDL_GetKeyName(key_p.sym) << std::endl;
        break;
    }
}


void EventHandler::HandleKeyUp(const SDL_Keysym& key_p)
{
    std::cout << "Key released: " << SDL_GetKeyName(key_p.sym) << std::endl;
}


void EventHandler::HandleMouseDown(const SDL_MouseButtonEvent& mouseButton_p)
{
    std::cout << "Mouse button pressed at (" << mouseButton_p.x << ", " << mouseButton_p.y << ")" << std::endl;

    switch (mouseButton_p.button)
    {
    case SDL_BUTTON_LEFT:
        std::cout << "Left mouse button pressed." << std::endl;
        break;

    case SDL_BUTTON_RIGHT:
        std::cout << "Right mouse button pressed." << std::endl;
        break;

    case SDL_BUTTON_MIDDLE:
        std::cout << "Middle mouse button pressed." << std::endl;
        break;

    default:
        std::cout << "Unknown mouse button: " << (int)mouseButton_p.button << std::endl;
        break;
    }
}

void EventHandler::HandleMouseUp(const SDL_MouseButtonEvent& mouseButton_p)
{
    std::cout << "Mouse button released at (" << mouseButton_p.x << ", " << mouseButton_p.y << ")" << std::endl;
}