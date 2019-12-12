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
 * keywords.c
 */

#include "keywords.h"

const char KEYWORDS[][10] = {
        "def",
        "else",
        "if",
        "None",
        "pass",
        "return",
        "while"
};

KeywordValue is_keyword(char * input) {
    if (strcmp(input, KEYWORDS[0]) == 0) {
        return DEF;
    } else if (strcmp(input, KEYWORDS[1]) == 0) {
        return ELSE;
    } else if (strcmp(input, KEYWORDS[2]) == 0) {
        return IF;
    } else if (strcmp(input, KEYWORDS[3]) == 0) {
        return NONE;
    } else if (strcmp(input, KEYWORDS[4]) == 0) {
        return PASS;
    } else if (strcmp(input, KEYWORDS[5]) == 0) {
        return RETURN;
    } else if (strcmp(input, KEYWORDS[6]) == 0) {
        return WHILE;
    } else {
        return NON_KEYWORD;
    }
}
