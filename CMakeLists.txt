cmake_minimum_required(VERSION 3.12)
project(Threads_Fun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

add_executable(Threads_Fun mergeSort.cpp)