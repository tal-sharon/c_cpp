cmake_minimum_required(VERSION 3.16)
project(ex3a_talsharon C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex3a_talsharon
        linked_list.c
        linked_list.h
        markov_chain.c
        markov_chain.h
        tweets_generator.c)
