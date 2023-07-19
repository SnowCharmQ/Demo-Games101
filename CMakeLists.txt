cmake_minimum_required(VERSION 3.10)
project(GAMES101)
set(CMAKE_CXX_STANDARD 17)
set(CXX_MY_FLAGS -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
    -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow
    -Wsign-promo -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option)
add_subdirectory(a0)
add_subdirectory(a1)
add_subdirectory(a2)
add_subdirectory(a3)
add_subdirectory(a5)
add_subdirectory(a6)
add_subdirectory(a7)