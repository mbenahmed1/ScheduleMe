cmake_minimum_required(VERSION 3.1)

project(scheduler)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -O3 -Wall -pedantic -Wextra -Wcast-align
        -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2
        -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs
        -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
        -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
        -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
)

#--------------------------------------------------------------------

set(scheduler_sources
        src/io.cpp 
        src/schedule_generator.cpp 
        src/resource_profile.cpp 
        include/resource_profile.hpp 
        include/neighborhoods.hpp 
        src/neighborhoods.cpp
        src/simulated_annealing.cpp
)

add_executable(solver src/solver.cpp ${scheduler_sources})
