#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symtbl.h"
#include "ast.h"
#include "gen-code.h"

extern int chk_decl_flag;
extern int print_ast_flag;
extern int gen_code_flag;
extern int current_line;
int local;
int global;
SymTable* local_table;
SymTable* global_table;
char* prev;

const char* tokenStrings[] = {
    "UNDEF", "ID", "INTCON", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "COMMA",
    "SEMI", "kwINT", "kwIF", "kwELSE", "kwWHILE", "kwRETURN", "opASSG", "opADD",
    "opSUB", "opMUL", "opDIV", "opEQ", "opNE", "opGT", "opGE", "opLT", "opLE",
    "opAND", "opOR", "opNOT"
};

Token curr_tok;
int num_args_func_def = 0;
int num_local_var_func_def = 0;
int num_offset_local_var = 0;
int num_offset_formals = 4;
int num_args_func_call = 0;

/* match() checks whether the current token matches what is expected*/
void match(Token expected) {
   //printf("match\n");
    if (curr_tok == expected) {
        //printf("Current lexeme: %s\n", lexeme);
        //printf("current token %d\n", curr_tok);
        curr_tok = get_token();
    } else if (expected == EOF || curr_tok == EOF) {
        //printf("Current lexeme: %s in expect = EOF\n", lexeme);
        fprintf(stderr, "error (unbalanced '{') on line %d\n", current_line);
        exit(1);
    } else {
        fprintf(stderr, "ERROR: current lexeme %s, Expected token %s, but found %s at line %d\n", lexeme, tokenStrings[expected], tokenStrings[curr_tok], current_line);
        exit(1);
    }
}

ASTnode* opt_expr_list() {
    //printf("opt_expr_list\n");
    if (curr_tok == ID || curr_tok == INTCON) {
        return expr_list();
    } else {
        return NULL;
    }
    
    //expr_list(); 
}

void arithop() {
    if (curr_tok == opADD) {
        match(opADD);
    } else if (curr_tok == opSUB) {
        match(opSUB);
    } else if (curr_tok == opMUL) {
        match(opMUL);
    } else {
        match(opDIV);
    }
}

void logical_op() {
    if (curr_tok == opAND) {
        match(opAND);
    } else {
        match(opOR);
    }
}

ASTnode* expr_list() {
    //printf("expr_list, %s\n", lexeme);
    ASTnode* ast = malloc(sizeof(ASTnode));
    ast->ntype = EXPR_LIST;
    ast->child0 = arith_exp();
    if (curr_tok == COMMA) {
        match(COMMA);
        if (curr_tok == RPAREN) {
            match(ID);
        }
        ast->child1 = expr_list();
    }
    return ast;
}

ASTnode*  bool_exp() {
    //printf("bool_exp\n");
    ASTnode* left = arith_exp();
    ASTnode* operation = relop();
    ASTnode* right = arith_exp();
    operation->child0 = left;
    operation->child1 = right;

    if (curr_tok == opAND || curr_tok == opOR) {
       logical_op();
       bool_exp();
    }
    //printf("curr lexeme %s\n", lexeme);
    // if (curr_tok == ID || curr_tok == INTCON || curr_tok == LPAREN || curr_tok == opSUB) {
    //     //printf("Made it inside here\n");
    //     bool_exp();
    //     logical_op();
    //     bool_exp();
    // }

    return operation;
}

ASTnode* arith_exp() {
    printf("Inside arith expr %s\n", lexeme);
    ASTnode* ast = malloc(sizeof(ASTnode));
    if (curr_tok == ID) {
        ast->ntype = IDENTIFIER;
        ast->name = lexeme;
        match(ID);
        //printf("The cur lexeme %s\n", lexeme);

        if (curr_tok == LPAREN) {
            fn_call();
        } else if (curr_tok == opADD || curr_tok == opSUB || curr_tok == opMUL || curr_tok == opDIV) {
            while (curr_tok == opADD || curr_tok == opSUB || curr_tok == opMUL || curr_tok == opDIV) {
                // Handle binary arithmetic operators
                match(curr_tok); // Match the operator
                if (curr_tok == ID) {
                    match(ID);
                    if (curr_tok == RPAREN) {
                        match(RPAREN);
                    }
                } else if (curr_tok == INTCON) {
                    match(INTCON);
                    if (curr_tok == RPAREN) {
                        match(RPAREN);
                    }
                }
            }    
        }
    } else if (curr_tok == INTCON) {
        printf("Current lexeme %s\n", lexeme);
        ast->ntype = INTCONST;
        ast->num = atoi(lexeme);
        match(INTCON);
        printf("next lexeme %s\n", lexeme);

        while (curr_tok == opADD || curr_tok == opSUB || curr_tok == opMUL || curr_tok == opDIV) {
            // Handle binary arithmetic operators
            match(curr_tok); // Match the operator
            if (curr_tok == ID) {
                match(ID);
                if (curr_tok == RPAREN) {
                    match(RPAREN);
                }
            } else if (curr_tok == INTCON) {
                printf("Looskjskjsa lexeme %s\n", lexeme);
                match(INTCON);
                printf("cur_tok %s\n", lexeme);
                if (curr_tok == RPAREN) {
                    printf("Inside here\n");
                    match(RPAREN);
                    printf("You made it here babyyyyy\n");
                }
            }
        } 
    } else if (curr_tok == LPAREN) {
        match(LPAREN);
        arith_exp();
        printf("YOU ARE HERE %s\n", lexeme);
        match(RPAREN);
    } else if (curr_tok == opSUB) {
        match(opSUB);
        arith_exp();
    } else {
        fprintf(stderr, "ERROR: incomplete boolean expression on line %d\n", current_line);
        exit(1);
    }

    return ast;
}

ASTnode* relop() {
    //printf("relop\n");
    ASTnode* ast = malloc(sizeof(ASTnode));
    if (curr_tok == opEQ) {
        ast->ntype = EQ;
        match(opEQ);
        return ast;
        //arith_exp_not_empty();
    } else if (curr_tok == opNE) {
        ast->ntype = NE;
        match(opNE);
        return ast;
        //arith_exp_not_empty();
    } else if (curr_tok == opLE) {
        ast->ntype = LE;
        match(opLE);
        return ast;
        //arith_exp_not_empty();
    } else if (curr_tok == opLT) {
        ast->ntype = LT;
        match(opLT);
        return ast;
        //arith_exp_not_empty();
    } else if (curr_tok == opGE) {
        ast->ntype = GE;
        match(opGE);
        return ast;
        //arith_exp_not_empty();
    } else if (curr_tok == opGT) {
        ast->ntype = GT;
        match(opGT);
        return ast;
        //arith_exp_not_empty();
    } else {
        fprintf(stderr, "ERROR: incomplete boolean expression on line %d\n", current_line);
        exit(1);
    }
}

ASTnode* fn_call_or_assignment() {
    printf("Curr lexeme %s\n", lexeme);
    ASTnode* ast = malloc(sizeof(ASTnode));
    char* func_name_or_assg = lexeme;
    match(ID);
    // Function call
    if (curr_tok == LPAREN) {
        ast->ntype= FUNC_CALL;
        match(LPAREN);
        if (local == 1 && chk_decl_flag != 0) {
            if (symtbl_look_up(global_table, func_name_or_assg) == 0) {
                fprintf(stderr, "ERROR: Undeclared function %s called at line %d\n", lexeme, current_line);
                exit(1);
            }
        } 
        ast->name = func_name_or_assg;
        ast->st_ref = get_symtbl(global_table, ast->name, 1);
        ast->child0 = opt_expr_list();
        match(RPAREN);
        match(SEMI);

    // Assignment
    } else {
        printf("Inside assignment %s\n", lexeme);
        ast->ntype = ASSG;
        ASTnode* left = malloc(sizeof(ASTnode));
        left->ntype = IDENTIFIER;
        left->name = func_name_or_assg;
        // Look in local scope first
        left->st_ref = get_symtbl(local_table, left->name, 0);

        match(opASSG);
        printf("YOUUAUAUAUAUUAU CU\n");
        ASTnode* right = arith_exp();
        printf("YOUUAUAUAUAUUAU CU\n");
        ast->child0 = left;
        ast->child1 = right;
        match(SEMI);
    }
    return ast;
} 

void fn_call() {
    match(ID);
    match(LPAREN);
    opt_expr_list();
    match(RPAREN);
    match(SEMI);
}

ASTnode* stmt() {
    printf("stmt %s\n", lexeme);
    ASTnode* node = malloc(sizeof(ASTnode));
    node->ntype = STMT_LIST;
    if (curr_tok == kwWHILE) {
        node->child0 = while_stmt();
        return node;
    } else if (curr_tok == kwIF) {
        node->child0 = if_stmt();
        return node;
    } else if (curr_tok == ID) {
        node->child0 = fn_call_or_assignment();
        return node;
    } else if (curr_tok == kwRETURN) {
        node->child0 = return_stmt();
        return node;
    } else if (curr_tok == LBRACE) {
        match(LBRACE);
        node->child0 = opt_stmt_list();
        match(RBRACE);
        return node; 
    } else {
        match(SEMI);
        return node;
    }
    return NULL;
}

ASTnode* if_stmt() {
    //printf("if_stmt\n");
    ASTnode* ast = malloc(sizeof(ASTnode));
    ast->ntype = IF;
    match(kwIF);
    match(LPAREN);

    if (curr_tok == RPAREN) {
        fprintf(stderr, "ERROR: Missing condition inside if statement ON LINE %d\n", current_line);
        exit(1);
    } else {
        ast->child0 = bool_exp();
    }

    match(RPAREN);
    ast->child1 = stmt();

    if (curr_tok == kwELSE) {
        match(kwELSE);
        ast->child2 = stmt();
    }
    return ast;
}

ASTnode* while_stmt() {
    //printf("while_stmt\n");
    ASTnode* ast = malloc(sizeof(ASTnode));
    ast->ntype = WHILE;
    match(kwWHILE);
    match(LPAREN);
    ast->child0 = bool_exp();
    match(RPAREN);
    ast->child1 = stmt();
    return ast;
}

ASTnode* return_stmt() {
    //printf("return_stmt\n");
    ASTnode* ast = malloc(sizeof(ASTnode));
    ast->ntype = RETURN;
    match(kwRETURN);
    if (curr_tok == SEMI) {
        match(SEMI);
    } else {
        ast->child0 = arith_exp();
        match(SEMI);
    }
    return ast;
}

void assg_stmt() {
    match(ID);
    match(opASSG);
    arith_exp();
    match(SEMI);
}

void type() {
    //printf("type\n");
    match(kwINT);
}

ASTnode* id_list() {
    printf("id_list lexeme %s\n", lexeme);
    if (curr_tok == ID) {
        num_offset_local_var = num_offset_local_var - 4;
        //printf("************ lexeme %s, offset %d\n", lexeme, num_offset_local_var);
        symtbl_add(lexeme, 0, 0, 0, num_offset_local_var, 0, 0); // add variable local scope
        match(ID);

        while (curr_tok == COMMA) {
            match(COMMA);
            if (curr_tok == ID) {
                num_offset_local_var = num_offset_local_var - 4;
                //printf("************ lexeme %s, offset %d\n", lexeme, num_offset_local_var);
                symtbl_add(lexeme, 0, 0, 0, num_offset_local_var, 0, 0); // add variable to local or global scope
                match(ID);
            }
        }
    }
    return NULL;
}

ASTnode* var_decl() { 
    //printf("var_decl %s\n", lexeme);
    type();
    id_list(); 
    match(SEMI);
    return NULL;
}

ASTnode* formals() {
    //printf("formals\n");
    if (curr_tok == kwINT) {
        num_args_func_def++;
        type();
        ASTnode* ast = malloc(sizeof(ASTnode));
        ast->ntype = IDENTIFIER;
        ast->name = lexeme;
        num_offset_formals = num_offset_formals + 4;
        //printf("ast->name %s, offset %d", ast->name, num_offset_formals);
        ast->st_ref = symtbl_add(lexeme, 0, 0, 0, 0, num_offset_formals, 0);
        match(ID);
        //formals();
        if (curr_tok == COMMA) {
            match(COMMA);
            if (curr_tok == RPAREN) {
                match(ID);
            }
            ast->child0 = formals();
        } 
        return ast;
    }

    return NULL;      
}

ASTnode* opt_formals() {
    //printf("opt_formals\n");
    if (curr_tok == kwINT) {
        return formals();
    }

    return NULL;
}

ASTnode* opt_var_decls() {
    //printf("opt_var_decls\n");
    if (curr_tok == kwINT) {
        var_decl();
        opt_var_decls();
    } else if (curr_tok == COMMA) {
        match(COMMA);
        var_decl();
        opt_var_decls();
    }
    return NULL;
}

ASTnode* opt_stmt_list() {
    //printf("opt_stmt_list lexeme %s\n", lexeme);
    ASTnode* current = NULL;
    ASTnode* prev = NULL;

    //ASTnode* prev = NULL;
    while (curr_tok == ID || curr_tok == kwWHILE || curr_tok ==
     kwIF || curr_tok == kwRETURN || curr_tok == LBRACE || curr_tok == SEMI) {
        ASTnode* curr_stmt = stmt();
        //printf("%s \n", curr_stmt->child0->name);
        if (current == NULL) {
            current = curr_stmt;
            prev = curr_stmt;
        } else {
            prev->child1 = curr_stmt;
            prev = curr_stmt;
        }
    } 

    return current;
}

void func_defn() {
    //printf("func_defn\n");
    ASTnode* ast = malloc(sizeof(ASTnode));
    ast->ntype = FUNC_DEF;
    ast->st_ref = symtbl_add(prev, 1, 0, 0, 0, 0, 0); // 1 for function (global scope)
    ast->name = prev;
    match(LPAREN);
    set_local_scope(); 
    ast->child1 = opt_formals();
    ast->num = num_args_func_def;
    ast->st_ref->args = num_args_func_def;
    num_args_func_def = 0;
    match(RPAREN);
    match(LBRACE);
    opt_var_decls();
    ast->child0 = opt_stmt_list();
    //printf("next lexeme %s\n", lexeme);
    match(RBRACE);
    num_local_var_func_def = count_table(local_table) - num_args_func_def;
    ast->st_ref->local_var_num = num_local_var_func_def;

    // Print the AST if requested
    if (print_ast_flag) {
        print_ast(ast);
    }

    // Generate code
    if (gen_code_flag) {
        gen_code(ast);
    }

    set_global_scope();
    num_local_var_func_def = 0;
    num_offset_formals = 4;
    num_offset_local_var = -4;
}

void var_decl_prog() {
    if (curr_tok == SEMI) { // global scope
        if (gen_code_flag) {
            printf("_%s: .space 4\n", prev);
        }
        symtbl_add(prev, 0, 0, 0, 0, 0, 1);
        match(SEMI);
    } else if (curr_tok == COMMA) { // global
        if (gen_code_flag) {
            printf("_%s: .space 4\n", prev);
        }
        symtbl_add(prev, 0, 0, 0, 0, 0, 1); 
        match(COMMA);
        prev = lexeme;
        match(ID);
        var_decl_prog();
        //id_list_var_decl_prog();
        //match(SEMI);
    } else if (curr_tok == opASSG) {
        symtbl_add(prev, 0, 0, 0, 0, 0, 1);
        assg_stmt();
    } 
}

void prog() {
    while (curr_tok == kwINT) {
        //printf("prog\n");
        type();
        prev = lexeme;
        //printf("%s\n", lexeme);
        match(ID);
        
        if (curr_tok == SEMI || curr_tok == COMMA || curr_tok == opASSG) { // global scope
            // check variable
            if (gen_code_flag) {
                printf(".data\n");
            }
            var_decl_prog();
        } else {
            // check function
            func_defn();
        }
    }
}

/* parse() - top-level parse routine */
int parse() {
    // Write assembly output to output.s 
    //FILE *filePtr = freopen("output.s", "w", stdout);

    curr_tok = get_token();
    set_global_scope();
    if (gen_code_flag) {
        // Put println() in the global scope
        char print_func[] = "println";
        symtbl_add(print_func, 1, 0, 0, 0, 0, 0);
    }
    prog();
    match(EOF);

    if (gen_code_flag) {
        print_line(); // end of mips code
        printf("\n.align 2\n");
        printf(".text\n");
        printf("# main: calls the user-defined main function\n");
        printf("main:\n");
        printf("    j _main \n");
    }

    //fclose(filePtr);
    exit(0);
}
