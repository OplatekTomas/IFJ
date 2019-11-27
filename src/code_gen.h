//
// Created by tomas on 27.11.19.
//

#ifndef IFJ_CODE_GEN_H
#define IFJ_CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "astnode.h"
#include "symtable.h"


void GenerateCode(ASTNode* tree, SymTable *table, FILE* output);

#endif //IFJ_CODE_GEN_H
