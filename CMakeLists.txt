cmake_minimum_required(VERSION 3.6)
project(project0)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c minunit.h tests.c)
add_executable(project0 ${SOURCE_FILES})
