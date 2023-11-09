//
// Created by Russell Forrest on 18/10/2023.
//

#include <cstdio>
#include <vector>

#include "OpenCL/opencl.hpp"

std::vector<cl::Device> OpenCL::get_gpu_devices ()
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

    /**
     *
    std::ifstream kernel_file_contents ("/Users/forrest/CLionProjects/VectorField/kernels/runge_kutta.cl");

    std::string source (
            std::istreambuf_iterator<char>(kernel_file_contents),
            (std::istreambuf_iterator<char>())
            );

    std::printf("%s", source.c_str());

    cl::Program::Sources sources { source };

    cl::Context context = cl::Context(gpu_device);

    cl::Program program = cl::Program(context, sources);

    if (program.build() != CL_BUILD_SUCCESS)
    {
        std::printf("%s", gpu_device.getInfo<CL_DEVICE_NAME>().c_str());
        std::printf("%d", program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(gpu_device));
        std::printf("%s", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_device).c_str());
    }
     */
}

cl::Program OpenCL::get_program (cl::Program::Sources const & source, cl::Device const & device)
{
    cl::Context context (device);
    cl::Program program (context, source);
}