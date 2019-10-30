//
// Created by tomas on 14.10.19.
//


#ifndef IFJ_LEX_ANALYZER_H
#define IFJ_LEX_ANALYZER_H

#include "tokens.h"
#include "indent_stack.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    IndentStack is;
    bool first_on_line;
    FILE* source;
} Parser;

void parser_init(Parser* p, FILE* source);

int count_spaces(FILE* source);

Token get_next_token(Parser* p);


#endif //IFJ_LEX_ANALYZER_H
