//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_CONSTANTS_HPP
#define VECTORFIELD_CONSTANTS_HPP


#include <SDL.h>

#define TWO_PI 6.28318530717958647692528676655900576f

namespace constants
{
    constexpr float window_size = 800.f;

    constexpr float cursor_offset = 10.f;

    namespace font
    {
        const char * const file_path = "/System/Library/Fonts/Monaco.ttf";
        constexpr float size = 14.f;
    }

    namespace colour
    {
        constexpr SDL_Color background { 0, 0, 0, 255 };
        constexpr SDL_Color foreground { 255, 255, 255, 255 };
    }

    namespace vector_arrow
    {
        constexpr int row_sample_point_count = 40;

        constexpr float body_length     = 8.f;
        constexpr float head_length     = 6.f;
        constexpr float head_half_width = 3.f;
    }

    namespace particle
    {
        constexpr size_t count = 1000;

        constexpr SDL_Color colour { 100, 100, 100, 100 };
    }

    namespace axes
    {
        constexpr int maximum_range = 100;
        constexpr int minimum_range = 1;

        constexpr float label_margin = 5.f;
    }
}


#endif //VECTORFIELD_CONSTANTS_HPP
