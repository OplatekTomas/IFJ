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
} TokenTypes;

typedef struct{
    TokenTypes type;
    KeywordValue keywordValue;

}Token;

