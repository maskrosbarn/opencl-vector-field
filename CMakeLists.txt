cmake_minimum_required(VERSION 3.26)
project(VectorField)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIBRARY})

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

add_executable(VectorField main.cpp
        OpenCL/opencl.hpp
        OpenCL/opencl.cpp
        Application/Application.cpp
        Application/Application.hpp
        Misc/BivariateFunction.hpp
        Misc/Constants.hpp
        Plot/Plot.cpp
        Plot/Plot.hpp)

target_include_directories (VectorField PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (VectorField ${OpenCL_LIBRARY} ${SDL2_LIBRARY})