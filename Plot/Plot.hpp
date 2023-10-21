//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_PLOT_HPP
#define VECTORFIELD_PLOT_HPP


#include <SDL.h>

#include "Misc/BivariateFunction.hpp"

class Plot
{
public:
    Plot (SDL_Renderer *, BivariateFunction, BivariateFunction);

    void draw () const;

private:
    //
};


#endif //VECTORFIELD_PLOT_HPP
