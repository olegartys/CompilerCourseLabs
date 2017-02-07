cmake_minimum_required(VERSION 3.5)
project(FSM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp IState.h ISignal.h FSMatrix.h FirstTask.h SecondTask.h ThirdTask.h)
add_executable(FSM ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC external/spdlog/include)
