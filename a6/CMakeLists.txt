cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(a6 main.cpp object.hpp vector.hpp sphere.hpp global.hpp triangle.hpp scene.cpp
scene.hpp light.hpp arealight.hpp bvh.cpp bvh.hpp bounds3.hpp ray.hpp material.hpp intersection.hpp 
renderer.hpp renderer.cpp objloader.hpp)
