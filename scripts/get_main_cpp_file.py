from sys import argv, stdout

with open(argv[1], 'r') as file:
    x_expression, y_expression = file.read().splitlines()

x_expression = x_expression.replace('x', 'vector.x').replace('y', 'vector.y')
y_expression = y_expression.replace('x', 'vector.x').replace('y', 'vector.y')

file_contents = f"""\
#include "Application/Application.hpp"
#include "misc/BivariateFunction.hpp"

int main ()
{{
    BivariateFunction x_function = [](SDL_FPoint vector) -> float {{ return {x_expression}; }};
    BivariateFunction y_function = [](SDL_FPoint vector) -> float {{ return {y_expression}; }};

    Application application (x_function, y_function);

    return 0;
}}
"""

stdout.write(file_contents)
