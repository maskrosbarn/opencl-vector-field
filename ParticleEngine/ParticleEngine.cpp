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

    /*
    cl::Device gpu_device = OpenCL::get_gpu_devices()[0];

    cl::Context context = cl::Context(gpu_device);

    std::ifstream kernel_file_contents (OPENCL_KERNEL_FILE_PATH);

    std::string source (
            std::istreambuf_iterator<char>(kernel_file_contents),
            (std::istreambuf_iterator<char>())
    );

    cl::Program::Sources sources { source };

    cl::Program program = cl::Program(context, sources);

    if (program.build() != CL_BUILD_SUCCESS)
    {
        std::printf("%s", gpu_device.getInfo<CL_DEVICE_NAME>().c_str());
        std::printf("%d", program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(gpu_device));
        std::printf("%s", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device).c_str());

        exit(1);
    }

    cl::Kernel kernel (program, "update_particle_position_matrix");

    cl::Buffer a (context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int));
    kernel.setArg(0, a);

    SDL_FPoint result[10];

    cl::CommandQueue command_queue (context, gpu_device);

    for (int i = 0; i < 10; i++)
    {
        command_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1));
        command_queue.enqueueReadBuffer(a, CL_TRUE, 0, sizeof(int), particle_cartesian_positions.data());

        std::printf("%f\n", particle_cartesian_positions[0].x);
    }*/
}

void ParticleEngine::update (cl::size_type particle_count, SDL_FPoint cartesian_viewport_origin, int viewport_range)
{
    gpu_kernel.setArg(CARTESIAN_VIEWPORT_ORIGIN, cartesian_viewport_origin);
    gpu_kernel.setArg(VIEWPORT_RANGE, viewport_range);

    gpu_command_queue.enqueueNDRangeKernel(gpu_kernel, cl::NullRange, cl::NDRange { particle_count });
    gpu_command_queue.enqueueReadBuffer(cartesian_positions_buffer, CL_TRUE, 0, opencl_buffer_size, cartesian_positions.data());

    gpu_command_queue.enqueueNDRangeKernel(gpu_kernel, cl::NullRange, cl::NDRange { particle_count });
    gpu_command_queue.enqueueReadBuffer(graphical_positions_buffer, CL_TRUE, 0, opencl_buffer_size, graphical_positions.data());

    std::printf("%zu : %.2f, %.2f\n", opencl_buffer_size, cartesian_positions[99].x, cartesian_positions[99].y);
}

void ParticleEngine::draw () const
{
    float alpha_multiplier;

    SDL_Color colour = PARTICLE_COLOUR;

    for (int i = 0; i < PARTICLE_TRAIL_LENGTH; i++)
    {
        alpha_multiplier = 1 - (float)i / PARTICLE_TRAIL_LENGTH;

        colour.a = (Uint8)(255 * alpha_multiplier);
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