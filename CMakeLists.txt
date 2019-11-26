cmake_minimum_required(VERSION 3.10)
project(IFJ)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic)

include_directories(src)

add_executable(IFJ
        src/main.c
        src/indent_stack.c src/indent_stack.h src/error.c src/error.h
        src/keywords.h src/keywords.c src/lex_analyzer.h src/lex_analyzer.c src/tokens.h
        src/syntax_analyzer.c src/syntax_analyzer.h src/syntax_stack.c src/syntax_stack.h src/tokens.c src/symtable.c src/symtable.h src/astnode.c src/astnode.h src/semantic_analyzer.c src/semantic_analyzer.h)
