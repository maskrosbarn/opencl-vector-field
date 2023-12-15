//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_CONSTANTS_HPP
#define VECTORFIELD_CONSTANTS_HPP


#include <SDL.h>

#define TWO_PI 6.28318530717958647692528676655900576f

namespace constants
{
    const float window_size = 950.f;

    namespace font
    {
        const char * const file_path = "/System/Library/Fonts/Monaco.ttf";
        constexpr float size = 14.f;
    }

    namespace colour
    {
        const SDL_Color background { 0, 0, 0, 255 };
        const SDL_Color foreground { 255, 255, 255, 255 };
    }

    namespace vector_arrow
    {
        const int row_sample_point_count = 40;

        constexpr float body_length     = 8.f;
        constexpr float head_length     = 6.f;
        constexpr float head_half_width = 3.f;
    }

    namespace particle
    {
        const size_t count            = 1000;
        const size_t trail_length     = 10;
        const size_t data_point_count = count * trail_length;

        const SDL_Color colour { 164, 164, 164, 1 };
    }

    namespace axes
    {
        const int maximum_range = 100;
        const int minimum_range = 1;

        constexpr float label_margin = 5.f;
    }
}


#endif //VECTORFIELD_CONSTANTS_HPP
