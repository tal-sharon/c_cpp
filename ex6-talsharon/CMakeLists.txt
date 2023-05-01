cmake_minimum_required(VERSION 3.16)
project(ex6_talsharon)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex6_talsharon
        HashMap.cpp
        HashMap.hpp
        Dictionary.hpp
        )
