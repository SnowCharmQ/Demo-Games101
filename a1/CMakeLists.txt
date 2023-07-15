cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(a1 main.cpp rasterizer.hpp rasterizer.cpp triangle.hpp triangle.cpp)
target_link_libraries(a1 ${OpenCV_LIBRARIES})
