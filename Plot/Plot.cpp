//
// Created by Russell Forrest on 20/10/2023.
//

#include <cmath>

#include "Plot.hpp"
#include "misc/BivariateFunction.hpp"
#include "misc/vector_computation.hpp"
#include "misc/heat_map_colouring.hpp"

void Plot::update_on_next_pass ()
{
    needs_update = true;
}

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

void Plot::set_viewport_cartesian_origin (SDL_FPoint point)
{
    viewport.cartesian_origin = point;
}

SDL_FPoint Plot::get_viewport_cartesian_drag_origin () const
{
    return viewport.cartesian_drag_origin;
}

void Plot::set_viewport_cartesian_drag_origin (SDL_FPoint point)
{
    viewport.cartesian_drag_origin = point;
}

int Plot::get_viewport_range () const
{
    return viewport.range;
}

void Plot::set_viewport_range (int range)
{
    viewport.range = std::clamp(range, constants::axes::minimum_range, constants::axes::maximum_range);
}

SDL_FPoint Plot::graphical_to_cartesian (SDL_FPoint point) const
{
    return graphical_to_cartesian_space(point, viewport.cartesian_origin, viewport.range);
}

SDL_FPoint Plot::cartesian_to_graphical (SDL_FPoint point) const
{
    return cartesian_to_graphical_space(point, viewport.cartesian_origin, viewport.range);
}

bool Plot::get_axes_visibility () const
{
    return axes_are_visible;
}

void Plot::set_axes_visibility (bool is_visible)
{
    axes_are_visible = is_visible;
}

void Plot::set_is_on_window (bool is_on_window)
{
    mouse.is_on_window = is_on_window;
}

Plot::Plot (SDL_Renderer * renderer, BivariateFunction x_function, BivariateFunction y_function):
    renderer   { renderer },
    x_function { x_function },
    y_function { y_function }
{
    FC_LoadFont(
            font,
            renderer,
            constants::font::file_path,
            constants::font::size,
            constants::colour::foreground,
            TTF_STYLE_NORMAL
            );

    font_line_height = FC_GetLineHeight(font);
}

void Plot::update ()
{
    update_mouse_position();

    if (needs_update)
    {
        update_axes();

        update_vector_property_matrix();

        needs_update = false;
    }
}

SDL_FPoint Plot::get_fixed_graphical_length_from_cartesian (SDL_FPoint point, float length, float angle) const
{
    SDL_FPoint
            graphical_position = cartesian_to_graphical(point),
            adjustment         = length * (SDL_FPoint){ cosf(-angle), sinf(-angle) };

    return graphical_position + adjustment;
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

    axes_position = {
            std::clamp(axes_position.x, 0.f, constants::window_size),
            std::clamp(axes_position.y, 0.f, constants::window_size)
    };

    axes_labels.x.positive.label_value = viewport.cartesian_origin.x + (float)viewport.range * .5f;
    axes_labels.x.negative.label_value = viewport.cartesian_origin.x - (float)viewport.range * .5f;
    axes_labels.y.positive.label_value = viewport.cartesian_origin.y + (float)viewport.range * .5f;
    axes_labels.y.negative.label_value = viewport.cartesian_origin.y - (float)viewport.range * .5f;

    float
        positive_x_text_width = FC_GetWidth(font, axes_labels.text_format, axes_labels.x.positive.label_value),
        negative_x_text_width = FC_GetWidth(font, axes_labels.text_format, axes_labels.x.negative.label_value),
        positive_y_text_width = FC_GetWidth(font, axes_labels.text_format, axes_labels.y.positive.label_value),
        negative_y_text_width = FC_GetWidth(font, axes_labels.text_format, axes_labels.y.negative.label_value);

    //
    //
    //

    float x_label_y_position;

    if (axes_position.y > constants::window_size - font_line_height - constants::axes::label_margin)
    {
        x_label_y_position = axes_position.y - font_line_height - constants::axes::label_margin;
    }
    else
        x_label_y_position = axes_position.y + constants::axes::label_margin;

    axes_labels.x.positive.position = { constants::window_size - positive_x_text_width - constants::axes::label_margin, x_label_y_position };
    axes_labels.x.negative.position = { constants::axes::label_margin, x_label_y_position };

    //
    //
    //

    float y_label_x_position;

    float greatest_label_width = fmax(positive_y_text_width, negative_y_text_width);

    if (axes_position.x > constants::window_size - greatest_label_width)
        y_label_x_position = axes_position.x - greatest_label_width + constants::axes::label_margin;

    else
        y_label_x_position = axes_position.x + constants::axes::label_margin;

    axes_labels.y.positive.position = { y_label_x_position, constants::axes::label_margin };
    axes_labels.y.negative.position = { y_label_x_position, constants::window_size - constants::axes::label_margin - font_line_height };

    // fixme: how the hell can the axis label pinning be seamless?

    if (axes_position.x < negative_y_text_width + 2 * constants::axes::label_margin && axes_position.y < font_line_height + 2 * constants::axes::label_margin)
    {
        axes_labels.x.negative.position.y = font_line_height + 2 * constants::axes::label_margin;
        axes_labels.y.positive.position.x = negative_x_text_width + 2 * constants::axes::label_margin;
    }
    else if (axes_position.x > constants::window_size - positive_x_text_width - 2 * constants::axes::label_margin && axes_position.y > constants::window_size - font_line_height - 2 * constants::axes::label_margin)
    {
        axes_labels.x.positive.position.x = constants::window_size - constants::axes::label_margin - positive_y_text_width;
        axes_labels.y.negative.position.y = constants::window_size - font_line_height - 2 * constants::axes::label_margin;
    }
    else if (axes_position.x == 0 && axes_position.y == constants::window_size)
    {
        //
    }
    else if (axes_position.x == constants::window_size && axes_position.y == 0)
    {
        //
    }
}

void Plot::update_vector_property_matrix ()
{
    VectorProperties * properties;

    SDL_FPoint
        cartesian_tail_position,
        direction_vector,
        cartesian_head_position;

    float vector_angle, vector_magnitude;

    maximum_sample_point_magnitude = 0;
    minimum_sample_point_magnitude = 0;

    for (size_t row = 0; row < constants::vector_arrow::row_sample_point_count; row++)
        for (size_t column = 0; column < constants::vector_arrow::row_sample_point_count; column++)
        {
            properties = &vector_properties_matrix[row][column];

            properties->tail = {
                    std::fmaf(2, (float)column, 1) * constants::window_size / (2 * constants::vector_arrow::row_sample_point_count),
                    std::fmaf(2, (float)row, 1) * constants::window_size / (2 * constants::vector_arrow::row_sample_point_count)
            };

            cartesian_tail_position = graphical_to_cartesian(properties->tail);

            direction_vector = { x_function(cartesian_tail_position), y_function(cartesian_tail_position) };

            vector_magnitude = magnitude(direction_vector);

            properties->magnitude = vector_magnitude;

            if (vector_magnitude > maximum_sample_point_magnitude)
                maximum_sample_point_magnitude = vector_magnitude;

            else if (vector_magnitude < minimum_sample_point_magnitude)
                minimum_sample_point_magnitude = vector_magnitude;

            if (vector_magnitude > 0)
            {
                vector_angle = angle(direction_vector);

                properties->head = get_fixed_graphical_length_from_cartesian(
                        cartesian_tail_position,
                        constants::vector_arrow::body_length,
                        vector_angle
                        );

                cartesian_head_position = graphical_to_cartesian(properties->head);

                properties->head_left = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        constants::vector_arrow::head_half_width,
                        vector_angle - (float)M_PI_4
                        );

                properties->head_right = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        constants::vector_arrow::head_half_width,
                        vector_angle + (float)M_PI_4
                        );

                properties->tip = get_fixed_graphical_length_from_cartesian(
                        cartesian_head_position,
                        constants::vector_arrow::head_length,
                        vector_angle
                        );
            }
        }
}

void Plot::draw () const
{
    draw_vector_field();

    if (axes_are_visible)
        draw_axes();

    if (mouse.is_on_window)
        draw_cursor();
}

void Plot::draw_vector_field () const
{
    for (size_t row = 0; row < constants::vector_arrow::row_sample_point_count; row++)
        for (size_t column = 0; column < constants::vector_arrow::row_sample_point_count; column++)
            draw_vector(row, column);
}

void Plot::draw_vector (size_t sample_point_row, size_t sample_point_column) const
{
    VectorProperties vector_properties = vector_properties_matrix[sample_point_row][sample_point_column];

    SDL_Color colour = get_heat_map_colour(
            (vector_properties.magnitude - minimum_sample_point_magnitude) / (maximum_sample_point_magnitude - minimum_sample_point_magnitude)
            );

    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

    SDL_FPoint
            tail = vector_properties.tail,
            head = vector_properties.head,
            head_left = vector_properties.head_left,
            head_right = vector_properties.head_right,
            tip = vector_properties.tip;

    SDL_RenderDrawLineF(renderer, tail.x, tail.y, head.x, head.y);

    std::array<SDL_Vertex, 4> vertices = {
            SDL_Vertex {head,       colour, { 0 } },
            SDL_Vertex {head_left,  colour, { 0 } },
            SDL_Vertex {head_right, colour, { 0 } },
            SDL_Vertex {tip,        colour, { 0 } }
    };

    int vertex_render_indices[] {0, 1, 2, 1, 2, 3};

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), 4, vertex_render_indices, 6);
}

void Plot::draw_axes () const
{
    SDL_SetRenderDrawColor(
            renderer,
            constants::colour::foreground.r,
            constants::colour::foreground.g,
            constants::colour::foreground.b,
            constants::colour::foreground.a
            );

    SDL_RenderDrawLineF(renderer, 0, axes_position.y, constants::window_size, axes_position.y);
    SDL_RenderDrawLineF(renderer, axes_position.x, 0, axes_position.x, constants::window_size);

    FC_Draw(
            font,
            renderer,
            axes_labels.x.positive.position.x,
            axes_labels.x.positive.position.y,
            axes_labels.text_format,
            axes_labels.x.positive.label_value
            );

    FC_Draw(
            font,
            renderer,
            axes_labels.x.negative.position.x,
            axes_labels.x.negative.position.y,
            axes_labels.text_format,
            axes_labels.x.negative.label_value
    );

    FC_Draw(
            font,
            renderer,
            axes_labels.y.positive.position.x,
            axes_labels.y.positive.position.y,
            axes_labels.text_format,
            axes_labels.y.positive.label_value
    );

    FC_Draw(
            font,
            renderer,
            axes_labels.y.negative.position.x,
            axes_labels.y.negative.position.y,
            axes_labels.text_format,
            axes_labels.y.negative.label_value
    );
}

void Plot::draw_cursor () const
{
    SDL_SetRenderDrawColor(
            renderer,
            constants::colour::foreground.r,
            constants::colour::foreground.g,
            constants::colour::foreground.b,
            constants::colour::foreground.a
            );

    float text_width = FC_GetWidth(font, cursor_format, mouse.position.cartesian.x, mouse.position.cartesian.y);
    float text_height = FC_GetLineHeight(font);

    FC_Draw(
            font,
            renderer,
            fmin(mouse.position.graphical.x, constants::window_size - text_width - constants::cursor_offset) + constants::cursor_offset,
            fmin(mouse.position.graphical.y, constants::window_size - text_height - constants::cursor_offset) + constants::cursor_offset,
            cursor_format,
            mouse.position.cartesian.x,
            mouse.position.cartesian.y
            );
}