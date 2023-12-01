//
// Created by Russell Forrest on 28/10/2023.
//

#import <cmath>

#include "misc/vector_computation.hpp"
#include "misc/constants.hpp"

float magnitude (SDL_FPoint vector)
{
    return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
}

float angle (SDL_FPoint vector)
{
    float angle = acosf(vector.x / magnitude(vector));

    return vector.y > 0 ? angle : TWO_PI - angle;
}

SDL_FPoint operator + (SDL_FPoint lhs, SDL_FPoint rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

SDL_FPoint operator - (SDL_FPoint lhs, SDL_FPoint rhs)
{
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

SDL_FPoint operator * (float scalar, SDL_FPoint vector)
{
    return { scalar * vector.x, scalar * vector.y };
}

static float graphical_to_cartesian_x (float graphical_position, float cartesian_viewport_position, int viewport_range)
{
    float positional_fraction = (float)graphical_position / constants::window_size - .5f;

    return std::fmaf(positional_fraction, (float)viewport_range, cartesian_viewport_position);
}

static float graphical_to_cartesian_y (float graphical_position, float cartesian_viewport_position, int viewport_range)
{
    float positional_fraction = .5f - (float)graphical_position / constants::window_size;

    return std::fmaf(positional_fraction, (float)viewport_range, cartesian_viewport_position);
}

SDL_FPoint graphical_to_cartesian_space (SDL_FPoint vector, SDL_FPoint cartesian_viewport_position, int viewport_range)
{
    return {
            graphical_to_cartesian_x(vector.x, cartesian_viewport_position.x, viewport_range),
            graphical_to_cartesian_y(vector.y, cartesian_viewport_position.y, viewport_range)
    };
}

static float cartesian_to_graphical_x (float graphical_position, float cartesian_viewport_position, int viewport_range)
{
    float a = (graphical_position - cartesian_viewport_position) / (float)viewport_range;

    return constants::window_size * (a + .5f);
}

static float cartesian_to_graphical_y (float graphical_position, float cartesian_viewport_position, int viewport_range)
{
    float a = (graphical_position - cartesian_viewport_position) / (float)viewport_range;

    return constants::window_size * (.5f - a);
}

SDL_FPoint cartesian_to_graphical_space (SDL_FPoint vector, SDL_FPoint cartesian_viewport_position, int viewport_range)
{
    return {
            cartesian_to_graphical_x(vector.x, cartesian_viewport_position.x, viewport_range),
            cartesian_to_graphical_y(vector.y, cartesian_viewport_position.y, viewport_range)
    };
}