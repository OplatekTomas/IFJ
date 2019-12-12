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
 * error.c
 */

#include "error.h"

void throw_err(error_type et){
    switch(et){
        case LA_ERR:
            fprintf(stderr, "Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému, špatný počet mezer odsazení).\n");
            break;
        case SA_ERR:
            fprintf(stderr, "Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, neočekávaná změna úrovně odsazení).\n");
            break;
        case SEM_ERR:
            fprintf(stderr, "Sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atp.\n");
            break;
        case TYPE_ERR:
            fprintf(stderr, "Sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech\n");
            break;
        case PARAM_ERR:
            fprintf(stderr, "Sémantická chyba v programu – špatný počet parametrů u volání funkce.\n");
            break;
        case OTHER_ERR:
            fprintf(stderr, "Jiná sémantická chyba.\n");
            break;
        case ZERO_DIV_ERR:
            fprintf(stderr, "Běhová chyba dělení nulou.\n");
            break;
        case INTERN_ERR:
            fprintf(stderr, "Interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).\n");
            break;
        default:
            break;
    }
    exit(et);
}