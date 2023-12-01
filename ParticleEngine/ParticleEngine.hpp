//
// Created by Russell Forrest on 01/11/2023.
//

#ifndef VECTORFIELD_PARTICLEENGINE_HPP
#define VECTORFIELD_PARTICLEENGINE_HPP

#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120

#include <array>
#include <utility>

#include <SDL.h>
#include "lib/OpenCL-CLHPP/include/CL/opencl.hpp"

#include "Plot/Plot.hpp"
#include "misc/constants.hpp"

class ParticleEngine
{
public:
    ParticleEngine(SDL_Renderer *, Plot *);

    void update (size_t, SDL_FPoint, int);

    void draw () const;

private:
    static const cl::size_type opencl_buffer_size = constants::particle::count * constants::particle::trail_length * sizeof(SDL_FPoint);

    SDL_Renderer * renderer;

    Plot * plot;

    cl::Device       gpu_device;
    cl::Context      opencl_context;
    cl::Program      gpu_program;
    cl::Kernel       gpu_kernel;
    cl::CommandQueue gpu_command_queue;

    struct OpenCLKernelArguments
    {
        enum _
        {
            random_number_buffer,
            random_number_recycle_flags_buffer,

            particle_cartesian_position_buffer,
            particle_graphical_position_buffer,
            particle_trail_length,

            viewport_origin,
            viewport_range,

            window_size
        };
    };

    cl::Buffer random_number_buffer;
    cl::Buffer random_number_flags_buffer;
    cl::Buffer cartesian_positions_buffer;
    cl::Buffer graphical_positions_buffer;

    std::array<float, constants::particle::count> random_number_values {};
    std::array<bool, constants::particle::count>  random_number_flags  {};

    std::array<SDL_FPoint, constants::particle::data_point_count> cartesian_positions {};
    std::array<SDL_FPoint, constants::particle::data_point_count> graphical_positions {};

    float get_opencl_work_item_seed_value (size_t);

    static cl::Device get_gpu_device ();
    static cl::Program create_and_build_gpu_program (cl::Context const &);
};


#endif //VECTORFIELD_PARTICLEENGINE_HPP
