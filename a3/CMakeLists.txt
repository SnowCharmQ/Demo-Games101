cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(a3 main.cpp rasterizer.hpp rasterizer.cpp global.hpp triangle.hpp triangle.cpp texture.hpp shader.hpp objloader.h)
target_link_libraries(a3 ${OpenCV_LIBRARIES})