cmake_minimum_required(VERSION 3.8)
project(edal_schema)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fexceptions")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})


set(SOURCE_FILES main.cpp lib/core.h lib/core.cpp lib/menu.cpp lib/menu.h)
add_executable(edal_schema ${SOURCE_FILES} lib/menu.cpp lib/menu.h)
target_link_libraries(edal_schema ${CURSES_LIBRARIES})