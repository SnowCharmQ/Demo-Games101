cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include ./include)

add_executable(a3 main.cpp rasterizer.hpp rasterizer.cpp global.hpp triangle.hpp triangle.cpp texture.hpp texture.cpp shader.hpp OBJ_Loader.h)
target_link_libraries(a3 ${OpenCV_LIBRARIES})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)