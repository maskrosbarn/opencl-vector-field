cmake_minimum_required(VERSION 3.26)
project(VectorField)

set(CMAKE_CXX_STANDARD 23)

add_definitions(-DOPENCL_KERNEL_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/kernels/particle_engine.cl")

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Functions)

add_executable(VectorField main.cpp
        lib/SDL_FontCache/SDL_FontCache.h
        lib/SDL_FontCache/SDL_FontCache.c
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
        ParticleEngine/ParticleEngine.hpp
        ParticleEngine/ParticleEngine.cpp)

find_package(Python3 COMPONENTS Interpreter)

add_custom_target(MainConfiguration
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/get_main_cpp_file.py ${CMAKE_CURRENT_SOURCE_DIR}/Functions > ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
)

add_custom_target(KernelConfiguration
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/kernels/particle_engine.cl
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/get_particle_engine_kernel_file.py ${CMAKE_CURRENT_SOURCE_DIR}/Functions ${CMAKE_CURRENT_SOURCE_DIR}/kernels/particle_engine.cl > ${CMAKE_CURRENT_BINARY_DIR}/kernels/particle_engine.cl
)

add_dependencies(VectorField MainConfiguration KernelConfiguration)

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