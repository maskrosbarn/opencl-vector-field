#pragma OPENCL EXTENSION cl_intel_printf : enable

typedef struct Vector
{
    float x, y;
} Vector;

inline float x_function (Vector vector) { return @x_expression; }
inline float y_function (Vector vector) { return @y_expression; }

bool is_inside_viewport_area (Vector, Vector, int);
inline bool float_is_between (float, float, float);

__kernel void update_particle_data
(
    __global float * random_number_buffer,
    __global bool *  random_number_flags_buffer,

    __global Vector * cartesian_position_buffer,
    __global Vector * graphical_position_buffer,

    const int particle_trail_length,
    
    const Vector cartesian_viewport_origin,
    const int    viewport_range,
    
    const int window_size
)
{
    int id = get_global_id(0);

    Vector v = { 64 , 64 };

    graphical_position_buffer[id] = v;

    Vector point  = { .4, .4 };
    Vector origin = { 0, 0 };

    random_number_flags_buffer[id] = is_inside_viewport_area(point, origin, 1);
}

bool is_inside_viewport_area (Vector point, Vector cartesian_viewport_origin, int viewport_range)
{
    bool x_in_range = ldexpf(fabs(cartesian_viewport_origin.x - point.x), 1) <= (float)viewport_range;
    bool y_in_range = ldexpf(fabs(cartesian_viewport_origin.y - point.y), 1) <= (float)viewport_range;

    return x_in_range && y_in_range;
}

inline bool float_is_between (float n, float max, float min)
{
    return fabs(min + max - 2 * n) + min - max;
}