cmake_minimum_required(VERSION 4.0)
project(PipesLab5)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjectA_app ProjectA/ProjectA.cpp)
add_executable(ProjectM_app ProjectM/ProjectM.cpp)
add_executable(ProjectP_app ProjectP/ProjectP.cpp)
add_executable(ProjectS_app ProjectS/ProjectS.cpp)
add_executable(Main_app Main/main.cpp)

add_dependencies(Main_app ProjectA_app)
add_dependencies(Main_app ProjectM_app)
add_dependencies(Main_app ProjectP_app)
add_dependencies(Main_app ProjectS_app)