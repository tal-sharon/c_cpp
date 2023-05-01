cmake_minimum_required(VERSION 3.16)
project(ex2_talsharon C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex2_talsharon
        main.c
        sort_bus_lines.c
        sort_bus_lines.h
        test_bus_lines.c
        test_bus_lines.h)
