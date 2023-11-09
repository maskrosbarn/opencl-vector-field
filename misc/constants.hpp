//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_CONSTANTS_HPP
#define VECTORFIELD_CONSTANTS_HPP


#include <SDL.h>

#define TWO_PI 6.28318530717958647692528676655900576f

constexpr float
    WINDOW_WIDTH  = 950,
    WINDOW_HEIGHT = 950;

const char * const FONT_FILE_PATH = "/System/Library/Fonts/Monaco.ttf";

constexpr float FONT_SIZE = 14.f;

const SDL_Color
    BACKGROUND_COLOUR = { 0, 0, 0, 255 },
    FOREGROUND_COLOUR = { 255, 255, 255, 255 };

const int
    SAMPLE_POINT_ROW_COUNT    = 40,
    SAMPLE_POINT_COLUMN_COUNT = 40;

constexpr float
    SAMPLE_POINT_ROW_SPACING    = WINDOW_HEIGHT / SAMPLE_POINT_ROW_COUNT,
    SAMPLE_POINT_COLUMN_SPACING = WINDOW_WIDTH / SAMPLE_POINT_COLUMN_COUNT;

constexpr float
    VECTOR_ARROW_BODY_LENGTH     = 8.f,
    VECTOR_ARROW_HEAD_LENGTH     = 6.f,
    VECTOR_ARROW_HEAD_HALF_WIDTH = 3.F;

constexpr float AXES_LABEL_MARGIN = 5.f;

const int
    VIEWPORT_MAXIMUM_RANGE = 100,
    VIEWPORT_MINIMUM_RANGE = 1;

const int
    PARTICLE_COUNT = 100,
    PARTICLE_TRAIL_LENGTH = 10;

const char * const OPENCL_RUNGE_KUTTA_KERNEL_FILE_PATH = "/Users/forrest/CLionProjects/VectorField/kernels/runge_kutta.cl";


#endif //VECTORFIELD_CONSTANTS_HPP
