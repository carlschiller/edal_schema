cmake_minimum_required(VERSION 3.8)
project(edal_schema)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp core.h core.cpp)
add_executable(edal_schema ${SOURCE_FILES})