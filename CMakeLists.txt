cmake_minimum_required(VERSION 3.26)
project(VectorField)

set(CMAKE_CXX_STANDARD 23)

find_package(Python3 COMPONENTS Interpreter)

message("t=" ${PROJECT_SOURCE_DIR})

#execute_process(
#        WORKING_DIRECTORY /Users/forrest/CLionProjects/VectorField/
#        COMMAND ${Python3_EXECUTABLE} init.py)

add_custom_command(
        OUTPUT "${PROJECT_SOURCE_DIR}/kernels/particle_position_update.cl"
        COMMAND "${Python_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/scripts/get_opencl_kernel_file.py"
)


add_executable(VectorField main.cpp
        lib/SDL_FontCache/SDL_FontCache.h
        lib/SDL_FontCache/SDL_FontCache.c
        OpenCL/opencl.hpp
        OpenCL/opencl.cpp
        Application/Application.cpp
        Application/Application.hpp
        misc/BivariateFunction.hpp
        misc/constants.hpp
        Plot/Plot.cpp
        Plot/Plot.hpp
        misc/vector_computation.hpp
        misc/vector_computation.cpp
        misc/heat_map_colouring.hpp
        misc/heat_map_colouring.cpp
        ParticleEngine/ParticleEngine.cpp
        ParticleEngine/ParticleEngine.hpp)

target_include_directories (VectorField PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenCL REQUIRED)

include_directories(
        /Library/Frameworks/SDL2.framework/Versions/A/Headers
        /Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers
)

target_link_libraries(
        VectorField
        ${OpenCL_LIBRARY}
        /Library/Frameworks/SDL2.framework/Versions/A/SDL2
        /Library/Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf
)
