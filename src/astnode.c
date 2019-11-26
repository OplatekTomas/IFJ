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
