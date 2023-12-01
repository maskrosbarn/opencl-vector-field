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

    random_number_buffer       { opencl_context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_HOST_READ_ONLY, constants::particle::count * sizeof(float) },
    random_number_flags_buffer { opencl_context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_HOST_READ_ONLY, constants::particle::count * sizeof(bool) },
    cartesian_positions_buffer { opencl_context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_HOST_READ_ONLY, opencl_buffer_size },
    graphical_positions_buffer { opencl_context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_HOST_READ_ONLY, opencl_buffer_size }
{
    gpu_kernel.setArg(OpenCLKernelArguments::random_number_buffer, random_number_buffer);
    gpu_kernel.setArg(OpenCLKernelArguments::random_number_recycle_flags_buffer, random_number_flags_buffer);
    gpu_kernel.setArg(OpenCLKernelArguments::particle_cartesian_position_buffer, cartesian_positions_buffer);
    gpu_kernel.setArg(OpenCLKernelArguments::particle_graphical_position_buffer, graphical_positions_buffer);
    gpu_kernel.setArg(OpenCLKernelArguments::particle_trail_length, constants::particle::trail_length);
    gpu_kernel.setArg(OpenCLKernelArguments::window_size, constants::window_size);

    for (int i = 0; i < constants::particle::count; i++)
    {
        random_number_values[i] = 0;
        random_number_flags[i] = false;
    }
}

void ParticleEngine::update (size_t particle_count, SDL_FPoint cartesian_viewport_origin, int viewport_range)
{
    gpu_kernel.setArg(OpenCLKernelArguments::viewport_origin, cartesian_viewport_origin);
    gpu_kernel.setArg(OpenCLKernelArguments::viewport_range, viewport_range);

    gpu_command_queue.enqueueNDRangeKernel(gpu_kernel, cl::NullRange, cl::NDRange { particle_count });

    gpu_command_queue.enqueueReadBuffer(cartesian_positions_buffer, CL_TRUE, 0, opencl_buffer_size, cartesian_positions.data());
    gpu_command_queue.enqueueReadBuffer(graphical_positions_buffer, CL_TRUE, 0, opencl_buffer_size, graphical_positions.data());
    gpu_command_queue.enqueueReadBuffer(random_number_flags_buffer, CL_TRUE, 0, constants::particle::count * sizeof(bool), random_number_flags.data());

    std::printf("%d\n", random_number_flags[0]);
}

void ParticleEngine::draw () const
{
    float alpha_multiplier;

    SDL_Color colour = constants::particle::colour;

    SDL_FPoint points [constants::particle::count];

    for (int i = 0; i < constants::particle::trail_length; i++)
    {
        alpha_multiplier = 1 - (float)i / constants::particle::trail_length;

        colour.a = (Uint8)(255 * alpha_multiplier);

        for (int j = i, k = 0; j < constants::particle::count * constants::particle::trail_length; j += constants::particle::trail_length)
        {
            points[k] = graphical_positions[j];

            k++;
        }

        SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
        SDL_RenderDrawPointsF(renderer, points, constants::particle::count);
    }
}

float ParticleEngine::get_opencl_work_item_seed_value (size_t work_item_id)
{
    float seed_value = random_number_values[work_item_id];

    random_number_values[work_item_id] = (float)rand() / (float)RAND_MAX;

    return seed_value;
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