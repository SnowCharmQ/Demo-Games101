cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(a7 main.cpp object.hpp vector.hpp sphere.hpp global.hpp triangle.hpp scene.cpp
scene.hpp light.hpp arealight.hpp bvh.cpp bvh.hpp bounds3.hpp ray.hpp material.hpp intersection.hpp
renderer.cpp renderer.hpp objloader.hpp)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_definitions(-w)
set(CMAKE_C_FLAGS_RELEASE -Ofast)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -fopenmp")