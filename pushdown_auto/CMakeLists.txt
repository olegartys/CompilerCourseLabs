cmake_minimum_required(VERSION 3.5)
project(pushdown_auto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp first_task.h second_task.h)
add_executable(pushdown_auto ${SOURCE_FILES})