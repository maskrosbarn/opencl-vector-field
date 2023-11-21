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

    void update (cl::size_type, SDL_FPoint, int);

    void draw () const;

private:
    SDL_Renderer * renderer;

    Plot * plot;

    cl::Device       gpu_device;
    cl::Context      opencl_context;
    cl::Program      gpu_program;
    cl::Kernel       gpu_kernel;
    cl::CommandQueue gpu_command_queue;

    cl::Buffer cartesian_positions_buffer;
    cl::Buffer graphical_positions_buffer;

    std::array<SDL_FPoint, PARTICLE_DATA_POINT_COUNT> cartesian_positions {};
    std::array<SDL_FPoint, PARTICLE_DATA_POINT_COUNT> graphical_positions {};

    static const cl::size_type opencl_buffer_size = PARTICLE_COUNT * PARTICLE_TRAIL_LENGTH * sizeof(SDL_FPoint);

    static cl::Device get_gpu_device ();
    static cl::Program create_and_build_gpu_program (cl::Context const &);
};


#endif //VECTORFIELD_PARTICLEENGINE_HPP
