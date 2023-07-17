cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(a5 main.cpp object.hpp vector.hpp sphere.hpp global.hpp triangle.hpp scene.hpp light.hpp renderer.hpp renderer.cpp)
target_compile_options(a5 PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -fsanitize=undefined)
target_compile_features(a5 PUBLIC cxx_std_17)
target_link_libraries(a5 PUBLIC -fsanitize=undefined)
