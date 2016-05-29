cmake_minimum_required(VERSION 3.5)
project(HeapTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Heap.h)
add_executable(HeapTest ${SOURCE_FILES})