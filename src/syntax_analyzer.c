#include "syntax_analyzer.h"

char token_type[][100] = {
        "ID",
        "ASSIGN",
        "GREATER",
        "LESSER",
        "GREATER_OR_EQ",
        "LESSER_OR_EQ",
        "EQ",
        "NON_EQ",
        "OPEN_PARENTHES",
        "CLOSE_PARENTHES",
        "INT",
        "BOOL",
        "FLOAT",
        "STRING",
        "KEYWORD",
        "INDENT",
        "DEDENT",
        "END_OF_FILE",
        "END_OF_LINE",
        "NIL",
        "ERROR",
        "COLON",
        "COMMA",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "DOUBLE_DIV"
};

void free_tree(ASTNode* tree) {
    // TODO: dodelat
    free(tree);
}

void node_init(ASTNode* node) {
    //node->node_type = 0;
    node->subnode_len = 0;
    node->capacity = 10;
    node->nodes = malloc(10 * sizeof(ASTNode));
}

void node_insert(ASTNode* node, ASTNode new) {
    if ((node->subnode_len + 1) > node->capacity) {
        node->nodes = realloc(node->nodes, node->capacity * 10);
        node->capacity *= 10;
    }
    node->nodes[node->subnode_len] = new;
    node->subnode_len += 1;
}

bool check_function_call(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return true;
}

bool check_expression(ASTNode* tree, Scanner* s) {
     printf("kontrola vyrazu\n");

    // TODO: dodelat
    while(true) {
         Token t = get_next_token(s);
         if (t.type == ID || t.type == FLOAT || t.type == INT || t.type == STRING || t.type == OPEN_PARENTHES || t.type == CLOSE_PARENTHES || t.type == COMMA) {

        } else {
            return false;
        }
     }
}

bool check_assignment(ASTNode* tree, Scanner* s, char* left_side) {
    //TODO: dodelat
    printf("kontrola prirazeni\n");
    if (check_expression(tree, s)) {
        return true;
    } else {
        return false;
    }
}


bool check_cond(ASTNode* tree, Scanner* s){
    if(!check_expression(tree, s)){
        return false;
    }
    Token t = get_next_token(s);
    if(t.type != GREATER && t.type != LESSER && t.type != GREATER_OR_EQ && t.type != LESSER_OR_EQ && t.type != EQ && t.type != NON_EQ ){
        return false;
    }
    if(!check_expression(tree, s)){
        return false;
    }
    return true;
}

bool check_if(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    if(!check_cond(tree, s)){
        return false;
    }
    Token t = get_next_token(s);
    if(t.type != COLON){
        return false;
    }

    return true;
}

bool check_while(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return true;
}

bool check_definition(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    Token t = get_next_token(s);
    if (t.type != ID) {

    }

    return true;
}


// TODO: definice funkce nemuze byt v ifu / whilu

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
///         3 - kdyz nastal konec souboru
///         4 - kdyz narazi na 'DEDENT'
int check_block(ASTNode* tree, Scanner* s) {
    Token t = get_next_token(s);

    printf("kontrola bloku\n");

    switch (t.type) {
        case ID:;
            char* id = t.stringValue;
            t = get_next_token(s);
            switch (t.type) {
                case ASSIGN:
                    return check_assignment(tree, s, id);
                case OPEN_PARENTHES:
                    return check_function_call(tree, s);
                default:
                    return 2;
            }
        case KEYWORD:
            switch (t.keywordValue) {
                case IF:
                    return check_if(tree, s);
                case WHILE:
                    return check_while(tree, s);
                default:
                    return 2;
            }
        case STRING:
            //TODO: Poresit multiline stringy
            return 2;
        case END_OF_FILE:
            return 3;
        case DEDENT:
            return 4;
        default:
            return 2;
    }
}

int check_root_block(ASTNode* tree, Scanner *s) {
    Token t = get_next_token(s);
    switch (t.type) {
        case KEYWORD:
            if (t.keywordValue  == DEF) {
                return check_definition(tree, s);
            }
        default:
            scanner_unget(s, t);
            return check_block(tree, s);
    }
}

/// Vraci derivacni strom, ktery je potom potreba uvolnit
ASTNode* get_derivation_tree(FILE *source) {
    Scanner s;
    scanner_init(&s, source);

    ASTNode* root = (ASTNode*)malloc(sizeof(ASTNode));
    node_init(root);
    root->node_type = PROGRAM_ROOT;

    while (true) {
        int result = check_root_block(root ,&s);
        switch (result) {
            case 1:
                fprintf(stderr, "nastala lexikalni chyba\n");
                free_tree(root);
                return NULL;
            case 2:
                fprintf(stderr, "nastala syntakticka chyba\n");
                free_tree(root);
                return NULL;
            case 3:
                break;
            default:
                continue;
        }
    }
}
