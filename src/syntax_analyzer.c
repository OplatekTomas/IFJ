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

int check_block(ASTNode* tree, Scanner* s, bool is_inside_definition, SymTable** table);
int check_function_call(ASTNode* tree, Scanner* s);

const SSValue parse_table[9][9] = {
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

// 0 -> validni
// 1 -> nevalidni
// 2 -> levy int konvertuje na float
// 3 -> pravy int konvertuje na float
int check_if_valid_op(TypeValue left, TypeValue right, NonTerm op, TypeValue* op_result) {
    switch (op) {
        case ADDITION:
            if (left == right) {
                *op_result = left;
                return 0;
            } else if (left == TYPE_INT && right == TYPE_FLOAT) {
                *op_result = right;
                return 2;
            } else if (left == TYPE_FLOAT && right == TYPE_INT) {
                *op_result = left;
                return 3;
            } else {
                return 1;
            }
        case SUBTRACTION:
        case MULTIPLICATION:
            if (left == right && right != TYPE_STRING) {
                *op_result = left;
                return 0;
            } else if (left == TYPE_INT && right == TYPE_FLOAT) {
                *op_result = right;
                return 2;
            } else if (left == TYPE_FLOAT && right == TYPE_INT) {
                *op_result = left;
                return 3;
            } else {
                return 1;
            }
        case DIVISION:
            if (left == right && right != TYPE_STRING) {
                *op_result = TYPE_FLOAT;
                return 0;
            } else if (left == TYPE_INT && right == TYPE_FLOAT) {
                *op_result = right;
                return 2;
            } else if (left == TYPE_FLOAT && right == TYPE_INT) {
                *op_result = left;
                return 3;
            } else {
                return 1;
            }
        case INT_DIVISION:
            if (left == right && right == TYPE_INT) {
                *op_result = TYPE_INT;
                return 0;
            } else {
                return 1;
            }
        default:
            return 1;
    }
}

int check_rule(SyntaxStack* ss, SymTable** table) {
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
        // pop term and lessen sign
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        // add info
        switch (term.t.type) {
            case INT:
                sd.node->node_type = VALUE;
                sd.node->arith_type = TYPE_INT;
                sd.node->n.i = term.t.numberVal.i;
                break;
            case FLOAT:
                sd.node->node_type = VALUE;
                sd.node->arith_type = TYPE_FLOAT;
                sd.node->n.d = term.t.numberVal.d;
                break;
            case STRING:
                sd.node->node_type = VALUE;
                sd.node->arith_type = TYPE_STRING;
                sd.node->str_val = term.t.stringValue;
                break;
            case ID:
                sd.node->node_type = IDENTIFICATOR;
                SymTable* result = searchST(table, term.t.stringValue);
                if (result == NULL) {
                    free_tree(node);
                    return 3;
                } else {
                    sd.node->arith_type = result->type;
                    free(term.t.stringValue);
                }
                sd.node->symbol = result;
                //TODO: pridat kontrolu symbolu
                break;
            default:
                // tohle by se snad stat nemelo
                return 99;
        }
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_EXPR &&
            ss->data[ss->index - 2].type == SYNTAX_TERM &&
            ss->data[ss->index - 3].type == SYNTAX_EXPR &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER
            ) {
        // kontrola vsech aritmetickych pravidel
        printf("E => E '%i' E\n", ss->data[ss->index -2].t.type);

        TypeValue right_value = ss->data[ss->index - 1].node->arith_type;
        TypeValue left_value = ss->data[ss->index - 3].node->arith_type;

        switch (ss->data[ss->index - 2].t.type) {
            case ADD:
                sd.node->node_type = ADDITION;
                break;
            case MUL:
                sd.node->node_type = MULTIPLICATION;
                break;
            case SUB:
                sd.node->node_type = SUBTRACTION;
                break;
            case DIV:
                sd.node->node_type = DIVISION;
                break;
            case DOUBLE_DIV:
                sd.node->node_type = INT_DIVISION;
                break;
            default:
                free_tree(node);
                return 1;
        }

        TypeValue op_result;

        int result = check_if_valid_op(left_value, right_value, sd.node->node_type, &op_result);

        ASTNode *sub_node = NULL;

        switch (result) {
            case 0:
                sd.node->arith_type = op_result;
                break;
            case 1:
                free_tree(node);
                return 4;
            case 2:
            case 3:
                sd.node->arith_type = op_result;
                sub_node = node_new();
                if (sub_node == NULL) {
                    free_tree(node);
                    return 99;
                }
                sub_node->node_type = FLOAT_TO_INT;
                break;
            default:
                free_tree(node);
                return 99;
        }

        // pop left and right side, op and lesser sign
        SSData right_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        SSData left_side = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        switch (result) {
            case 0:
                node_insert(sd.node, left_side.node);
                node_insert(sd.node, right_side.node);
                break;
            case 2:
                node_insert(sub_node, left_side.node);
                node_insert(sd.node, sub_node);
                node_insert(sd.node, right_side.node);
                break;
            case 3:
                node_insert(sub_node, right_side.node);
                node_insert(sd.node, left_side.node);
                node_insert(sd.node, sub_node);
                break;
            default:
                //nemuze nastat, je kontrolovano uz predtim, leda ze by nekde flipnul bit v pameti, ale tak v tom pripade uz stejne program nefunguje spravne
                break;
        }
        syntax_stack_push(ss, sd);
    } else if (
            ss->data[ss->index - 1].type == SYNTAX_TERM && ss->data[ss->index - 1].t.type == CLOSE_PARENTHES &&
            ss->data[ss->index - 2].type == SYNTAX_EXPR &&
            ss->data[ss->index - 3].type == SYNTAX_TERM && ss->data[ss->index - 3].t.type == OPEN_PARENTHES &&
            ss->data[ss->index - 4].type == SYNTAX_LESSER
            ) {
        printf("E => (E)\n");
        // pop parenthesis and  expression
        syntax_stack_pop(ss);
        SSData term = syntax_stack_top(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        syntax_stack_push(ss, term);
    } else {
        free_tree(node);
        return 2;
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
        case EQ:
        case NON_EQ:
        case GREATER_OR_EQ:
        case LESSER_OR_EQ:
        case GREATER:
        case LESSER:
        case COMMA:
            return 8;
        default:
            return -1;
    }
}


bool is_comp(Token t, CondType* type){
    //Lehčí backwards kompatibilita.. když je mi OpType u prdele střelím tam prostě NULL a sere pes
    if(type == NULL){
        return t.type == EQ || t.type == NON_EQ || t.type == GREATER || t.type == LESSER || t.type == LESSER_OR_EQ || t.type == GREATER_OR_EQ;
    }
    switch(t.type){
        case EQ:
            *type = OP_EQ;
            break;
        case NON_EQ:
            *type = OP_NEQ;
            break;
        case GREATER:
            *type = OP_GR;
            break;
        case LESSER:
            *type = OP_LS;
            break;
        case GREATER_OR_EQ:
            *type = OP_GREQ;
            break;
        case LESSER_OR_EQ:
            *type = OP_LSEQ;
            break;
        default:
            *type = OP_NONE;
            return false;
    }
    return true;
}

int check_expression(ASTNode* tree, Scanner* s, SymTable** table) {
     printf("kontrola vyrazu\n");

    // zkontrolovat jestli neni volana funkce
    Token t = get_next_token(s);
    if (t.type == ID) {
        Token par = get_next_token(s);
        if (par.type == OPEN_PARENTHES) {
            scanner_unget(s, par);
            scanner_unget(s, t);
            return check_function_call(tree, s);
        } else if (par.type == ERROR) {
            return 1;
        } else {
            scanner_unget(s, par);
            scanner_unget(s, t);
        }
    } else {
        if (t.type == ERROR) {
            return 1;
        }
        scanner_unget(s, t);
    }

    SyntaxStack ss;
    syntax_stack_init(&ss);

    t = get_next_token(s);

    if (t.type == ERROR) {
        return 1;
    }

    // TODO: dodelat
    do {
        unsigned loc;
        SSData sd = syntax_stack_nearest_term(&ss, &loc);

        int A = convert_token_to_table_index(sd);

        SSData term;
        term.type = SYNTAX_TERM;
        term.t = t;
        term.node = NULL;

        int B = convert_token_to_table_index(term);

        switch (parse_table[A][B]) {
            case SYNTAX_GREATER:;
                int result = check_rule(&ss, table);
                if (result != 0) {
                    syntax_stack_free_nodes(&ss);
                    return result;
                }
                break;
            case SYNTAX_EQUAL:
                syntax_stack_push(&ss, term);
                t = get_next_token(s);
                if (t.type == ERROR) {
                    syntax_stack_free_nodes(&ss);
                    return 1;
                }
                break;
            case SYNTAX_LESSER:
                syntax_stack_shift(&ss, loc);
                syntax_stack_push(&ss, term);
                t = get_next_token(s);
                if (t.type == ERROR) {
                    syntax_stack_free_nodes(&ss);
                    return 1;
                }
                if (t.type == END_OF_LINE || t.type == END_OF_FILE || is_comp(t, NULL) || t.type == COMMA ||  t.type == COLON) {
                    scanner_unget(s, t);
                }
                break;
            case SYNTAX_EMPTY:
            default:
                syntax_stack_free_nodes(&ss);
                return 2;
        }
    } while (!(t.type == END_OF_LINE || t.type == COMMA || t.type == END_OF_FILE  || t.type == COLON || is_comp(t, NULL)) || syntax_stack_nearest_term(&ss, NULL).type != SYNTAX_END);
    SSData result = syntax_stack_top(&ss);

    node_insert(tree, result.node);

    return 0;
}

int check_function_call(ASTNode* tree, Scanner* s) {
    printf("kontrola volani funkce\n");
    Token t = get_next_token(s);
    if(t.type == ERROR){
        return 1;
    }
    ASTNode *root_tree = node_new();
    root_tree->str_val = t.stringValue;
    root_tree->node_type = FUNCITON_CALL;
    if((t = get_next_token(s)).type != OPEN_PARENTHES){
        free_tree(root_tree);
        return 2;
    }
    Token prev_t = t;
    t = get_next_token(s);
    while(t.type != CLOSE_PARENTHES){
        ASTNode *param = node_new();
        switch(t.type){
            case ERROR:
                free_tree(param);
                return 1;
            case ID:
                param->node_type = IDENTIFICATOR;
                param->str_val = t.stringValue;
                break;
            case NONE:
                param->node_type = VALUE;
                param->arith_type = TYPE_NONE;
                break;
            case INT:
                param->node_type = VALUE;
                param->arith_type = TYPE_INT;
                param->n.i = t.numberVal.i;
                break;
            case FLOAT:
                param->node_type = VALUE;
                param->arith_type = TYPE_FLOAT;
                param->n.d = t.numberVal.d;
                break;
            case STRING:
                param->node_type = VALUE;
                param->arith_type = TYPE_STRING;
                param->str_val = t.stringValue;
                break;
            default:
                free_tree(root_tree);
                return 2;
        }
        node_insert(root_tree, param);
        prev_t = t;
        t = get_next_token(s);
        if(t.type == CLOSE_PARENTHES){
            break;
        }
        if(t.type != COMMA){
            free_tree(root_tree);
            return 2;
        }
        prev_t = t;
        t = get_next_token(s);
    }
    if(prev_t.type == COMMA){
        free_tree(root_tree);
        return 2;
    }
    node_insert(tree, root_tree);
    return 0;
}


int check_assignment(ASTNode* tree, Scanner* s, char* left_side, SymTable** table) {
    //TODO: dodelat
    printf("kontrola prirazeni\n");
    ASTNode* assign_node = node_new();
    if (assign_node == NULL) {
        return 99;
    }
    assign_node->node_type = ASSIGNMENT;

    ASTNode* id_node = node_new();
    if (id_node == NULL) {
        return 99;
    }
    id_node->node_type = IDENTIFICATOR;
    //TODO: pridat pointer na identifikator do tabulky symbolu

    SymTable* result = searchST(table, left_side);

    if (result == NULL) {
        SymTable* new_item = allocST(left_side);
        if (new_item == NULL) {
            free_tree(assign_node);
            free_tree(id_node);
            return 99;
        }
        insertST(table, new_item);
        result = new_item;
    } else {
        free(left_side);
    }

    id_node->symbol = result;

    node_insert(assign_node, id_node);

    int expr_result = check_expression(assign_node, s, table);

    if (expr_result == 0) {
        Token t = get_next_token(s);
        if (t.type == ERROR) {
            free_tree(assign_node);
            return 1;
        }
        if (t.type == END_OF_LINE) {
            result->type = assign_node->nodes[1]->arith_type;
            node_insert(tree, assign_node);
            return 0;
        }
    }
    free_tree(assign_node);
    return expr_result;
}


bool check_cond(ASTNode* tree, Scanner* s, SymTable** table){
    //Kontrola podmínky
    CondType optype = OP_NONE;
    ASTNode* comp = node_new();
    comp->node_type = CONDITION;
    if(check_expression(comp, s, table)){
        free_tree(comp);
        return false;
    }
    Token t = get_next_token(s);
    if(t.type == ERROR){
        free_tree(comp);
        return 1;
    }
    if(!is_comp(t, &optype)){
        free_tree(comp);
        return false;
    }
    comp->condType = optype;
    if(check_expression(comp, s, table)){
        free_tree(comp);
        return false;
    }
    node_insert(tree, comp);
    return true;
}

int check_keyword_helper(ASTNode* tree, Scanner* s, bool is_inside_definition, SymTable** table){
    Token t = get_next_token(s);
    if(t.type == ERROR){
        return 1;
    }
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
    ASTNode *block_node = node_new();
    block_node->node_type = BLOCK;
    while(true){ // Read inside block
        t = get_next_token(s);
        if(t.type == ERROR){
            free_tree(block_node);
            return 1;
        }
        if(t.type == DEDENT){
            break;
        }
        scanner_unget(s, t);
        int result = check_block(block_node, s, is_inside_definition, table);
        if(result != 0){
            free_tree(block_node);
            return result;
        }
    }
    if(block_node->subnode_len == 0){
        free_tree(block_node);
        return 2;
    }
    node_insert(tree, block_node);
    return 0;
}

int check_args(ASTNode* tree, Scanner* s, SymTable* table){
    Token t = get_next_token(s);
    if(t.type == ERROR){
        return 1;
    }
    if(t.type != OPEN_PARENTHES){
        return 1;
    }
    Token prev_t = t;
    t = get_next_token(s);
    if(t.type == ERROR){
        return 1;
    }
    Arguments* argsTemp = malloc(sizeof(Arguments));
    table->args = argsTemp;
    while(t.type != CLOSE_PARENTHES){
        if(t.type != ID){
            return 2;
        }
        Arguments* args = malloc(sizeof(Arguments));
        args->id = t.stringValue;
        argsTemp->nextArg = args;
        table->argNum++;
        ASTNode *param = node_new();
        param->node_type = IDENTIFICATOR;
        node_insert(tree, param);
        prev_t = t;
        t = get_next_token(s);
        if(t.type == ERROR){
            free_tree(param);
            return 1;
        }
        if(t.type == CLOSE_PARENTHES){
            break;
        }
        if(t.type != COMMA){
            free_tree(tree);
            return 2;
        }
        prev_t = t;
        t = get_next_token(s);
        if(t.type == ERROR){
            free_tree(param);
            return 1;
        }
    }
    if(prev_t.type == COMMA){
        return 2;
    }
    Arguments * arTmp = table->args;
    table->args = arTmp->nextArg;
    table->argNum--;
    free(arTmp);
    return 0;
}

int check_if(ASTNode* tree, Scanner* s, bool is_inside_definition, SymTable** table) {
    printf("Kontrola ifu\n");
    ASTNode *root_node = node_new();
    root_node->node_type = IF_ELSE;
    if(!check_cond(root_node, s, table)){ //if x < y
        free_tree(root_node);
        return 2;
    }
    int result = check_keyword_helper(root_node, s, is_inside_definition, table);
    if(result != 0){
        free_tree(root_node);
        return result;
    }
    Token t = get_next_token(s);
    if(t.type == ERROR){
        free_tree(root_node);
        return 1;
    }
    printf("Kontrola else\n");
    if(t.type != KEYWORD || t.keywordValue != ELSE){
        free_tree(root_node);
        return 2;
    }
    result = check_keyword_helper(root_node, s, is_inside_definition, table);
    if(result != 0){
        free_tree(root_node);
        return result;
    }
    node_insert(tree, root_node);
    return 0;
}

int check_while(ASTNode* tree, Scanner* s, bool is_inside_definition, SymTable** table) {
     printf("kontrola whilu\n");
    ASTNode *while_node = node_new();
    while_node->node_type = WHILE_LOOP;
    if(!check_cond(while_node, s, table)){
        free_tree(while_node);
        return 2;
    }
    int result = check_keyword_helper(while_node, s, is_inside_definition, table);
    if(result != 0){
        free_tree(while_node);
        return result;
    }
    node_insert(tree, while_node);
    return 0;
}

int check_definition(ASTNode* tree, Scanner* s, SymTable** table) {
    //TODO: pouzit tabulku
    printf("kontrola defu\n");
    Token token = get_next_token(s);
    if(searchST(table, token.stringValue) != NULL){
        return 3;
    }
    if(token.type == ERROR){
        return 1;
    }
    ASTNode* root_tree = node_new();
    root_tree->node_type = FUNCTION_DEFINITION;
    SymTable *tb = allocST(token.stringValue);
    tb->type = TYPE_FUNCTION;
    insertST(table, tb);
    int result = check_args(root_tree, s, tb);
    if(result != 0){
        free(root_tree);
        return result;
    }
    result = check_keyword_helper(root_tree, s, true, table);
    if(result != 0){
        free_tree(root_tree);
        return result;
    }
    node_insert(tree, root_tree);
    return 0;
}

int check_return(ASTNode* tree, Scanner* s, SymTable** table){
    print_tree(tree);
    ASTNode* node = node_new();
    node->node_type = RETURN_VALUE;
    int result = check_expression(node, s, table);
    if(result != 0){
        free_tree(node);
        return result;
    }
    node_insert(tree, node);
    return 0;
}

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
int check_block(ASTNode* tree, Scanner* s, bool is_inside_function, SymTable** table) {
    Token t = get_next_token(s);

    printf("kontrola bloku\n");

    switch (t.type) {
        case ID:;
            char* id = t.stringValue;
            Token after = get_next_token(s);
            switch (after.type) {
                case ASSIGN:
                    return check_assignment(tree, s, id, table);
                case OPEN_PARENTHES:
                    scanner_unget(s, after);
                    scanner_unget(s, t);
                    return check_function_call(tree, s);
                case ERROR:
                    return 1;
                default:
                    return 2;
            }
        case KEYWORD:
            switch (t.keywordValue) {
                case IF:
                    return check_if(tree, s, is_inside_function, table);
                case WHILE:
                    return check_while(tree, s, is_inside_function, table);
                case PASS:
                    ;
                    ASTNode *pass = node_new();
                    pass->node_type = KEYWORD_PASS;
                    node_insert(tree, pass);
                    return 0;
                case DEF:
                    return check_definition(tree,s, table);
                case RETURN:
                    if(is_inside_function){
                        return check_return(tree, s, table);
                    }
                    return 2;
                default:
                    return 2;
            }
        case STRING:
            //TODO: Poresit multiline stringy
            return 2;
        case END_OF_LINE:
            return 0;
        case ERROR:
            return 1;
        default:
            return 2;
    }
}

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
///         -1 - kdyz nastal konec souboru
int check_root_block(ASTNode* tree, Scanner *s, SymTable** table) {
    Token t = get_next_token(s);
    switch (t.type) {
        case KEYWORD:
            if (t.keywordValue  == DEF) {
                return check_definition(tree, s, table);
            } else {
                scanner_unget(s, t);
                return check_block(tree, s, false, table);
            }
        case END_OF_FILE:
            return -1;
        case ERROR:
            return 1;
        default:
            scanner_unget(s, t);
            return check_block(tree, s, false, table);
    }
}

int get_derivation_tree(FILE *source, ASTNode** tree, SymTable*** table_ptr) {
    Scanner s;
    scanner_init(&s, source);

    ASTNode* root = node_new();
    if (root == NULL) {
        return 99;
    }

    SymTable** table = allocHT();

    if (table == NULL) {
        free_tree(root);
        return 99;
    }

    root->node_type = PROGRAM_ROOT;
    int result = 0;
    while (result != -1) {
        result = check_root_block(root ,&s, table);
        switch (result) {
            case -1:
            case 0:
                break;
            default:
                free_tree(root);
                freeHT(table);
                return result;
        }
    }
    *tree = root;
    *table_ptr = table;
    return 0;
}
