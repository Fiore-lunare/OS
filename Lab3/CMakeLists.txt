cmake_minimum_required(VERSION 4.0)
project(BufferChannel)

set(CMAKE_CXX_STANDARD 20)

add_executable(BufferChannel main.cpp
        BufferChannel.h
        MatrixMultiplication.cpp
        MatrixMultiplication.h)
