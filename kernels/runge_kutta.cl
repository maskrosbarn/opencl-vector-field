typedef struct Point
{
    float x, y;
} Point;

__kernel void test
(
    __global Point * particle_position_buffer,
    float (__global * x_function)(Point)
)
{
    int id = get_global_id(0);
    
    *particle_position_buffer = x_function(M_2_PI_F, 0);
}