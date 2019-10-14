cmake_minimum_required(VERSION 3.10)
project(IFJ)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)

include_directories(src)

add_executable(IFJ
        src/main.c
        src/stack.h src/stack.c
        src/keywords.h src/keywords.c
        src/lex_analyzer.h src/lex_analyzer.c
        src/tokens.h
        src/syntax_analyzer.h src/syntax_analyzer.c
        src/error.h src/error.c
        )
