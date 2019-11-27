//
// Created by petr on 26.11.19.
//

#include "astnode.h"

ASTNode* node_new() {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    node->node_type = PROGRAM_ROOT;
    node->subnode_len = 0;
    node->capacity = START_CAPACITY;
    node->n.i = 0;
    node->str_val = NULL;
    node->symbol = NULL;
    node->condType = OP_NONE;
    node->nodes = malloc(START_CAPACITY * sizeof(ASTNode*));
    if (node->nodes == NULL) {
        free(node);
        return NULL;
    } else {
        return node;
    }
}

void node_insert(ASTNode* node, ASTNode* new) {
    if ((node->subnode_len + 1) > node->capacity) {
        node->nodes = realloc(node->nodes, node->capacity * 10);
        node->capacity *= 10;
    }
    node->nodes[node->subnode_len] = new;
    node->subnode_len += 1;
}

void free_tree(ASTNode* tree) {
    if (tree == NULL) {
        return;
    }

    // TODO: dodelat
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        free_tree(tree->nodes[i]);
    }
    free(tree->nodes);
    free(tree);
}

ASTNode* node_iter_next(ASTIterator *iter) {
     return NULL;
}

bool is_num_op(NonTerm type) {
    return type == ADDITION || type == MULTIPLICATION || type == SUBTRACTION || type == DIVISION || type == INT_DIVISION;
}

void check_for_expr(ASTNode* root, ASTNode** expressions, unsigned* expr_len, unsigned* expr_cap) {
    for (unsigned i = 0; i < root->subnode_len; i++) {
        ASTNode* child = root->nodes[i];

        if (is_num_op(child->node_type)) {
            expressions[*expr_len] = child;
            (*expr_len)+=1;
            if (expr_len == expr_cap) {
                expressions = realloc(expressions, 10 * (*expr_cap) * sizeof(ASTNode*));
            }
        } else {
            check_for_expr(child, expressions, expr_len, expr_cap);
        }
    }
}

ASTNode** get_all_expressions(ASTNode* root, unsigned* len) {
    unsigned expression_capacity = 20;
    ASTNode** expressions = malloc(sizeof(ASTNode*) * expression_capacity);
    unsigned expressions_len = 0;

    check_for_expr(root, expressions, &expressions_len, &expression_capacity);

    (*len) = expressions_len;

    return expressions;
}

const char *node_types[100] = {
    "STATEMENT",
    "EXPRESSION",
    "ADDITION",
    "SUBTRACTION",
    "MULTIPLICATION",
    "DIVISION",
    "INT_DIVISION",
    "ASSIGNMENT",
    "CONDITION",
    "BLOCK",
    "PROGRAM_ROOT",
    "IDENTIFICATOR",
    "IF_ELSE",
    "WHILE_LOOP",
    "VALUE_INT",
    "VALUE_FLOAT",
    "VALUE_STRING",
    "VALUE_NONE",
    "CONVERSION",
    "FUNCTION_DEFINITION",
    "FUNCTION_CALL",
    NULL
};

void print_node_name(ASTNode* tree, int indent){
    for(int i = 0; i < indent*2; i++){
        printf(" ");
    }
    printf("%s\n", node_types[tree->node_type]);
}
void print_tree_private(ASTNode* tree, int indent){
    indent += 1;
    print_node_name(tree, indent);
    for(unsigned int i = 0; i < tree->subnode_len; i++){
        print_tree_private(tree->nodes[i], indent);
    }
}

void print_tree(ASTNode* tree){
    printf("Vypis binarniho stromu:\n");
    print_tree_private(tree, -1);
    printf("Konec stromu\n");
}
