//
// Created by Russell Forrest on 18/10/2023.
//

#ifndef VECTORFIELD_OPENCL_HPP
#define VECTORFIELD_OPENCL_HPP

#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120

#include "lib/OpenCL-CLHPP/include/CL/opencl.hpp"

namespace OpenCL
{
    std::vector<cl::Device> get_gpu_devices ();

    cl::Context get_context (cl::Device const &);

    cl::Program get_program (cl::Context);

    cl::Kernel get_kernel (cl::Program const &, char const *);
};

#endif //VECTORFIELD_OPENCL_HPP
