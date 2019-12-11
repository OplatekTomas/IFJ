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
 * keywords.h
 */

#ifndef IFJ_KEYWORDS_H
#define IFJ_KEYWORDS_H

#include <string.h>

typedef enum {
        DEF,
        ELSE,
        IF,
        NONE,
        PASS,
        RETURN,
        WHILE,
        NON_KEYWORD
} KeywordValue;

extern const char KEYWORDS[][10];

KeywordValue is_keyword(char * input);

#endif //IFJ_KEYWORDS_H
