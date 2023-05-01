cmake_minimum_required(VERSION 3.16)
project(ex4_talsharon)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex4_talsharon
#        test.h
#        test.cpp
#        test_activation.h
#        test_activation.cpp
#        test_dense.h
#        test_dense.cpp
#        test_matrix.h
#        test_matrix.cpp
#        tests.cpp
        Activation.h
        Activation.cpp
        Dense.h
        Dense.cpp
        main.cpp
        Matrix.cpp
        Matrix.h
        MlpNetwork.h
        MlpNetwork.cpp)
