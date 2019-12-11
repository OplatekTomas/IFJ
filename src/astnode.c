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
 * astnode.c
 */


#include "astnode.h"

ASTNode* node_new() {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    addPtr(node);
    node->node_type = PROGRAM_ROOT;
    node->subnode_len = 0;
    node->capacity = START_CAPACITY;
    node->n.i = 0;
    node->str_val = NULL;
    node->symbol = NULL;
    node->condType = OP_NONE;
    node->arith_type = TYPE_NONE;
    node->nodes = malloc(START_CAPACITY * sizeof(ASTNode*));
    addPtr(node->nodes);
    if (node->nodes == NULL) {
        return NULL;
    } else {
        return node;
    }
}

void node_insert(ASTNode* node, ASTNode* new) {
    if ((node->subnode_len + 1) > node->capacity) {
        struct node ** tmp = node->nodes;
        node->nodes = realloc(node->nodes, node->capacity * 10);
        changePtr(tmp, node->nodes);
        node->capacity *= 10;
    }
    node->nodes[node->subnode_len] = new;
    node->subnode_len += 1;
}

void free_tree(ASTNode* tree) {
    /*if (tree == NULL) {
        return;
    }
    if (tree->str_val != NULL && tree->node_type == VALUE) {
        //free(tree->str_val);
    }

    for (unsigned i = 0; i < tree->subnode_len; i++) {
        free_tree(tree->nodes[i]);
    }
    //free(tree->nodes);
    //free(tree);*/
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
                void* tmp = expressions;
                expressions = realloc(expressions, 10 * (*expr_cap) * sizeof(ASTNode*));
                changePtr(tmp, expressions);
            }
        } else {
            check_for_expr(child, expressions, expr_len, expr_cap);
        }
    }
}

ASTNode** get_all_expressions(ASTNode* root, unsigned* len) {
    unsigned expression_capacity = 20;
    ASTNode** expressions = malloc(sizeof(ASTNode*) * expression_capacity);
    addPtr(expressions);
    unsigned expressions_len = 0;

    check_for_expr(root, expressions, &expressions_len, &expression_capacity);

    (*len) = expressions_len;

    return expressions;
}


const char *node_types[100] = {
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
        "VALUE",
        "FLOAT_TO_INT",
        "FUNCTION_DEFINITION",
        "FUNCTION_CALL",
        "RETURN_VALUE",
        "KEYWORD_PASS",
        NULL
};


void goLeft(ASTNode* tree,ASTNode** resultThingy, int *counter, int *currentCap, bool isPost){
    if(!isPost){
        resultThingy[*counter] = tree;
        (*counter)++;
    }
    for(unsigned i = 0; i < tree->subnode_len; i++){
        if(*counter + 2 >= *currentCap){
            *currentCap += 100;
            void* tmp = resultThingy;
            resultThingy = realloc(resultThingy, sizeof(ASTNode*)* *currentCap);
            changePtr(tmp, resultThingy);
        }
        goLeft(tree->nodes[i],resultThingy, counter, currentCap, isPost);
    }
    if(isPost){
        resultThingy[*counter] = tree;
        (*counter)++;
    }
}

ASTNode** get_postorder(ASTNode* root, int* size){
    int cap = 100;
    ASTNode** result = malloc(sizeof(ASTNode*) * cap);
    addPtr(result);
    goLeft(root, result, size, &cap, true);
    for(int i = 0; i < *size; i++){
        fprintf(stderr,"%s ",node_types[result[i]->node_type]);
    }
    return result;
}

ASTNode** get_preorder(ASTNode* root, int* size){
    int cap = 100;
    ASTNode** result = malloc(sizeof(ASTNode*) * cap);
    addPtr(result);
    goLeft(root, result, size, &cap, false);
    for(int i = 0; i < *size; i++){
        fprintf(stderr,"%s ",node_types[result[i]->node_type]);
    }
    return result;
}

void print_node_name(ASTNode* tree, int indent){
    for(int i = 0; i < indent*2; i++){
        fprintf(stderr," ");
    }
    fprintf(stderr,"%s\n", node_types[tree->node_type]);
}
void print_tree_private(ASTNode* tree, int indent){
    indent += 1;
    print_node_name(tree, indent);
    for(unsigned int i = 0; i < tree->subnode_len; i++){
        print_tree_private(tree->nodes[i], indent);
    }
}

void print_tree(ASTNode* tree){
    fprintf(stderr,"Vypis binarniho stromu:\n");
    print_tree_private(tree, -1);
    fprintf(stderr,"Konec stromu\n");
}
