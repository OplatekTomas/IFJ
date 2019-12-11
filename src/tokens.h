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
 * tokens.h
 */

#ifndef IFJ_TOKENS_H
#define IFJ_TOKENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "keywords.h"

//PRO DEJVA ZÁKAZ VSTUPU!

#define TOKEN_STACK_MAX_SIZE 20

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

typedef struct {
    unsigned index;
    Token data[TOKEN_STACK_MAX_SIZE];
} TokenStack;

void token_stack_init(TokenStack* ts);
void token_stack_push(TokenStack* ts, Token t);
void token_stack_pop(TokenStack* ts);
bool token_stack_empty(TokenStack* ss);
Token token_stack_top(TokenStack* ts);

#endif //IFJ_TOKENS_H
