//
// Created by Russell Forrest on 20/10/2023.
//

#include "Plot.hpp"
#include "misc/BivariateFunction.hpp"
#include "misc/vector_computation.hpp"

SDL_FPoint Plot::get_graphical_mouse_position () const
{
    return mouse.position.graphical;
}

void Plot::set_graphical_mouse_position (SDL_FPoint position)
{
    mouse.position.graphical = position;
}

SDL_FPoint Plot::get_cartesian_mouse_position () const
{
    return mouse.position.cartesian;
}

void Plot::set_cartesian_mouse_position (SDL_FPoint position)
{
    mouse.position.cartesian = position;
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
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse.position.graphical = { (float)mouse_x, (float)mouse_y };

    mouse.position.cartesian = {};

    update_vector_property_matrix();
}

void Plot::update_vector_property_matrix ()
{
    VectorProperties * properties;

    SDL_FPoint
        cartesian_tail_position,
        direction_vector,
        cartesian_head_position;

    float vector_angle;

    for (size_t row = 0, column; row < SAMPLE_POINT_ROW_COUNT; row++)
    {
        for (column = 0; column < SAMPLE_POINT_COLUMN_COUNT; column++)
        {
            properties = &vector_properties_matrix[row][column];

            properties->tail = {
                    (2 * (float)column + 1) * WINDOW_WIDTH / (2 * SAMPLE_POINT_COLUMN_COUNT),
                    (2 * (float)row + 1) * WINDOW_HEIGHT / (2 * SAMPLE_POINT_ROW_COUNT)
            };

            cartesian_tail_position = graphical_to_cartesian(properties->tail);

            direction_vector = { x_function(properties->tail), y_function(properties->tail) };

            properties->magnitude = magnitude(direction_vector);

            if (properties->magnitude > 0)
            {
                properties->colour = { 0, 0, 0, 1 };

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);

    SDL_FRect p { mouse.position.graphical.x, mouse.position.graphical.y, 50, 50 };
    SDL_RenderDrawRectF(renderer, &p);

    SDL_FPoint points[SAMPLE_POINT_COLUMN_COUNT * SAMPLE_POINT_ROW_COUNT];

    int i = 0;

    for (const auto & row : vector_properties_matrix)
    {
        for (const auto & column : row)
        {
            points[i] = column.tail;

            i++;
        }
    }

    SDL_RenderDrawPointsF(renderer, points, SAMPLE_POINT_COLUMN_COUNT * SAMPLE_POINT_ROW_COUNT);

    draw_vector_field();
}

SDL_FPoint Plot::get_fixed_graphical_length_from_cartesian (SDL_FPoint point, float length, float angle) const
{
    SDL_FPoint
        graphical_position = cartesian_to_graphical(point),
        adjustment         = length * (SDL_FPoint ){ cosf(-angle), sinf(-angle) };

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