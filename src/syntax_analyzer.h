#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"

typedef enum {
    PROGRAM_END,
    STATEMENT,
    EXPRESSION,
    ASSIGNMENT,
    CONDITION,
    BLOCK,
    PROGRAM_ROOT,
    IF_ELSE,
    WHILE_LOOP
} NonTerm;

struct node {
    NonTerm node_type;
    unsigned subnode_len;
    unsigned capacity;
    struct node* nodes;
};

typedef struct node ASTNode;

ASTNode* node_new();

typedef union {
    Token t;
    NonTerm n;
} Word;

typedef enum {
    TOKEN,
    NONTERM
} WordType;

ASTNode* get_derivation_tree(FILE *source);
void free_tree(ASTNode* tree);
int check_block(ASTNode* tree, Scanner* s);

#endif //IFJ_SYNTAX_ANALYZER_H
