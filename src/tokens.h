#include <stdio.h>
#include <stdlib.h>
#include "keywords.h"


//PRO DEJVA ZÁKAZ VSTUPU!


typedef enum{
    ID,
    COMP,
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
    NIL,
    ERROR,
    COLON,
    COMMA
} TokenTypes;

typedef union{
    int i;
    double d;
} NumberVal;

typedef struct{
    TokenTypes type;
    KeywordValue keywordValue;
    NumberVal numberVal;
}Token;

