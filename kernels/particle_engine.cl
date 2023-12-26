#pragma OPENCL EXTENSION cl_intel_printf : enable

__constant float DISCRETIZATION_CONSTANT = .01;
__constant float MINIMUM_MAGNITUDE       = .005;

typedef struct Vector
{
    float x, y;
} Vector;

inline float x_function (Vector vector) { return @x_expression; }
inline float y_function (Vector vector) { return @y_expression; }

bool is_inside_viewport_area (Vector, Vector, int);

float get_random_float (__global float *, __global bool *, size_t);

Vector get_new_cartesian_position (__global float *, __global bool *, Vector, int, size_t);

Vector vector_cartesian_to_graphical (Vector, Vector, int, int);
inline float cartesian_to_graphical_x (float, float, int, int);
inline float cartesian_to_graphical_y (float, float, int, int);

inline float vector_magnitude (Vector);

Vector vector_add (Vector, Vector);
Vector vector_scalar_multiply (float, Vector);

__kernel void update_particle_data
(
    __global float * random_numbers_buffer,
    __global bool *  random_number_flags_buffer,

    __global Vector * cartesian_positions_buffer,
    __global Vector * graphical_positions_buffer,

    const int particle_count,

    const Vector cartesian_viewport_origin,
    const int    viewport_range,

    const int window_size
)
{
    size_t id = get_global_id(0);

    Vector current_cartesian_position = cartesian_positions_buffer[id];

    Vector position_change = {
        x_function(current_cartesian_position),
        y_function(current_cartesian_position)
    };

    if (vector_magnitude(position_change) > MINIMUM_MAGNITUDE)
    {
        Vector next_cartesian_position = vector_add(
            current_cartesian_position,
            vector_scalar_multiply(DISCRETIZATION_CONSTANT, position_change)
        );

        if (is_inside_viewport_area(next_cartesian_position, cartesian_viewport_origin, viewport_range))
        {
            Vector next_graphical_position = vector_cartesian_to_graphical(
                next_cartesian_position,
                cartesian_viewport_origin,
                viewport_range,
                window_size
            );

            cartesian_positions_buffer[id] = next_cartesian_position;
            graphical_positions_buffer[id] = next_graphical_position;

            return;
        }
    }

    Vector new_cartesian_position = get_new_cartesian_position(
        random_numbers_buffer,
        random_number_flags_buffer,
        cartesian_viewport_origin,
        viewport_range,
        id
    );

    Vector new_graphical_position = vector_cartesian_to_graphical(
        new_cartesian_position,
        cartesian_viewport_origin,
        viewport_range,
        window_size
    );

    cartesian_positions_buffer[id] = new_cartesian_position;
    graphical_positions_buffer[id] = new_graphical_position;
}

bool is_inside_viewport_area (Vector point, Vector cartesian_viewport_origin, int viewport_range)
{
    bool x_in_range = ldexp(fabs(cartesian_viewport_origin.x - point.x), 1) <= (float)viewport_range;
    bool y_in_range = ldexp(fabs(cartesian_viewport_origin.y - point.y), 1) <= (float)viewport_range;

    return x_in_range && y_in_range;
}

float get_random_float (__global float * number_buffer, __global bool * flag_buffer, size_t particle_index)
{
    flag_buffer[particle_index] = true;

    return number_buffer[particle_index];
}

Vector get_new_cartesian_position
(
    __global float * random_numbers_buffer,
    __global bool *  random_number_flags_buffer,

    Vector cartesian_viewport_origin,
    int viewport_range,

    size_t particle_index
)
{
    float seed = get_random_float(random_numbers_buffer, random_number_flags_buffer, particle_index);
    float random_number;

    Vector position;

    if (seed <= .25)
    {
        random_number = 8 * seed - 1;

        position.x = -viewport_range * .5;
        position.y = random_number * viewport_range * .5;
    }
    else if (seed > .25 && seed <= .5)
    {
        random_number = 8 * seed - 3;

        position.x = viewport_range * .5;
        position.y = random_number * viewport_range * .5;
    }
    else if (seed > .5 && seed <= .75)
    {
        random_number = 8 * seed - 5;

        position.x = random_number * viewport_range * .5;
        position.y = -viewport_range * .5;
    }
    else if (seed > .75)
    {
        random_number = 8 * seed - 7;

        position.x = random_number * viewport_range * .5;
        position.y = viewport_range * .5;
    }

    return vector_add(position, cartesian_viewport_origin);
}

Vector vector_cartesian_to_graphical (Vector vector, Vector cartesian_origin, int viewport_range, int window_size)
{
    Vector graphical_position = {
        cartesian_to_graphical_x(vector.x, cartesian_origin.x, viewport_range, window_size),
        cartesian_to_graphical_y(vector.y, cartesian_origin.y, viewport_range, window_size)
    };

    return graphical_position;
}

float cartesian_to_graphical_x (float position, float cartesian_viewport_position, int viewport_range, int window_size)
{
    float product = (position - cartesian_viewport_position) * window_size;

    return product / (float)viewport_range + .5 * window_size;
}

float cartesian_to_graphical_y (float position, float cartesian_viewport_position, int viewport_range, int window_size)
{
    float product = (position - cartesian_viewport_position) * window_size;

    return .5 * window_size - product / viewport_range;
}

inline float vector_magnitude (Vector vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

Vector vector_add (Vector lhs, Vector rhs)
{
    Vector result = { lhs.x + rhs.x, lhs.y + rhs.y };
    return result;
}

Vector vector_scalar_multiply (float scalar, Vector vector)
{
    Vector result = { scalar * vector.x, scalar * vector.y };
    return result;
}