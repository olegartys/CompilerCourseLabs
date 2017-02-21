cmake_minimum_required(VERSION 3.5)
project(regex2NFA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp InfixTransform.cc State.cpp State.h Regex2NFA.cpp Regex2NFA.h)
add_executable(regex2NFA ${SOURCE_FILES})