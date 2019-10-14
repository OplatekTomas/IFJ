#include <stdio.h>
#include <stdlib.h>

typedef enum{
    ID,
    COMP,
    OPEN_PARENTHES,
    CLOSE_PARENTHES,
    INT,
    BOOL,
    FLOAT,
    STRING,
    NIL
} token_types;

typedef struct{
    token_types type;

}token;


