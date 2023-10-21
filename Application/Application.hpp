//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_APPLICATION_HPP
#define VECTORFIELD_APPLICATION_HPP


#include <SDL.h>

#include "Misc/BivariateFunction.hpp"

class Application
{
public:
    Application (BivariateFunction, BivariateFunction);

private:
    SDL_Window * window;

    SDL_Renderer * renderer;

    bool is_running = true;

    void main ();

    void did_receive_event (SDL_Event);
};


#endif //VECTORFIELD_APPLICATION_HPP
