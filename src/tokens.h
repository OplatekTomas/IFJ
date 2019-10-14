#include <stdio.h>
#include <stdlib.h>


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
    NIL,
    END_OF_FILE
} token_types;

typedef struct{
    token_types type;

}token;

