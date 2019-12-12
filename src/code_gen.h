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
 * code_gen.h
 */

#ifndef IFJ_CODE_GEN_H
#define IFJ_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "astnode.h"
#include "symtable.h"


void generate_code(ASTNode* tree, SymTable **table);


#endif //IFJ_CODE_GEN_H
