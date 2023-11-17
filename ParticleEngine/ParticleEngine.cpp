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
    plot     { plot }
{
    /*cl::Device gpu_device = OpenCL::get_gpu_devices()[0];

    std::ifstream kernel_file_contents (OPENCL_KERNEL_FILE_PATH);

    std::string source (
            std::istreambuf_iterator<char>(kernel_file_contents),
            (std::istreambuf_iterator<char>())
    );

    std::printf("%s\n", source.c_str());

    cl::Program::Sources sources { source };

    cl::Context context = cl::Context(gpu_device);

    cl::Program program = cl::Program(context, sources);

    if (program.build() != CL_BUILD_SUCCESS)
    {
        std::printf("%s", gpu_device.getInfo<CL_DEVICE_NAME>().c_str());
        std::printf("%d", program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(gpu_device));
        std::printf("%s", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device).c_str());

        exit(1);
    }

    cl::Buffer a (context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int));

    SDL_FPoint result[10];

    cl::Kernel kernel (program, "update_particle_position_matrix");
    kernel.setArg(0, a);

    cl::CommandQueue command_queue (context, gpu_device);

    for (int i = 0; i < 10; i++)
    {
        command_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1));
        command_queue.enqueueReadBuffer(a, CL_TRUE, 0, sizeof(int), particle_cartesian_positions.data());

        std::printf("%f\n", particle_cartesian_positions[0].x);
    }*/
}

void ParticleEngine::update ()
{
    //
}

void ParticleEngine::draw () const
{
    //
}

std::vector<cl::Device> ParticleEngine::get_opencl_gpu_devices ()
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

    return platform_devices;
}

cl::Program ParticleEngine::get_opencl_runge_kutta_program (cl::Device const & gpu_device)
{
    std::ifstream kernel_file (OPENCL_KERNEL_FILE_PATH);

    std::string kernel_file_contents (
            std::istreambuf_iterator<char>(kernel_file),
            (std::istreambuf_iterator<char>())
            );

    cl::Program::Sources source { kernel_file_contents };

    cl::Context context { gpu_device };

    cl::Program program { context, source };
    cl_int build_status = program.build();

    if (build_status == CL_BUILD_SUCCESS)
        std::printf(
                "Built OpenCL kernel at: %s\n\n%s\n\nfor GPU: %s\n\n",
                OPENCL_KERNEL_FILE_PATH,
                kernel_file_contents.c_str(),
                gpu_device.getInfo<CL_DEVICE_NAME>().c_str()
                );

    else
        std::printf(
                "OpenCL build failed:\n\n%s\n\n",
                program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device).c_str()
                );

    return program;
}