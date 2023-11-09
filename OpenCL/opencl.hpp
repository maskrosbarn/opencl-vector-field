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

    cl::Program get_program (cl::Program::Sources const &, cl::Device const &);
};

#endif //VECTORFIELD_OPENCL_HPP
