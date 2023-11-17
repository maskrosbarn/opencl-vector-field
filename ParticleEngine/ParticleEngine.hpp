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

    void update ();

    void draw () const;

private:
    SDL_Renderer * renderer;

    Plot * plot;

    cl::Kernel       opencl_particle_update_kernel;
    cl::CommandQueue opencl_gpu_command_queue;

    struct
    {
        cl::Buffer graphical;
        cl::Buffer cartesian;
        cl::Buffer particle_visibility;
    } opencl_buffer;

    struct
    {
        std::array<SDL_FPoint, PARTICLE_TRAIL_LENGTH * PARTICLE_COUNT> graphical {};
        std::array<SDL_FPoint, PARTICLE_TRAIL_LENGTH * PARTICLE_COUNT> cartesian {};
    } particle_positions;

    std::array<bool, PARTICLE_COUNT> particle_visibility_flags {};

    static std::vector<cl::Device> get_opencl_gpu_devices ();
    static cl::Program get_opencl_runge_kutta_program (cl::Device const &);
    //static cl::CommandQueue
};


#endif //VECTORFIELD_PARTICLEENGINE_HPP
