__constant float DISCRETIZATION_CONSTANT = .01;

typedef struct Vector
{
    float x, y;
} Vector;

inline float x_function (Vector vector) { return @x_expression; }
inline float y_function (Vector vector) { return @y_expression; }

bool is_inside_viewport_area (Vector, Vector, int);

float get_random_float (__global float * random_number_buffer, __global bool * flag_buffer, size_t work_item_id);

Vector get_new_cartesian_position ();
Vector get_next_cartesian_position (Vector, float, float (*)(Vector), float (*)(Vector));

Vector vector_cartesian_to_graphical (Vector, Vector, int);
inline float cartesian_to_graphical_x (float, float, int);
inline float cartesian_to_graphical_y (float, float, int);

inline float vector_magnitude (Vector);
Vector vector_normalize (Vector);
Vector vector_add (Vector, Vector);
Vector vector_scalar_multiply (float, Vector);

__kernel void update_particle_data
(
    __global float * random_number_buffer,
    __global bool *  random_number_flags_buffer,

    __global Vector * cartesian_position_buffer,
    __global Vector * graphical_position_buffer,

    const int particle_count,
    const int particle_trail_length,
    
    const Vector cartesian_viewport_origin,
    const int    viewport_range,
    
    const int window_size
)
{
    int id = get_global_id(0);

    int particle_index = id * particle_trail_length;

    bool particle_component_visibility_flags[particle_trail_length];

    for (int i = 0; i < particle_trail_length; i++)
    {
        int particle_component_index = particle_index + i;

        if (i == 0)
        {
            //Vector current_cartesian_position = cartesian_position_buffer[particle_component_index];

            /*cartesian_position_buffer[particle_component_index] = get_next_position(
                current_cartesian_position,
                DISCRETIZATION_CONSTANT,
                x_function,
                y_function
                );*/
        }

        float k = get_random_float(random_number_buffer, random_number_flags_buffer, particle_index);

        Vector v = { id, k };

        graphical_position_buffer[particle_component_index] = v;
    }
}

bool is_inside_viewport_area (Vector point, Vector cartesian_viewport_origin, int viewport_range)
{
    bool x_in_range = ldexp(fabs(cartesian_viewport_origin.x - point.x), 1) <= (float)viewport_range;
    bool y_in_range = ldexp(fabs(cartesian_viewport_origin.y - point.y), 1) <= (float)viewport_range;

    return x_in_range && y_in_range;
}

float get_random_float (__global float * random_number_buffer, __global bool * flag_buffer, size_t particle_index)
{
    flag_buffer[particle_index] = true;
    return random_number_buffer[particle_index];
}

Vector get_new_cartesian_position () // fixme
{
    Vector new_position = { 0, 0 };
    return new_position;
}

Vector get_next_cartesian_position (Vector current_position, float epsilon, float (* x_function)(Vector), float (* y_function)(Vector))
{
    Vector change = { x_function(current_position), y_function(current_position) };

    return vector_add(current_position, vector_scalar_multiply(epsilon, vector_normalize(change)));
}

Vector vector_cartesian_to_graphical (Vector vector, Vector cartesian_origin, int viewport_range)
{
    //
}

float cartesian_to_graphical_x (float, float, int)
{
    //
}

float cartesian_to_graphical_y (float, float, int)
{
    //
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