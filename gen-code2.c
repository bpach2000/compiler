#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtbl.h"
#include "gen-code.h"
#include "ast.h"
#include <ctype.h>

int tmp_num = 0;
int arg_count = 0;
static int label_num = 0;

void print_line() {
    printf(".align 2\n");
    printf(".data\n");
    printf("_nl: .asciiz \"\\n\"\n\n");
    printf(".align 2\n");
    printf(".text\n");
    printf("\n# println: print out an integer followed by a newline\n");
    printf("_println:\n");
    printf("    li $v0, 1\n");
    printf("    lw $a0, 0($sp)\n");
    printf("    syscall\n");
    printf("    li $v0, 4\n");
    printf("    la $a0, _nl\n");
    printf("    syscall\n");
    printf("    jr $ra\n");
}

// Concatonate code with parent and child
void code_concat(ASTnode* node1, ASTnode* node2) {
    if (node1->code_tl != NULL) {
        node1->code_tl->next = node2->code_hd;
        node1->code_tl = node2->code_tl; 
    } else {
        node1->code_hd = node2->code_hd;
        node1->code_tl = node2->code_tl;
    }
}

// Concatonate instructions 
void code_instr(ASTnode* curr, Instr* new) {
    if (curr->code_tl != NULL) {
        curr->code_tl->next = new;
        curr->code_tl = curr->code_tl->next;
    } else {
        curr->code_hd = new;
        curr->code_tl = new;
    }

}

void codeGen_expr(ASTnode *e) {
    switch (e->ntype) {
        case INTCONST:
            e->place = newtemp(e->ntype);

            Operand* src1 = (Operand*) malloc(sizeof(Operand));
            src1->operand_type = OP_INTCONST;
            src1->val.iconst = e->num;
    
            Operand* dest = (Operand*) malloc(sizeof(Operand));
            dest->operand_type = OP_SYM;
            dest->val.stptr = e->place;
            
            Instr* new = newinstr(GC_ASSG, src1, NULL, dest);
            
            code_instr(e, new);
            break;
        case IDENTIFIER:
            e->place = e->st_ref;
            e->code_hd = NULL;
            e->code_tl = NULL;
            break;
        case EXPR_LIST:
            if (e->child0 != NULL) {
                codeGen_expr(e->child0);  
            }
            if (e->child1 != NULL) {
                codeGen_expr(e->child1);  
            }
            if (e->child0 != NULL) {
                code_concat(e, e->child0);
            }
            if (e->child1 != NULL) {
                code_concat(e, e->child1);
            }
            break;
        
        // For latter 
        case EQ:
            break;
        case NE:
            break;
        case LE:
            break;
        case LT:
            break;
        case GE:
            break;
        case GT:
            break;
        case ADD:
            break;
        case SUB:
            break;
        case MUL:
            break;
        case DIV:
            break;
        case UMINUS:
            break;
        case AND:
            break;
        case OR:
            break;
        default:
            break;
    }
}

void params(ASTnode* node1, ASTnode* node2, int func_args_total) {
    // Keep recursing to the last arguement 
    if (node2 == NULL) {
        return;
    } else {
        arg_count++;
        params(node1, node2->child1);
    }
    printf("AST node name %s\n", node1->name);
    if (node2->child0 != NULL) {
        Operand* arg = (Operand*) malloc(sizeof(Operand));
        arg->operand_type = OP_SYM;
        arg->val.stptr = node2->child0->place;
        printf("This is the arg count %d\n", arg_count);

        Instr* new = newinstr(GC_PARAM, arg, NULL, NULL);
        code_instr(node1, new);
        //printInstr(new);
    }
}

// ------ generate three address code for statements -----
// stmt : fn_call
// | assg_stmt
// | LBRACE opt_stmt_list RBRACE
// | SEMI
//
// fn_call: ID LPAREN opt_expr_list RPAREN
// assg_stmt: ID opASSG rith_exp SEMI
void codeGen_stmt(ASTnode *s) {
    switch (s->ntype) {
        case FUNC_CALL:
            s->place = newtemp(INTCONST);

            // Generate expression lists on all of the params 
            ASTnode* temp = s->child0;
            codeGen_expr(temp);  
            code_concat(s, temp);

            arg_count = 0;
            if (s->child0 != NULL) {
                params(s, s->child0, func_args_total);
            }

            Operand* f = (Operand*) malloc(sizeof(Operand));
            f->operand_type = OP_SYM;
            f->val.stptr = s->st_ref;
            
            Operand* k = (Operand*) malloc(sizeof(Operand));
            k->operand_type =  OP_INTCONST;
            k->val.iconst = arg_count;

            Instr* new = newinstr(GC_CALL, f, k, NULL);
            code_instr(s, new);

            Instr* retrieve = newinstr(GC_RETRIEVE, NULL, NULL, k);
            code_instr(s, retrieve);

            //printInstr(new);
            break;
        case ASSG: 
            codeGen_expr(s->child0);  // LHS
            codeGen_expr(s->child1);  // RHS

            Operand* left = (Operand*) malloc(sizeof(Operand));
            left->operand_type = OP_SYM;
            left->val.stptr = s->child0->place;

            Operand* right = (Operand*) malloc(sizeof(Operand));
            right->operand_type = OP_SYM;
            right->val.stptr = s->child1->place;

            code_concat(s, s->child0);
            code_concat(s, s->child1);

            Instr* curr = newinstr(GC_ASSG, left, NULL, right);
            code_instr(s, curr);
            
            //printInstr(curr);
            break;
        case STMT_LIST:
            if (s->child0 != NULL) {
                codeGen_stmt(s->child0);  // LHS
                code_concat(s, s->child0);
            }

            if (s->child1 != NULL) {
                codeGen_stmt(s->child1);  // RHS
                code_concat(s, s->child1);
            }
            break;

        // For latter
        case DUMMY:
            break;
        case IF:
            break;
        case WHILE:
            break;
        case RETURN:
            break;
        default:
            break;
    }
}

void gen_mips(Instr* instruction) {
    switch (instruction->op) {
        case GC_PLUS:
            break;
        case GC_MINUS:
            break;
        case GC_MULT:
            break;
        case GC_DIV:
            break;
        case GC_UMINUS:
            break;
        case GC_ASSG:
            printf("    # ASSG _%s = %d\n", instruction->dst->val.stptr->name, instruction->src1->val.iconst);
            printf("    li $t0, %d\n", instruction->src1->val.iconst);
            //printf("offset %d\n\n", instruction->src1->val.stptr->offset_formals);
            printf("    sw $t0, 4($fp)\n\n");
            break;
        case GC_GOTO:
            break;
        case GC_IF:
            break;
        case GC_LABEL:
            break;
        case GC_ENTER:
            // loop through the local table - params and local vars
            // multiply by -4
            int num_vars = instruction->src1->val.stptr->args + instruction->src1->val.stptr->local_var_num;
            printf("%d %d\n", instruction->src1->val.stptr->args, instruction->src1->val.stptr->local_var_num);
            int val = num_vars * 4; 
            printf("\n#enter instruction %s\n", instruction->src1->val.stptr->name);
            printf(".align 2\n");
            printf(".text\n");
            printf("_%s:\n", instruction->src1->val.stptr->name);

            printf("\n# Set up the stack fram\n");
            printf("    la $sp, -8($sp)     # allocate space for old $fp and $ra\n");
            printf("    sw $fp, 4($sp)      # save old $fp\n");
            printf("    sw $ra, 0($sp)      # save return address\n");
            printf("    la $fp, 0($sp)      # set up frame pointer\n");
            printf("    la $sp, -%d($sp)    # allocate stack frame: n = space for locals/temps, in bytes\n\n", val);
            break;
        case GC_LEAVE:
            printf("    # leave\n");
            printf("    li $t0, 0\n");
            printf("    sw $t0, -4($fp)\n\n");
            break;
        case GC_PARAM:
            printf("    # PARAM _tmp0\n");
            printf("    lw $t0, -4($fp)\n"); // This is the instruction that changes
            printf("    la $sp, -4($sp)\n");
            printf("    sw $t0, 0($sp)\n\n");
            break;
        case GC_CALL:
            int num_params = count_table(local_table) * 4;
            //printf("instruction->src1->val.stptr->args %d\n", instruction->src1->val.stptr->args);
            printf("    # Call instruction %s\n", instruction->src1->val.stptr->name);
            printf("    jal _%s\n", instruction->src1->val.stptr->name);
            printf("    la $sp,%d($sp)\n\n", num_params);
            break;
        case GC_RETURN_VOID:
            break;
        case GC_RETURN:
            break;
        case GC_RETRIEVE:
            break;
        default:
            break;
    }
}

// ------ generate three address code for functions -----
void codeGen_func_def(ASTnode *e) {
    e->place = e->st_ref;
    Operand* arg = (Operand*) malloc(sizeof(Operand));
    arg->operand_type = OP_SYM;
    arg->val.stptr = e->place;
    Instr* enter = newinstr(GC_ENTER, arg, NULL, NULL);

    code_instr(e, enter);

    // Iterate through the statement list
    codeGen_stmt(e->child0);

    // Concatenate AST node's code list with the child node's code list
    if (e->child0->code_hd != NULL && e->child0->code_tl != NULL) {
        if (e->code_tl != NULL) {
            // If the original node's code list is not empty,
            // append the child node's code list to the end
            e->code_tl->next = e->child0->code_hd;
            e->code_tl = e->child0->code_tl;
        } else {
            // If the original node's code list is empty,
            // update the head and tail pointers to the child node's code list
            e->code_hd = e->child0->code_hd;
            e->code_tl = e->child0->code_tl;
        }
    }

    Operand* arg2 = (Operand*) malloc(sizeof(Operand));
    arg2->operand_type = OP_SYM;
    arg2->val.stptr = e->place;
    Instr* leave = newinstr(GC_LEAVE, arg, NULL, NULL);

    e->code_tl->next = leave;

    // Print out the order of 3 address instructions
    //printf("_____________ FINAL OUTPUT _______________\n");

    // Write assembly output to output.s 
    FILE *filePtr = freopen("output.s", "w", stdout);

    print_line(); // start of mips code
    if (e->code_hd != NULL) {
        Instr* temp = e->code_hd;
        while (temp != NULL) {
            gen_mips(temp);
            printInstr(temp);
            temp = temp->next;
        }
    }
    printf("    lw $v0, -4($fp)\n");
    printf("    la $sp, 0($fp)\n");
    printf("    lw $ra, 0($sp)\n");
    printf("    lw $fp, 4($sp)\n");
    printf("    la $sp, 8($sp)\n");
    printf("    jr $ra\n");
    printf("    main:\n");
    printf("    j _main \n");

    fclose(filePtr);

}

char* nodeTypeToString(NodeType type) {
    char* str;
    switch(type) {
        case DUMMY:
            str = "DUMMY";
            break;
        case FUNC_DEF:
            str = "FUNC_DEF";
            break;
        case FUNC_CALL:
            str = "FUNC_CALL";
            break;
        case IF:
            str = "IF";
            break;
        case WHILE:
            str = "WHILE";
            break;
        case ASSG:
            str = "ASSG";
            break;
        case RETURN:
            str = "RETURN";
            break;
        case STMT_LIST:
            str = "STMT_LIST";
            break;
        case EXPR_LIST:
            str = "EXPR_LIST";
            break;
        case IDENTIFIER:
            str = "IDENTIFIER";
            break;
        case INTCONST:
            str = "INTCONST";
            break;
        case EQ:
            str = "EQ";
            break;
        case NE:
            str = "NE";
            break;
        case LE:
            str = "LE";
            break;
        case LT:
            str = "LT";
            break;
        case GE:
            str = "GE";
            break;
        case GT:
            str = "GT";
            break;
        case ADD:
            str = "ADD";
            break;
        case SUB:
            str = "SUB";
            break;
        case MUL:
            str = "MUL";
            break;
        case DIV:
            str = "DIV";
            break;
        case UMINUS:
            str = "UMINUS";
            break;
        case AND:
            str = "AND";
            break;
        case OR:
            str = "OR";
            break;
        default:
            str = "Unknown";
            break;
    }
    return str;
}

void post_order_traversal(ASTnode *root) {
    if (root == NULL) {
        return;
    }

    // Traverse left subtree
    post_order_traversal(root->child0);

    // Traverse right subtree
    post_order_traversal(root->child1);

    // Process the current node (root) here
    // For example, you can print the node type or perform any other operation
    char *ntypestr = nodeTypeToString(root->ntype);
    printf("AST Node Type: %s, Name: %s, Num: %d,", ntypestr, root->name, root->num);
    printf("SymTable name: %s\n\n", root->st_ref->name);

    // After processing left and right subtrees, move to the next node
    // in the linked list (if exists)
    post_order_traversal(root->child2);
}

// create a symbol table entry for a new temporary
// return a pointer to this ST entry.
SymTable* newtemp(int t)  {
    // Determine the length of the formatted string
    int len = snprintf(NULL, 0, "tmp%d", tmp_num);
    
    // Allocate memory for the formatted string
    char* name = malloc((len + 1) * sizeof(char));
    
    // Format the string and store it in name
    sprintf(name, "tmp%d", tmp_num);
    tmp_num++;

    if (local_table == NULL) {
        local_table = malloc(sizeof(SymTable));
        strcpy(local_table->name, name);
        local_table->next = NULL;
        return local_table;
    } else {
        SymTable* ntmp = malloc(sizeof(SymTable));
        if (ntmp == NULL) {
            fprintf(stderr, "ERROR: Failed to allocate memory.\n");
            exit(1);
        }

        strcpy(ntmp->name, name);
        ntmp->next = NULL;

        // Traverse to the end of the linked list
        SymTable* current = local_table;
        while (current->next != NULL) {
            current = current->next;
        }

        return ntmp;
    }
    return NULL;
}

// create a new instruction, fill in the arguments
// supplied
// return a pointer to the result
Instr* newinstr(Operator op, Operand* arg1, Operand* arg2, Operand* dst) {
    Instr *ninstr = malloc(sizeof(Instr));
    ninstr->op = op; 
    ninstr->src1 = arg1; 
    ninstr->src2 = arg2; 
    ninstr->dst = dst;
    ninstr->next = NULL;
    return ninstr;
}

// return a new label instruction
// Instr *newlabel() {  
void newlabel() {
    label_num++;
    //return newinstr(LABEL, label_num++);
}

void printInstr(Instr *instruction) { 
    // Print out operator
    printf("operator: ");
    switch(instruction->op) {
        case GC_PLUS: printf("PLUS"); break;
        case GC_MINUS: printf("MINUS"); break;
        case GC_MULT: printf("MULT"); break;
        case GC_DIV: printf("DIV"); break;
        case GC_UMINUS: printf("UMINUS"); break;
        case GC_ASSG: printf("ASSG"); break;
        case GC_GOTO: printf("GOTO"); break;
        case GC_IF: printf("IF"); break;
        case GC_LABEL: printf("LABEL"); break;
        case GC_ENTER: printf("ENTER"); break;
        case GC_LEAVE: printf("LEAVE"); break;
        case GC_PARAM: printf("PARAM"); break;
        case GC_CALL: printf("CALL"); break;
        case GC_RETURN_VOID: printf("RETURN_VOID"); break;
        case GC_RETURN: printf("RETURN"); break;
        case GC_RETRIEVE: printf("RETRIEVE"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\n");

    // Print out the operands
    printf("src1: ");
    if (instruction->src1 != NULL) {
        if (instruction->src1->operand_type == OP_INTCONST) {
            printf(" %d", instruction->src1->val.iconst);
        } else if (instruction->src1->operand_type == OP_SYM) {
            printf(" %s", instruction->src1->val.stptr->name);
        }
    }
    printf("\nscr2: ");
    if (instruction->src2 != NULL) {
        if (instruction->src2->operand_type == OP_INTCONST) {
            printf(" %d", instruction->src2->val.iconst);
        } else if (instruction->src2->operand_type == OP_SYM) {
            printf(" %s", instruction->src2->val.stptr->name);
        }
    }
    printf("\ndst: ");
    if (instruction->dst != NULL) {
        if (instruction->dst->operand_type == OP_INTCONST) {
            printf(" %d", instruction->dst->val.iconst);
        } else if (instruction->dst->operand_type == OP_SYM) {
            printf(" %s", instruction->dst->val.stptr->name);
        }
    }
    printf("\n\n\n");
}

void gen_code(void *e) {
    ASTnode *eAST = (ASTnode*) e;
    //post_order_traversal(eAST);
    printf("\n\n");

    if (eAST != NULL) {
        codeGen_func_def(eAST);
    }
}


