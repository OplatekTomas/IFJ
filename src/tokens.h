#include <stdio.h>
#include <stdlib.h>
#include "keywords.h"


//PRO DEJVA ZÁKAZ VSTUPU!

#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

typedef enum{
    ID,
    ASSIGN,
    GREATER,
    LESSER,
    GREATER_OR_EQ,
    LESSER_OR_EQ,
    EQ,
    NON_EQ,
    OPEN_PARENTHES,
    CLOSE_PARENTHES,
    INT,
    BOOL,
    FLOAT,
    STRING,
    KEYWORD,
    INDENT,
    DEDENT,
    END_OF_FILE,
    END_OF_LINE,
    NIL,
    ERROR,
    COLON,
    COMMA,
    ADD,
    SUB,
    MUL,
    DIV,
    DOUBLE_DIV
} TokenTypes;

typedef union{
    int i;
    double d;
} NumberVal;

typedef struct{
    TokenTypes type;
    KeywordValue keywordValue;
    NumberVal numberVal;
    char *stringValue;
}Token;

#endif //IFJ_TOKENS_H
