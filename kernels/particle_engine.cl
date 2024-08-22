#pragma OPENCL EXTENSION cl_intel_printf : enable

__constant long MRG32K3A_M1 = 4294967087;
__constant long MRG32K3A_M2 = 4294944443;
__constant long MRG32K3A_D  = 4294967088;

typedef enum MRG32K3A_VARIABLE_INDICES
{
    a10, a11, a12,
    a20, a21, a22,
    x10, x11, x12,
    x20, x21, x22,

    variable_count

} MRG32K3A_VARIABLE_INDICES;

__constant float DISCRETIZATION_CONSTANT = .01;
__constant float MINIMUM_MAGNITUDE       = .005;

typedef struct Vector
{
    float x, y;

} Vector;

long mod (long, long);

long next_random_long (size_t, __global long *);
float get_random_float (size_t, __global long *);

inline float x_function (Vector vector) { return @x_expression; }
inline float y_function (Vector vector) { return @y_expression; }

bool is_inside_viewport_area (Vector, Vector, int);

Vector get_new_cartesian_position (size_t, __global long *, Vector, int);

Vector vector_cartesian_to_graphical (Vector, Vector, int, int);
inline float cartesian_to_graphical_x (float, float, int, int);
inline float cartesian_to_graphical_y (float, float, int, int);

inline float vector_magnitude (Vector);

Vector vector_add (Vector, Vector);
Vector vector_scalar_multiply (float, Vector);

__kernel void update_particle_data
(
    __global long * mrg32k3a_state_variables_buffer,

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
        id,
        mrg32k3a_state_variables_buffer,
        cartesian_viewport_origin,
        viewport_range
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

long mod (long dividend, long divisor)
{
    long mod_result = dividend % divisor;

    if (mod_result < 0)
        return divisor < 0 ? mod_result - divisor : mod_result + divisor;

    return mod_result;
}

long next_random_long
(
    size_t global_id,
    __global long * mrg32k3a_state_variables_buffer
)
{
    size_t offset = global_id * variable_count;

    long x1i = mod(
            mrg32k3a_state_variables_buffer[a10 + offset] * mrg32k3a_state_variables_buffer[x10 + offset] +
            mrg32k3a_state_variables_buffer[a11 + offset] * mrg32k3a_state_variables_buffer[x11 + offset] +
            mrg32k3a_state_variables_buffer[a12 + offset] * mrg32k3a_state_variables_buffer[x12 + offset],
            MRG32K3A_M1);

    long x2i = mod(
            mrg32k3a_state_variables_buffer[a20 + offset] * mrg32k3a_state_variables_buffer[x20 + offset] +
            mrg32k3a_state_variables_buffer[a21 + offset] * mrg32k3a_state_variables_buffer[x21 + offset] +
            mrg32k3a_state_variables_buffer[a22 + offset] * mrg32k3a_state_variables_buffer[x22 + offset],
            MRG32K3A_M2);

    mrg32k3a_state_variables_buffer[x12 + offset] = mrg32k3a_state_variables_buffer[x11 + offset];
    mrg32k3a_state_variables_buffer[x11 + offset] = mrg32k3a_state_variables_buffer[x10 + offset];
    mrg32k3a_state_variables_buffer[x10 + offset] = x1i;

    mrg32k3a_state_variables_buffer[x22 + offset] = mrg32k3a_state_variables_buffer[x21 + offset];
    mrg32k3a_state_variables_buffer[x21 + offset] = mrg32k3a_state_variables_buffer[x20 + offset];
    mrg32k3a_state_variables_buffer[x20 + offset] = x2i;

    return mod(x1i - x2i, MRG32K3A_M1);
}

float get_random_float
(
    size_t global_id,
    __global long * mrg32k3a_state_variables_buffer
)
{
    long random_long = next_random_long(global_id, mrg32k3a_state_variables_buffer);

    return (float)random_long / (float)MRG32K3A_D;
}

bool is_inside_viewport_area (Vector point, Vector cartesian_viewport_origin, int viewport_range)
{
    bool x_in_range = ldexp(fabs(cartesian_viewport_origin.x - point.x), 1) <= (float)viewport_range;
    bool y_in_range = ldexp(fabs(cartesian_viewport_origin.y - point.y), 1) <= (float)viewport_range;

    return x_in_range && y_in_range;
}

Vector get_new_cartesian_position
(
    size_t global_id,
    __global long * mrg32k3a_state_variables_buffer,

    Vector cartesian_viewport_origin,

    int viewport_range
)
{
    float random_number = get_random_float(global_id, mrg32k3a_state_variables_buffer);
    float mapped_random_number;

    Vector position;

    if (random_number <= .25)
    {
        mapped_random_number = 8 * random_number - 1;

        position.x = -viewport_range * .5;
        position.y = mapped_random_number * viewport_range * .5;
    }
    else if (random_number > .25 && random_number <= .5)
    {
        mapped_random_number = 8 * random_number - 3;

        position.x = viewport_range * .5;
        position.y = mapped_random_number * viewport_range * .5;
    }
    else if (random_number > .5 && random_number <= .75)
    {
        mapped_random_number = 8 * random_number - 5;

        position.x = mapped_random_number * viewport_range * .5;
        position.y = -viewport_range * .5;
    }
    else if (random_number > .75)
    {
        mapped_random_number = 8 * random_number - 7;

        position.x = mapped_random_number * viewport_range * .5;
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