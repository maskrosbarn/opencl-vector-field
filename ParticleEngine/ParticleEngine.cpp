//
// Created by Russell Forrest on 01/11/2023.
//

#include <cstdio>
#include <fstream>
#include <string>

#include "ParticleEngine.hpp"
#include "OpenCL/opencl.hpp"

ParticleEngine::ParticleEngine (SDL_Renderer * renderer, Plot * plot):
    renderer { renderer },
    plot     { plot },

    gpu_device        { get_gpu_device() },
    opencl_context    { gpu_device },
    gpu_program       { create_and_build_gpu_program(opencl_context) },
    gpu_kernel        { gpu_program, "update_particle_data" },
    gpu_command_queue { opencl_context, gpu_device },

    cartesian_positions_buffer { opencl_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, opencl_buffer_size },
    graphical_positions_buffer { opencl_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, opencl_buffer_size }
{
    gpu_kernel.setArg(CARTESIAN_POSITION_BUFFER, cartesian_positions_buffer);
    gpu_kernel.setArg(GRAPHICAL_POSITION_BUFFER, graphical_positions_buffer);
    gpu_kernel.setArg(WINDOW_SIZE, WINDOW_WIDTH);
}

void ParticleEngine::update (cl::size_type particle_count, SDL_FPoint cartesian_viewport_origin, int viewport_range)
{
    gpu_kernel.setArg(CARTESIAN_VIEWPORT_ORIGIN, cartesian_viewport_origin);
    gpu_kernel.setArg(VIEWPORT_RANGE, viewport_range);

    gpu_command_queue.enqueueNDRangeKernel(gpu_kernel, cl::NullRange, cl::NDRange { particle_count });
    gpu_command_queue.enqueueReadBuffer(cartesian_positions_buffer, CL_TRUE, 0, opencl_buffer_size, cartesian_positions.data());

    gpu_command_queue.enqueueNDRangeKernel(gpu_kernel, cl::NullRange, cl::NDRange { particle_count });
    gpu_command_queue.enqueueReadBuffer(graphical_positions_buffer, CL_TRUE, 0, opencl_buffer_size, graphical_positions.data());
}

void ParticleEngine::draw () const
{
    float alpha_multiplier;

    SDL_Color colour = PARTICLE_COLOUR;

    SDL_FPoint points [PARTICLE_COUNT];

    for (int i = 0; i < PARTICLE_TRAIL_LENGTH; i++)
    {
        alpha_multiplier = 1 - (float)i / PARTICLE_TRAIL_LENGTH;

        colour.a = (Uint8)(255 * alpha_multiplier);

        for (int j = i, k = 0; j < PARTICLE_COUNT * PARTICLE_TRAIL_LENGTH; j += PARTICLE_TRAIL_LENGTH)
        {
            points[k] = graphical_positions[j];

            k++;
        }

        SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
        SDL_RenderDrawPointsF(renderer, points, PARTICLE_COUNT);
    }
}

cl::Device ParticleEngine::get_gpu_device ()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty())
    {
        std::printf("[OpenCL] No platforms found");
        exit(1);
    }

    cl::Platform platform = platforms[0];
    std::vector<cl::Device> platform_devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &platform_devices);

    if (platform_devices.empty())
    {
        std::printf("[OpenCL] No devices found");
        exit(1);
    }

    return platform_devices[0];
}

cl::Program ParticleEngine::create_and_build_gpu_program (cl::Context const & opencl_context)
{
    cl::Device gpu_device = get_gpu_device();

    std::ifstream kernel_file_data { OPENCL_KERNEL_FILE_PATH };

    std::string kernel_file_contents {
            std::istreambuf_iterator<char>(kernel_file_data),
            (std::istreambuf_iterator<char>())
    };

    cl::Program::Sources program_sources { kernel_file_contents };

    cl::Program program { opencl_context, program_sources };
    cl_int build_status = program.build();

    std::printf("[OpenCL] program build for GPU: %s;\n", gpu_device.getInfo<CL_DEVICE_NAME>().c_str());

    if (build_status != CL_BUILD_SUCCESS)
    {
        std::printf("Failed\n%s", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device).c_str());
        exit(1);
    }
    else
        std::printf("Success\n");

    return program;
}