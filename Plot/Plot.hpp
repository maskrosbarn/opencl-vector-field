//
// Created by Russell Forrest on 20/10/2023.
//

#ifndef VECTORFIELD_PLOT_HPP
#define VECTORFIELD_PLOT_HPP

#include <array>

#include <SDL.h>

#include "misc/BivariateFunction.hpp"
#include "misc/constants.hpp"
#include "lib/SDL_FontCache/SDL_FontCache.h"

class Plot
{
public:
    void update_on_next_pass ();

    [[nodiscard]] SDL_FPoint get_graphical_mouse_position () const;
    [[nodiscard]] SDL_FPoint get_cartesian_mouse_position () const;

    [[nodiscard]] SDL_FPoint get_cartesian_mouse_drag_origin () const;
    void set_cartesian_mouse_drag_origin (SDL_FPoint);

    [[nodiscard]] bool mouse_has_left_button_pressed () const;

    void set_mouse_left_button_pressed (bool);

    [[nodiscard]] SDL_FPoint get_viewport_cartesian_origin () const;

    void set_viewport_cartesian_origin (SDL_FPoint);

    [[nodiscard]] SDL_FPoint get_viewport_cartesian_drag_origin () const;
    void set_viewport_cartesian_drag_origin (SDL_FPoint);

    [[nodiscard]] int get_viewport_range () const;
    void set_viewport_range (int);

    [[nodiscard]] SDL_FPoint
        graphical_to_cartesian (SDL_FPoint) const,
        cartesian_to_graphical (SDL_FPoint) const;

    Plot (SDL_Renderer *, BivariateFunction, BivariateFunction);

    void update ();

    void draw () const;

private:
    SDL_Renderer * renderer;

    FC_Font * const font = FC_CreateFont();
    float font_line_height;

    BivariateFunction x_function, y_function;

    struct VectorProperties
    {
        float magnitude;

        SDL_FPoint tail, head, head_left, head_right, tip;
    };

    std::array<std::array<VectorProperties, constants::vector_arrow::row_sample_point_count>, constants::vector_arrow::row_sample_point_count> vector_properties_matrix {};

    bool needs_update = true;

    float
        maximum_sample_point_magnitude = 0,
        minimum_sample_point_magnitude = 0;

    struct
    {
        struct
        {
            SDL_FPoint
                cartesian { 0, 0 },
                graphical { 0, 0 };
        } position;

        SDL_FPoint cartesian_drag_origin { 0, 0 };

        bool has_left_button_pressed = false;
    } mouse;

    struct
    {
        SDL_FPoint
            cartesian_origin      { 0, 0 },
            cartesian_drag_origin { 0, 0 };

        int range = 20;
    } viewport;

    struct
    {
        const char * text_format = "%.1f";

        struct
        {
            struct
            {
                float label_value = 0;
                int text_width = 0;

                SDL_FPoint position { 0, 0 };
            } positive;

            struct
            {
                float label_value = 0;
                int text_width = 0;

                SDL_FPoint position { 0, 0 };
            } negative;
        } x;

        struct
        {
            struct
            {
                float label_value = 0;
                int text_width = 0;

                SDL_FPoint position { 0, 0 };
            } positive;

            struct
            {
                float label_value = 0;
                int text_width = 0;

                SDL_FPoint position { 0, 0 };
            } negative;
        } y;
    } axes_labels;

    SDL_FPoint axes_position { 0, 0 };

    [[nodiscard]] SDL_FPoint get_fixed_graphical_length_from_cartesian (SDL_FPoint, float, float) const;

    void update_mouse_position ();
    void update_axes ();
    void update_vector_property_matrix ();

    void draw_vector_field () const;
    void draw_vector (size_t, size_t) const;

    void draw_axes () const;
};


#endif //VECTORFIELD_PLOT_HPP
