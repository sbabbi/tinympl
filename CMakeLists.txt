
cmake_minimum_required(VERSION 2.6)

project(tinympl)

set (tinympl_VERSION_MAJOR 0)
set (tinympl_VERSION_MINOR 1)

include_directories(${CMAKE_SOURCE_DIR})
add_definitions(-std=c++11)
add_definitions(-Wall -pedantic)

add_executable(tinympl_test examples/main.cpp)
