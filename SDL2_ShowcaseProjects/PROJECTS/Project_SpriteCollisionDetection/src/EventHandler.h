#pragma once

#include <SDL_events.h>

class EventHandler
{
public:
    EventHandler(SDL_Event& event_p);

    void HandleEvents(bool& state_p);

    void HandleKeyDown(const SDL_Keysym& key);
    void HandleKeyUp(const SDL_Keysym& key);

    void HandleMouseDown(const SDL_MouseButtonEvent& button);
    void HandleMouseUp(const SDL_MouseButtonEvent& button);

public:
    SDL_Event m_event;
};
