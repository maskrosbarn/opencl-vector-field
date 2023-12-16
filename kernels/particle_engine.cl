#pragma OPENCL EXTENSION cl_intel_printf : enable

__constant float DISCRETIZATION_CONSTANT = .01;

typedef struct Vector
{
    float x, y;
} Vector;

__constant Vector NULL_VECTOR = { NAN, NAN };

inline float x_function (Vector vector) { return @x_expression; }
inline float y_function (Vector vector) { return @y_expression; }

bool is_inside_viewport_area (Vector, Vector, int);

float get_random_float (__global float *, __global bool *, size_t);


Vector get_new_cartesian_position (__global float *, __global bool *, Vector, int, size_t);


Vector get_next_cartesian_position (Vector, float, float (*)(Vector), float (*)(Vector));

Vector vector_cartesian_to_graphical (Vector, Vector, int, int);
inline float cartesian_to_graphical_x (float, float, int, int);
inline float cartesian_to_graphical_y (float, float, int, int);

inline float vector_magnitude (Vector);
Vector vector_normalize (Vector);
Vector vector_add (Vector, Vector);
Vector vector_scalar_multiply (float, Vector);

__kernel void update_particle_data
(
    __global float * random_numbers_buffer,
    __global bool *  random_number_flags_buffer,

    __global Vector * cartesian_positions_buffer,
    __global Vector * graphical_positions_buffer,

    __global bool * reset_flags_buffer,

    const int particle_count,
    const int particle_trail_length,
    
    const Vector cartesian_viewport_origin,
    const int    viewport_range,
    
    const int window_size
)
{
    size_t id = get_global_id(0);

    size_t particle_index = id * particle_trail_length;

    bool trail_is_visible = false;

    for (size_t i = particle_index; i < particle_index + particle_trail_length; i++)
    {
        if (is_inside_viewport_area(cartesian_positions_buffer[i], cartesian_viewport_origin, viewport_range))
        {
            trail_is_visible = true;
            break;
        }
    }

    Vector current_cartesian_position = cartesian_positions_buffer[particle_index];

    Vector next_cartesian_position = get_next_cartesian_position(
        current_cartesian_position,
        DISCRETIZATION_CONSTANT,
        x_function,
        y_function
    );

    Vector next_graphical_position = vector_cartesian_to_graphical(
        next_cartesian_position,
        cartesian_viewport_origin,
        viewport_range,
        window_size
    );

    if (trail_is_visible || is_inside_viewport_area(next_cartesian_position, cartesian_viewport_origin, viewport_range))
    {
        Vector previous_component_cartesian_position;

        for (size_t i = particle_index + 1; i < particle_index + particle_trail_length; i++)
        {
            Vector current_cartesian_position = cartesian_positions_buffer[i];

            cartesian_positions_buffer[i] = cartesian_positions_buffer[i - 1];

            graphical_positions_buffer[i] = vector_cartesian_to_graphical(
                cartesian_positions_buffer[i],
                cartesian_viewport_origin,
                viewport_range,
                window_size
            );

            if (current_cartesian_position.x > -1000000) {
                //printf("%.2f, %.2f\n", current_cartesian_position.x, current_cartesian_position.y);
                //break;
            }
        }

        cartesian_positions_buffer[particle_index] = next_cartesian_position;
        graphical_positions_buffer[particle_index] = next_graphical_position;
    }
    else
    {
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

        cartesian_positions_buffer[particle_index] = new_cartesian_position;
        graphical_positions_buffer[particle_index] = new_graphical_position;

        for (size_t i = particle_index + 1; i < particle_index + particle_trail_length; i++)
        {
            cartesian_positions_buffer[i] = NULL_VECTOR;
            graphical_positions_buffer[i] = NULL_VECTOR;
        }

        reset_flags_buffer[particle_index] = true;
    }

    /*for (size_t i = 0; i < 20; i++)
    {
        printf(" %.2f, %.2f |", cartesian_positions_buffer[i].x, cartesian_positions_buffer[i].y);
    }

    printf("\n");*/
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

Vector get_next_cartesian_position (Vector current_position, float epsilon, float (* x_function)(Vector), float (* y_function)(Vector))
{
    Vector change = { x_function(current_position), y_function(current_position) };

    return vector_add(current_position, vector_scalar_multiply(epsilon, change));
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

Vector vector_normalize (Vector vector)
{
    float magnitude = vector_magnitude(vector);

    Vector normalized_vector = { vector.x / magnitude, vector.y / magnitude };

    return normalized_vector;
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