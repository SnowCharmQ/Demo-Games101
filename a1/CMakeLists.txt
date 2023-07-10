cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(a1 main.cpp rasterizer.hpp rasterizer.cpp triangle.hpp triangle.cpp)
target_link_libraries(a1 ${OpenCV_LIBRARIES})
