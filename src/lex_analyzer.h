/*
 * IFJ projekt 2019
 * **************************
 * Tým 82, varianta II
 * **************************
 * Autoři:
 * Zdeněk Kolba (xkolba01)
 * Tomáš Oplatek (xoplat01)
 * David Rubý (xrubyd00)
 * Petr Volf (xvolfp00)
 * **************************
 * lex_analyzer.h
 */

#ifndef IFJ_LEX_ANALYZER_H
#define IFJ_LEX_ANALYZER_H

#include "tokens.h"
#include "indent_stack.h"
#include "error.h"
#include "ptr_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    IndentStack is;
    TokenStack ts;
    bool first_on_line;
    FILE* source;
} Scanner;

void scanner_init(Scanner* s, FILE* source);

void scanner_unget(Scanner* s, Token t);

int count_spaces(FILE* source);

Token get_next_token(Scanner* p);


#endif //IFJ_LEX_ANALYZER_H
