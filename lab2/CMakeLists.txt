cmake_minimum_required(VERSION 3.5)
project(regex2NFA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp InfixTransform.cpp State.cpp State.h ThompsonConstruction.cpp ThompsonConstruction.h SubsetConstruction.cpp SubsetConstruction.h)
add_executable(regex2NFA ${SOURCE_FILES})