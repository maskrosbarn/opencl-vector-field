//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_APPLICATION_HPP
#define VECTORFIELD_APPLICATION_HPP


#include <SDL.h>

#include "misc/BivariateFunction.hpp"
#include "Plot/Plot.hpp"
#include "ParticleEngine/ParticleEngine.hpp"

class Application
{
public:
    static const char * const name;

    Application (BivariateFunction, BivariateFunction);

private:
    SDL_Window * window;

    SDL_Renderer * renderer;

    bool is_running = true;

    Plot plot;

    ParticleEngine particle_engine;

    void main ();

    void did_receive_event (SDL_Event);

    void mouse_moved (SDL_MouseMotionEvent);
    void mouse_button_down (SDL_MouseButtonEvent);
    void mouse_button_up (SDL_MouseButtonEvent);
    void mouse_wheel_moved (SDL_MouseWheelEvent);
};


#endif //VECTORFIELD_APPLICATION_HPP
