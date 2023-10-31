//
// Created by Russell Forrest on 20/10/2023.
//

#include <cstdio>

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
            "Vector Field",
            display_bounds.x,
            display_bounds.y,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            0
            );
}

static SDL_Renderer * get_renderer (SDL_Window * window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

Application::Application (BivariateFunction x_function, BivariateFunction y_function):
    window   { get_window() },
    renderer { get_renderer(window) },
    plot (renderer, x_function, y_function)
{
    main();
}

void Application::main ()
{
    SDL_Event event;

    do
    {
        SDL_RenderClear(renderer);

        plot.update();

        plot.draw();

        SDL_SetRenderDrawColor(
                renderer,
                BACKGROUND_COLOUR.r,
                BACKGROUND_COLOUR.g,
                BACKGROUND_COLOUR.b,
                BACKGROUND_COLOUR.a
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