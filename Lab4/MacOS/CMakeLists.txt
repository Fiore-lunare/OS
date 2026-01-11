cmake_minimum_required(VERSION 4.0)
project(lab4MacOS)



set(CMAKE_CXX_STANDARD 20)

add_executable(killer_app killer/killer.cpp)
add_executable(user_app user/user.cpp)

add_dependencies(user_app killer_app)