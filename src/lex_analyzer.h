//
// Created by tomas on 14.10.19.
//


#ifndef IFJ_LEX_ANALYZER_H
#define IFJ_LEX_ANALYZER_H

#include "lex_analyzer.h"
#include "tokens.h"
#include "stack.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>


int count_spaces(FILE*);

Token get_next_token(FILE*, IndentStack* is);


#endif //IFJ_LEX_ANALYZER_H
