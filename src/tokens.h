#include <stdio.h>
#include <stdlib.h>
#include "keywords.h"

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
    NIL
} TokenTypes;

typedef struct{
    TokenTypes type;
    KeywordValue keywordValue;

}Token;


