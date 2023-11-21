//
// Created by Russell Forrest on 18/10/2023.
//

#include <cstdio>
#include <vector>
#include <fstream>
#include <string>

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
}

cl::Context get_context (cl::Device const & device)
{
    return cl::Context { device };
}

cl::Program get_program (cl::Context const & context)
{
    std::ifstream kernel_file_data { OPENCL_KERNEL_FILE_PATH };

    std::string kernel_file_contents {
        std::istreambuf_iterator<char>(kernel_file_data),
        (std::istreambuf_iterator<char>())
    };

    cl::Program::Sources sources { kernel_file_contents };

    return { context, sources };
}

cl::Kernel get_kernel (cl::Program const & program, char const * name)
{
    //
}