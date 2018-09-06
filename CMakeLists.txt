cmake_minimum_required(VERSION 3.8)
project(edal_schema)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fexceptions")

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)


set(SOURCE_FILES main.cpp lib/core.h lib/core.cpp tests/ClassName.h tests/test.cpp lib/menu.cpp lib/menu.h)
add_executable(edal_schema ${SOURCE_FILES} tests/ClassName.h tests/test.cpp lib/menu.cpp lib/menu.h)
target_link_libraries(edal_schema gtest gtest_main)