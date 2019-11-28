//
// Created by petr on 26.11.19.
//

#ifndef IFJ_ASTNODE_H
#define IFJ_ASTNODE_H

#include <stdlib.h>
#include "tokens.h"
#include "symtable.h"

#define START_CAPACITY 10

typedef enum {
    STATEMENT,
    EXPRESSION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    INT_DIVISION,
    ASSIGNMENT,
    CONDITION,
    BLOCK,
    PROGRAM_ROOT,
    IDENTIFICATOR,
    IF_ELSE,
    WHILE_LOOP,
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    VALUE_NONE,
    CONVERSION,
    FUNCTION_DEFINITION,
    FUNCITON_CALL,
    RETURN_VALUE
} NonTerm;

typedef enum {
    OP_NONE,
    OP_GR,
    OP_LS,
    OP_EQ,
    OP_NEQ,
    OP_GREQ,
    OP_LSEQ
} CondType;

struct node {
    NonTerm node_type;
    unsigned subnode_len;
    unsigned capacity;
    SymTable* symbol;
    NumberVal n;
    CondType condType;
    char* str_val;
    struct node** nodes;
};

typedef struct node ASTNode;

typedef struct{
    ASTNode* root;
    ASTNode** current;
} ASTIterator;

ASTNode* node_new();
void node_insert(ASTNode* node, ASTNode* new);
void free_tree(ASTNode* tree);
void print_tree(ASTNode* tree);
ASTNode** get_all_expressions(ASTNode* root, unsigned* len);
ASTNode* node_iter_next(ASTIterator* iter);
ASTNode** get_postorder(ASTNode* root);

#endif //IFJ_ASTNODE_H
