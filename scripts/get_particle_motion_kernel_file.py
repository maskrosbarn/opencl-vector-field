from sys import argv, stdout

with open(argv[1], 'r') as file:
    x_expression, y_expression = file.read().splitlines()

x_expression = x_expression.replace('x', 'vector.x').replace('y', 'vector.y')
y_expression = y_expression.replace('x', 'vector.x').replace('y', 'vector.y')

file_contents = f"""\
typedef struct Vector
{{
float x, y;
}} Vector;

float x_function (Vector vector) {{ return {x_expression}; }}
float y_function (Vector vector) {{ return {y_expression}; }}

__kernel void update_particle_data
(
    __global Vector * cartesian_position_buffer,
    __global Vector * graphical_position_buffer,
    
    Vector cartesian_viewport_origin,
    int    viewport_range,
    
    int window_size
)
{{
    int id = get_global_id(0);

    Vector v = {{ 64, id + 1 }};

    graphical_position_buffer[id] = v;
}}
"""

stdout.write(file_contents)
