cmake_minimum_required(VERSION 3.9)
project(vectors C)

set(CMAKE_C_STANDARD 99)

find_package(GSL REQUIRED)

add_executable(vectors main.c)
include_directories('/usrk/include/gsl')
target_link_libraries(vectors GSL::gsl GSL::gslcblas m)


