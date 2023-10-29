//
// Created by Russell Forrest on 20/10/2023.
//

#include <cstdio>

#include <SDL.h>

#include "Application.hpp"
#include "misc/constants.hpp"

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
    return SDL_CreateRenderer(window, -1, 0);
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
            break;
    }
}

void Application::mouse_moved (SDL_MouseMotionEvent event)
{
    //
}