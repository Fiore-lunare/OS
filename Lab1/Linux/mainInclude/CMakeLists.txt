cmake_minimum_required(VERSION 4.0)
project(mainInclude)

set(CMAKE_CXX_STANDARD 20)

add_executable(mainInclude main.cpp
        lib/include/VectorConstants.h)

target_include_directories(mainInclude PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
target_link_directories(mainInclude PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib)
target_link_libraries(mainInclude PRIVATE Vector Number)