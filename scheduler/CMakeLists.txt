cmake_minimum_required(VERSION 3.1)

project(scheduler)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -Wall -pedantic -Wextra -Wcast-align
        -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
        -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
        -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
        -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
        -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
)

#--------------------------------------------------------------------

add_executable(solver src/solver.cpp src/io.cpp)
