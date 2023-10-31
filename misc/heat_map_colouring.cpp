//
// Created by Russell Forrest on 29/10/2023.
//

#include "misc/heat_map_colouring.hpp"

#include <cstdio>

SDL_Color get_heat_map_colour (float h)
{
    float r, g, b;

    if (h <= .25)
        r = 0, g = 255 * h / .25f, b = 255;

    else if (h > .25 && h <= .5)
        r = 255 * (4 * h - 1), g = b = 255;

    else if (h > .5 && h <= .75)
        r = g = 255, b = 255 * (3 - 4 * h);

    else
        r = 255, g = 255 * (4 - 4 * h), b = 0;

    return { (Uint8)r, (Uint8)g, (Uint8)b, 1 };
}