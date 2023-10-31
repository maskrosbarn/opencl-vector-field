//
// Created by Russell Forrest on 28/10/2023.
//

#ifndef VECTORFIELD_VECTOR_COMPUTATION_HPP
#define VECTORFIELD_VECTOR_COMPUTATION_HPP

#include <ostream>

#include <SDL.h>

float magnitude (SDL_FPoint);

float angle (SDL_FPoint);

SDL_FPoint operator + (SDL_FPoint, SDL_FPoint);

SDL_FPoint operator - (SDL_FPoint, SDL_FPoint);

SDL_FPoint operator * (float, SDL_FPoint);

SDL_FPoint
    graphical_to_cartesian_space (SDL_FPoint, SDL_FPoint, int),
    cartesian_to_graphical_space (SDL_FPoint, SDL_FPoint, int);

#endif //VECTORFIELD_VECTOR_COMPUTATION_HPP
