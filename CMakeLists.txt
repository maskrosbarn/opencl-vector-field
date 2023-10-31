cmake_minimum_required(VERSION 3.26)
project(VectorField)

set(CMAKE_CXX_STANDARD 23)

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
        misc/heat_map_colouring.cpp)

target_include_directories (VectorField PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
