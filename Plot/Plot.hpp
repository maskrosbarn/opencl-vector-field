//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_PLOT_HPP
#define VECTORFIELD_PLOT_HPP


#include <SDL.h>

#include "misc/BivariateFunction.hpp"
#include "misc/constants.hpp"
#include "lib/SDL_FontCache/SDL_FontCache.h"

class Plot
{
public:
    [[nodiscard]] SDL_FPoint get_graphical_mouse_position () const;
    void       set_graphical_mouse_position (SDL_FPoint);

    [[nodiscard]] SDL_FPoint get_cartesian_mouse_position () const;
    void       set_cartesian_mouse_position (SDL_FPoint);

    [[nodiscard]] SDL_FPoint
        graphical_to_cartesian (SDL_FPoint) const,
        cartesian_to_graphical (SDL_FPoint) const;

    Plot (SDL_Renderer *, BivariateFunction, BivariateFunction);

    void update ();

    void update_vector_property_matrix ();

    void draw () const;

private:
    SDL_Renderer * renderer;

    FC_Font * const font = FC_CreateFont();

    BivariateFunction x_function, y_function;

    struct VectorProperties
    {
        float magnitude;

        SDL_Color colour;

        SDL_FPoint tail, head, head_left, head_right, tip;
    };

    VectorProperties vector_properties_matrix[SAMPLE_POINT_ROW_COUNT][SAMPLE_POINT_COLUMN_COUNT];

    struct
    {
        struct
        {
            SDL_FPoint
                cartesian { 0, 0 },
                graphical { 0, 0 };
        } position;

        SDL_Point cartesian_drag_origin { 0, 0 };

        bool has_left_button_pressed = false;
    } mouse;

    struct
    {
        SDL_FPoint cartesian_origin { 0, 0 };

        int range = 1;
    } viewport;

    [[nodiscard]] SDL_FPoint get_fixed_graphical_length_from_cartesian (SDL_FPoint, float, float) const;

    void draw_vector_field () const;
    void draw_vector (size_t, size_t) const;
};


#endif //VECTORFIELD_PLOT_HPP
