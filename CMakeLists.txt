cmake_minimum_required(VERSION 3.10)
project(IFJ)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)

include_directories(src)

add_executable(IFJ
        src/main.c
        src/test.c
        src/test.h)
