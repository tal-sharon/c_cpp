cmake_minimum_required(VERSION 3.16)
project(ex5_talsharon)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_talsharon
        Movie.cpp
        Movie.h
        RecommenderSystem.cpp
        RecommenderSystem.h
        RecommenderSystemLoader.cpp
        RecommenderSystemLoader.h
        RSUser.cpp
        RSUser.h
        RSUsersLoader.cpp
        RSUsersLoader.h)
