//
// Created by Russell Forrest on 20/10/2023.
//

#include <cstdio>

#include "Plot.hpp"
#include "misc/BivariateFunction.hpp"
#include "misc/vector_computation.hpp"
#include "misc/heat_map_colouring.hpp"

SDL_FPoint Plot::get_graphical_mouse_position () const
{
    return mouse.position.graphical;
}

SDL_FPoint Plot::get_cartesian_mouse_position () const
{
    return mouse.position.cartesian;
}

SDL_FPoint Plot::get_cartesian_mouse_drag_origin () const
{
    return mouse.cartesian_drag_origin;
}

void Plot::set_cartesian_mouse_drag_origin (SDL_FPoint point)
{
    mouse.cartesian_drag_origin = point;
}

void Plot::set_viewport_cartesian_origin (SDL_FPoint point)
{
    viewport.cartesian_origin = point;
}

bool Plot::mouse_has_left_button_pressed () const
{
    return mouse.has_left_button_pressed;
}

void Plot::set_mouse_left_button_pressed (bool pressed)
{
    mouse.has_left_button_pressed = pressed;
}

SDL_FPoint Plot::get_viewport_cartesian_origin () const
{
    return viewport.cartesian_origin;
}

int Plot::get_viewport_range () const
{
    return viewport.range;
}

SDL_FPoint Plot::get_viewport_cartesian_drag_origin () const
{
    return viewport.cartesian_drag_origin;
}

void Plot::set_viewport_cartesian_drag_origin (SDL_FPoint point)
{
    viewport.cartesian_drag_origin = point;
}

SDL_FPoint Plot::graphical_to_cartesian (SDL_FPoint point) const
{
    return graphical_to_cartesian_space(point, viewport.cartesian_origin, viewport.range);
}

SDL_FPoint Plot::cartesian_to_graphical (SDL_FPoint point) const
{
    return cartesian_to_graphical_space(point, viewport.cartesian_origin, viewport.range);
}

Plot::Plot (SDL_Renderer * renderer, BivariateFunction x_function, BivariateFunction y_function):
    renderer   { renderer },
    x_function { x_function },
    y_function { y_function }
{
    FC_LoadFont(
            font,
            renderer,
            FONT_FILE_NAME,
            FONT_SIZE,
            FOREGROUND_COLOUR,
            TTF_STYLE_NORMAL
            );
}

void Plot::update ()
{
    update_mouse_position();

    update_axes();

    update_vector_property_matrix();
}

void Plot::update_mouse_position ()
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse.position.graphical = { (float)mouse_x, (float)mouse_y };

    mouse.position.cartesian = graphical_to_cartesian(mouse.position.graphical);
}

void Plot::update_axes ()
{
    axes_position = cartesian_to_graphical({ 0, 0 });

    axes_labels.x.positive.position = viewport.cartesian_origin.x + (float)viewport.range * .5f,
    axes_labels.x.negative.position = viewport.cartesian_origin.x - (float)viewport.range * .5f;

    axes_labels.y.positive.position = viewport.cartesian_origin.y + (float)viewport.range * .5f,
    axes_labels.y.negative.position = viewport.cartesian_origin.y - (float)viewport.range * .5f;
}

void Plot::update_vector_property_matrix ()
{
    VectorProperties * properties;

    SDL_FPoint
        cartesian_tail_position,
        direction_vector,
        cartesian_head_position;

    float vector_angle;

    for (size_t row = 0; row < SAMPLE_POINT_ROW_COUNT; row++)
    {
        for (size_t column = 0; column < SAMPLE_POINT_COLUMN_COUNT; column++)
        {
            properties = &vector_properties_matrix[row][column];

            properties->tail = {
                    (2 * (float)column + 1) * WINDOW_WIDTH / (2 * SAMPLE_POINT_COLUMN_COUNT),
                    (2 * (float)row + 1) * WINDOW_HEIGHT / (2 * SAMPLE_POINT_ROW_COUNT)
            };

            cartesian_tail_position = graphical_to_cartesian(properties->tail);

            direction_vector = { x_function(cartesian_tail_position), y_function(cartesian_tail_position) };

            properties->magnitude = magnitude(direction_vector);

            if (properties->magnitude > 0)
            {
                properties->colour = { 255, 255, 255, 1 };

                vector_angle = angle(direction_vector);

                properties->head = get_fixed_graphical_length_from_cartesian(
                        cartesian_tail_position,
                        VECTOR_ARROW_BODY_LENGTH,
                        vector_angle
                        );

                cartesian_head_position = graphical_to_cartesian(properties->head);

                properties->head_left = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        VECTOR_ARROW_HEAD_HALF_WIDTH,
                        vector_angle - (float)M_PI_4
                        );

                properties->head_right = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        VECTOR_ARROW_HEAD_HALF_WIDTH,
                        vector_angle + (float)M_PI_4
                        );

                properties->tip = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        VECTOR_ARROW_HEAD_LENGTH,
                        vector_angle
                        );
            }
            else
            {
                properties->colour = { 0, 0, 0, 1 };
            }

            //cartesian_head_position = cartesian_tail_position + direction_vector;
        }
    }
}

void Plot::draw () const
{
    float h = mouse.position.graphical.x / WINDOW_WIDTH;

    SDL_Color c = get_heat_map_colour(h);

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 1);

    SDL_FRect p { mouse.position.graphical.x, mouse.position.graphical.y, 50, 50 };
    SDL_RenderDrawRectF(renderer, &p);


    draw_vector_field();
    draw_axes();
    draw_axes_labels();
}

SDL_FPoint Plot::get_fixed_graphical_length_from_cartesian (SDL_FPoint point, float length, float angle) const
{
    SDL_FPoint
        graphical_position = cartesian_to_graphical(point),
        adjustment         = length * (SDL_FPoint){ cosf(-angle), sinf(-angle) };

    return graphical_position + adjustment;
}

void Plot::draw_vector_field () const
{
    for (size_t row = 0; row < SAMPLE_POINT_ROW_COUNT; row++)
    {
        for (size_t column = 0; column < SAMPLE_POINT_COLUMN_COUNT; column++)
            draw_vector(row, column);
    }
}

void Plot::draw_vector (size_t row, size_t column) const
{
    VectorProperties vector_properties = vector_properties_matrix[row][column];

    SDL_Color colour = vector_properties.colour;

    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

    SDL_FPoint
        tail       = vector_properties.tail,
        head       = vector_properties.head,
        head_left  = vector_properties.head_left,
        head_right = vector_properties.head_right,
        tip        = vector_properties.tip;

    SDL_RenderDrawLineF(renderer, tail.x, tail.y, head.x, head.y);

    SDL_Vertex vertices[] {
            { head, colour, { 0 } },
            { head_left, colour, { 0 } },
            { head_right, colour, { 0 } },
            { tip, colour, { 0 } }
    };

    int vertex_render_indices[] { 0, 1, 2, 1, 2, 3 };

    SDL_RenderGeometry(renderer, nullptr, vertices, 4, vertex_render_indices, 6);
}

void Plot::draw_axes () const
{
    SDL_SetRenderDrawColor(
            renderer,
            FOREGROUND_COLOUR.r,
            FOREGROUND_COLOUR.g,
            FOREGROUND_COLOUR.b,
            FOREGROUND_COLOUR.a
            );

    SDL_RenderDrawLineF(renderer, 0, axes_position.y, WINDOW_WIDTH, axes_position.y);
    SDL_RenderDrawLineF(renderer, axes_position.x, 0, axes_position.x, WINDOW_HEIGHT);
}

void Plot::draw_axes_labels () const
{
    //
}