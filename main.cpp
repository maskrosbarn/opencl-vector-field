#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120

#include "Application/Application.hpp"

#include "misc/BivariateFunction.hpp"

int main ()
{
    BivariateFunction
        x_function = [](SDL_FPoint vector) -> float {

            return vector.y;
        },
        y_function = [](SDL_FPoint vector) -> float {

            return -.2f * vector.y - (9.8f / 8) * sin(vector.x);
        };

    Application application (x_function, y_function);

    return 0;
}
