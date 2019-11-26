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


SSValue parse_table[9][9] = {
    //            +    -    *    /    //   (    )   ID   end
    /* + */     {'>', ' ', '<', '<', '<', '<', '>', '<', '>'},
    /* - */     {' ', '>', '<', '<', '<', '<', '>', '<', '>'},
    /* * */     {'>', '>', '>', ' ', ' ', '<', '>', '<', '>'},
    /* / */     {'>', '>', ' ', '>', ' ', '<', '>', '<', '>'},
    /* // */    {'>', '>', ' ', ' ', '>', '<', '>', '<', '>'},
    /* ( */     {'<', '<', '<', '<', '<', '<', '=', '<', ' '},
    /* ) */     {'>', '>', '>', '>', '>', ' ', '>', ' ', '>'},
    /* ID */    {'>', '>', '>', '>', '>', ' ', '>', ' ', '>'},
    /* end */   {'<', '<', '<', '<', '<', '<', ' ', '<', ' '},
};

//Pravidla pro PSA
// E => E+E
// E => E-E
// E => E*E
// E => E/E
// E => E//E
// E => (E)
// E => ID

// vraci true kdyz token je cislo, ID, string, nebo podobne
bool is_token_i(Token t) {
    return t.type == ID || t.type == INT || t.type == FLOAT || t.type == STRING;
}

int check_rule(SyntaxStack* ss) {
    SSData sd;
    sd.type = SYNTAX_EXPR;

    ASTNode* node = node_new();
    if (node == NULL) {
        return 99;
    }
    node->node_type = EXPRESSION;
    sd.node = node;

    if (ss->data[ss->index - 1].type == SYNTAX_TERM && is_token_i(ss->data[ss->index - 1].t) && ss->data[ss->index - 2].type == SYNTAX_LESSER) {
        printf("ID => E\n");
        SSData term = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        // add info
        switch (term.t.type) {
            case INT:
                sd.node->node_type = VALUE_INT;
                sd.node->n.i = term.t.numberVal.i;
                break;
            case FLOAT:
                sd.node->node_type = VALUE_FLOAT;
                sd.node->n.d = term.t.numberVal.d;
                break;
            case STRING:
                sd.node->node_type = VALUE_STRING;
                sd.node->str_val = term.t.stringValue;
                break;
            case ID:
                sd.node->node_type = IDENTIFICATOR;
                //TODO: pridat kontrolu symbolu
                break;
            default:
                // tohle by se snad stat nemelo
                return 99;
        }
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM && ss->data[ss->index - 2].t.type == ADD &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER
            ) {
        printf("E => E + E\n");
        SSData right_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        SSData left_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        sd.node->node_type = ADDITION;
        node_insert(sd.node, left_side.node);
        node_insert(sd.node, right_side.node);
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM && ss->data[ss->index - 2].t.type == MUL &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER
            ) {
        printf("E => E * E\n");
        SSData right_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        SSData left_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        sd.node->node_type = MULTIPLICATION;
        node_insert(sd.node, left_side.node);
        node_insert(sd.node, right_side.node);
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM && ss->data[ss->index - 2].t.type == SUB
            ) {
        printf("E => E - E\n");
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM && ss->data[ss->index - 2].t.type == DIV
            ) {
        printf("E => E / E\n");
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM && ss->data[ss->index - 2].t.type == DOUBLE_DIV
            ) {
        printf("E => E // E\n");
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_TERM && ss->data[ss->index - 1].t.type == CLOSE_PARENTHES &&
            ss->data[ss->index - 2].type == SYNTAX_EXPR &&
            ss->data[ss->index - 3].type == SYNTAX_TERM && ss->data[ss->index - 3].t.type == OPEN_PARENTHES &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER
            ) {
        printf("E => (E)\n");
        syntax_stack_pop(ss);
        SSData term = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_push(ss, term);
    } else {
        free_tree(node);
        return 1;
    }
    return 0;
}

int convert_token_to_table_index(SSData sd) {
    if (sd.type == SYNTAX_END) {
        return 8;
    }
    switch (sd.t.type) {
        case ADD:
            return 0;
        case SUB:
            return 1;
        case MUL:
            return 2;
        case DIV:
            return 3;
        case DOUBLE_DIV:
            return 4;
        case OPEN_PARENTHES:
            return 5;
        case CLOSE_PARENTHES:
            return 6;
        case ID:
        case INT:
        case FLOAT:
        case STRING:
            return 7;
        case END_OF_LINE:
        case END_OF_FILE:
        case COLON:
            return 8;
        default:
            return -1;
    }
}

bool check_function_call(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    printf("kontrola volani funkce\n");
    return true;
}

bool is_comp(Token t){
    return t.type == EQ || t.type == NON_EQ || t.type == GREATER_OR_EQ || t.type == LESSER_OR_EQ || t.type == GREATER || t.type == LESSER;
}

bool check_expression(ASTNode* tree, Scanner* s) {
     printf("kontrola vyrazu\n");

    // zkontrolovat jestli neni volana funkce
    Token t = get_next_token(s);
    if (t.type == ID) {
        Token par = get_next_token(s);
        if (t.type == OPEN_PARENTHES) {
            scanner_unget(s, par);
            scanner_unget(s, t);
            return check_function_call(tree, s);
        } else {
            scanner_unget(s, par);
            scanner_unget(s, t);
        }
    } else {
        scanner_unget(s, t);
    }

    SyntaxStack ss;
    syntax_stack_init(&ss);

    t = get_next_token(s);

    // TODO: dodelat
    do {
        unsigned loc;
        SSData sd = syntax_stack_nearest_term(&ss, &loc);

        int A = convert_token_to_table_index(sd);

        SSData term;
        term.type = SYNTAX_TERM;
        term.t = t;

        int B = convert_token_to_table_index(term);

        switch (parse_table[A][B]) {
            case SYNTAX_GREATER:
                if (check_rule(&ss)) {
                    return 1;
                }
                break;
            case SYNTAX_EQUAL:
                syntax_stack_push(&ss, term);
                t = get_next_token(s);
                break;
            case SYNTAX_LESSER:
                syntax_stack_shift(&ss, loc);
                syntax_stack_push(&ss, term);
                t = get_next_token(s);
                if (t.type == END_OF_LINE || t.type == END_OF_FILE || is_comp(t) || t.type == COLON) {
                    scanner_unget(s, t);
                }
                break;
            case SYNTAX_EMPTY:
            default:
                return 1;
        }
    } while (!(t.type == END_OF_LINE || t.type == END_OF_FILE  || t.type == COLON || is_comp(t)) || syntax_stack_nearest_term(&ss, NULL).type != SYNTAX_END);
    SSData result = syntax_stack_top(&ss);

    node_insert(tree, result.node);

    return 0;
}

bool check_assignment(ASTNode* tree, Scanner* s, char* left_side) {
    //TODO: dodelat
    printf("kontrola prirazeni\n");
    ASTNode* assign_node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node_init(assign_node) == NULL) {
        //TODO: spravna kontrola chyb
        return 1;
    }
    assign_node->node_type = ASSIGNMENT;

    ASTNode* id_node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node_init(id_node) == NULL) {
        //TODO: spravna kontrola chyb
        return 1;
    }
    id_node->node_type = IDENTIFICATOR;
    //TODO: pridat pointer na identifikator do tabulky symbolu
    node_insert(assign_node, id_node);

    if (check_expression(assign_node, s) == 0) {
        Token t = get_next_token(s);
        if (t.type == END_OF_LINE) {
            node_insert(tree, assign_node);
            return false;
        }
    }
    free_tree(assign_node);
    return true;
}


bool check_cond(ASTNode* tree, Scanner* s){
    //Kontrola podmínky
    ASTNode* comp = node_new();
    comp->node_type = CONDITION;
    if(check_expression(comp, s)){
        return false;
    }
    Token t = get_next_token(s);
    if(!is_comp(t)){
        return false;
    }
    if(check_expression(tree, s)){
        return false;
    }
    return true;
}

int check_keyword_helper(ASTNode* tree, Scanner* s){
    Token t = get_next_token(s);
    if(t.type != COLON){ // if x < y:
        return 2;
    }
    t = get_next_token(s);
    if(t.type != END_OF_LINE){ //if x < y: EOL
        return 2;
    }
    t = get_next_token(s);
    if(t.type != INDENT){ //if x < y: EOL a nějaké hovado by to nechalo prázdné
        return 1;
    }
    while(true){
        t = get_next_token(s);
        if(t.type == DEDENT){
            break;
        }
        scanner_unget(s, t);
        int result = check_block(tree, s);
        /*if(result != 0){
            return result;
        }*/
    }
    return 0;
}

int check_args(ASTNode* tree, Scanner* s){
    Token token = get_next_token(s);
    if(token.type != OPEN_PARENTHES)
        return 1;

    while(true){
        token = get_next_token(s);
        if(token.type != ID){
            return 1;
        }
        token = get_next_token(s);
        if(token.type != COLON){
            break;
        }
    }
    return (token.type == CLOSE_PARENTHES) ? 0 : 1;
}

int check_if(ASTNode* tree, Scanner* s) {
    printf("Kontrola ifu\n");
    ASTNode *root_node = node_new();
    root_node->node_type = IF_ELSE;
    if(!check_cond(root_node, s)){ //if x < y
        return 2;
    }
    int result = check_keyword_helper(tree, s);
    if(result != 0){
        return result;
    }
    Token t = get_next_token(s);
    printf("Kontrola else\n");
    if(t.type != KEYWORD || t.keywordValue != ELSE){
        return 2;
    }
    result = check_keyword_helper(tree, s);
    if(result != 0){
        return result;
    }
    return 0;
}

int check_while(ASTNode* tree, Scanner* s) {
    //TODO: dodelat strom
    printf("kontrola whilu\n");
    if(!check_cond(tree, s))
        return 2;
    int result = check_keyword_helper(tree, s);
    if(result != 0){
        return result;
    }
    return 0;
}

bool check_definition(ASTNode* tree, Scanner* s) {
    //TODO: dodelat strom
    //TODO: pouzit tabulku
    printf("kontrola defu\n");
    Token token = get_next_token(s);
    int result = check_args(tree, s);
    if(result != 0)
        return result;

    result = check_keyword_helper(tree, s);
    if(result != 0)
        return result;
    return 0;
}


// TODO: definice funkce nemuze byt v ifu / whilu

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
int check_block(ASTNode* tree, Scanner* s) {
    Token t = get_next_token(s);

    printf("kontrola bloku\n");

    switch (t.type) {
        case ID:;
            char* id = t.stringValue;
            Token after = get_next_token(s);
            switch (after.type) {
                case ASSIGN:
                    return check_assignment(tree, s, id);
                case OPEN_PARENTHES:
                    scanner_unget(s, after);
                    scanner_unget(s, t);
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
                case PASS:
                    break;
                case DEF:
                    return check_definition(tree,s);
                default:
                    return 2;
            }
        case STRING:
            //TODO: Poresit multiline stringy
            return 2;
        case END_OF_LINE:
            return 0;
        default:
            return 2;
    }
}

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
///         3 - kdyz nastal konec souboru
int check_root_block(ASTNode* tree, Scanner *s) {
    Token t = get_next_token(s);
    switch (t.type) {
        case KEYWORD:
            if (t.keywordValue  == DEF) {
                return check_definition(tree, s);
            } else {
                scanner_unget(s, t);
                return check_block(tree, s);
            }
        case END_OF_FILE:
            return 3;
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
    int result = 0;
    while (result != 3) {
        result = check_root_block(root ,&s);
        switch (result) {
            case 1:
                fprintf(stderr, "nastala lexikalni chyba\n");
                free_tree(root);
                return NULL;
            case 2:
                fprintf(stderr, "nastala syntakticka chyba\n");
                free_tree(root);
                return NULL;
            default:
                continue;
        }
    }
    return root;
}
