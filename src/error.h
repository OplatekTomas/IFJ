#ifndef IFJ_ERROR_H
#define IFJ_ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    LA_ERR = 1,         //chyba lexikální analýzy
    SA_ERR = 2,         //chyba syntaktické analýzy
    SEM_ERR = 3,        //sémantická chyba v programu
    TYPE_ERR = 4,       //chyba typové kompability
    PARAM_ERR = 5,      //špatný počet parametrů u volání funkce
    OTHER_ERR = 6,      //ostatní sémantické chyby
    ZERO_DIV_ERR = 9,   //chyba dělení nulou
    INTERN_ERR = 99     //interní chyba překladače
}error_type;

void throw_err(error_type et); //deklarace funkce, která oznamuje chyby

#endif
