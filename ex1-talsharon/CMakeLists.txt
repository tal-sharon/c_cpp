cmake_minimum_required(VERSION 3.21)
project(ex1_talsharon C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex1_talsharon
        cipher.c
        cipher.h
        main.c
        )
