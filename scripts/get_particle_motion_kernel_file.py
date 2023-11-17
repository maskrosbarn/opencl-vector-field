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

__kernel void update_particle_position_matrix
(
    __constant int      plot_width,
    __constant int      plot_height,
    __global   Vector * cartesian_particle_position_matrix_buffer,
    __global   Vector * graphical_particle_position_matrix_buffer
)
{{
    int id = get_global_id(0);
    
    something;

    Vector v = {{ 89, 8 }};

    particle_position_matrix_buffer[id] = v;
}}
"""
print(file)
stdout.write(file_contents)
