//
// Created by Russell Forrest on 20/10/2023.
//

#include <cstdio>
#include <cstdlib>

#include <SDL.h>

#include "Application.hpp"
#include "misc/constants.hpp"
#include "misc/vector_computation.hpp"

static SDL_Window * get_window ()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::printf("%s", SDL_GetError());
        exit(1);
    }

    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);

    return SDL_CreateWindow(
            Application::name,
            display_bounds.x,
            display_bounds.y,
            constants::window_size,
            constants::window_size,
            0
            );
}

static SDL_Renderer * get_renderer (SDL_Window * window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

const char * const Application::name = "Vector Field";

Application::Application (BivariateFunction x_function, BivariateFunction y_function):

    window   { get_window() },
    renderer { get_renderer(window) },

    plot (renderer, x_function, y_function),
    particle_engine(renderer, &plot)
{
    srand(time(nullptr));

    main();
}

void Application::main ()
{
    SDL_Event event;

    do
    {
        SDL_RenderClear(renderer);

        particle_engine.update(constants::particle::count, plot.get_viewport_cartesian_origin(), plot.get_viewport_range());
        particle_engine.draw();

        plot.update();
        plot.draw();

        SDL_SetRenderDrawColor(
                renderer,
                constants::colour::background.r,
                constants::colour::background.g,
                constants::colour::background.b,
                constants::colour::background.a
                );

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event))
            did_receive_event(event);
    } while (is_running);
}

void Application::did_receive_event (SDL_Event event)
{
    switch (event.type)
    {
        case SDL_QUIT:
            is_running = false;
            break;

        case SDL_MOUSEMOTION:
            mouse_moved(event.motion);
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouse_button_down(event.button);
            break;

        case SDL_MOUSEBUTTONUP:
            mouse_button_up(event.button);
            break;

        case SDL_MOUSEWHEEL:
            mouse_wheel_moved(event.wheel);
            break;

        case SDL_KEYDOWN:
            key_pressed(event.key);
            break;

        case SDL_WINDOWEVENT:
            window_event(event.window);
            break;
    }
}

void Application::mouse_moved (SDL_MouseMotionEvent event)
{
    if (plot.mouse_has_left_button_pressed())
    {
        SDL_FPoint relative_cartesian_mouse_position = graphical_to_cartesian_space(
                plot.get_graphical_mouse_position(),
                plot.get_viewport_cartesian_drag_origin(),
                plot.get_viewport_range()
                );

        SDL_FPoint cartesian_drag_delta = plot.get_cartesian_mouse_drag_origin() - relative_cartesian_mouse_position;

        plot.set_viewport_cartesian_origin(plot.get_viewport_cartesian_drag_origin() + cartesian_drag_delta);

        plot.update_on_next_pass();
    }
}

void Application::mouse_button_down (SDL_MouseButtonEvent event)
{
    switch (event.button)
    {
        case SDL_BUTTON_LEFT:
            plot.set_mouse_left_button_pressed(true);
            plot.set_cartesian_mouse_drag_origin(plot.get_cartesian_mouse_position());
            plot.set_viewport_cartesian_drag_origin(plot.get_viewport_cartesian_origin());
            break;
    }
}

void Application::mouse_button_up (SDL_MouseButtonEvent event)
{
    switch (event.button)
    {
        case SDL_BUTTON_LEFT:
            plot.set_mouse_left_button_pressed(false);
            break;
    }
}

void Application::mouse_wheel_moved (SDL_MouseWheelEvent event)
{
    plot.set_viewport_range(plot.get_viewport_range() - (int)event.preciseY);

    plot.update_on_next_pass();
}

void Application::key_pressed (SDL_KeyboardEvent event)
{
    switch (event.keysym.sym)
    {
        case 32:
            plot.set_viewport_range(20);
            plot.set_viewport_cartesian_origin({0, 0});
            plot.update_on_next_pass();
            break;

        case 97:
            plot.set_axes_visibility(!plot.get_axes_visibility());
            break;
    }
}

void Application::window_event (SDL_WindowEvent event)
{
    switch (event.event)
    {
        case SDL_WINDOWEVENT_ENTER:
            plot.set_is_on_window(true);
            break;

        case SDL_WINDOWEVENT_LEAVE:
            plot.set_is_on_window(false);
            break;
    }
}