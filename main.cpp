

#include "Application/Application.hpp"

#include "misc/BivariateFunction.hpp"
#include "OpenCL/opencl.hpp"

#include <fstream>
#include <utility>

int main ()
{
    /*cl::Device gpu_device = OpenCL::get_gpu_devices()[0];

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

    cl::Buffer a (context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int));

    int result;

    cl::Kernel kernel (program, "test");
    kernel.setArg(0, a);

    cl::CommandQueue command_queue (context, gpu_device);

    for (int i = 0; i < 10; i++)
    {
        command_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1));
        command_queue.enqueueReadBuffer(a, CL_TRUE, 0, sizeof(int), &result);

        std::printf("%d\n", result);
    }

    return 0;*/

    BivariateFunction
        x_function = [](SDL_FPoint vector) -> float {

            return vector.y;
        },
        y_function = [](SDL_FPoint vector) -> float {

            return -.2f * vector.y - (9.8f / 8) * sin(vector.x);
        };

    Application application (x_function, y_function);

    return 0;
}
