cmake_minimum_required(VERSION 3.28.3)
project(mystl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mystl)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

add_executable(vector main.cpp
        shared_ptr.hpp
        mutex.hpp
        function.hpp)
