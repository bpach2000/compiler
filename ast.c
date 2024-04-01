#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"

NodeType ast_node_type(ASTnode *ptr) {
    return ptr->ntype;
}

char *func_def_name(ASTnode *ptr) {
    // SymTable* curSymTbl = ptr->st_ref;
    // char* name = curSymTbl->name;
    return ptr->name;
}

int func_def_nargs(ASTnode *ptr) {
    // SymTable* current = ptr->child0->st_ref;
    ASTnode* curr = ptr->child1;
    int size = 0; // num of params is 0

    while (curr != NULL) {
        size++;
        curr = curr->child0;
    }
    return size;  // Assuming 0 as a default value, change if necessary
}

char* func_def_argname(void* ptr, int n) {
    ASTnode* node = (ASTnode*)ptr;
    int numArgs = func_def_nargs(node);
    if (n > 0 && n <= numArgs) {
        int i = 1;
        ASTnode* curr = node->child1;
        while (curr != NULL) {
            if (i == n) {
                // SymTable* curSymTbl = curr->st_ref;
                // char* name = curSymTbl->name;
                // return name; // Assuming the parameter name is stored in st_ref
                return curr->name;
            }
            i += 1;
            curr = curr->child0;
        }
    }
    return NULL;
}

void *func_def_body(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* func_body = node->child0;
    
    if (func_body != NULL) {
        return func_body;
    }

    return NULL;
}

char *func_call_callee(ASTnode *ptr) {
    // SymTable* curSymTbl = ptr->st_ref;
    // char* name = curSymTbl->name;
    return ptr->name;
}

void *func_call_args(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* curr = node->child0;
    if (curr != NULL) {
        return curr;
    }
    return NULL;
}

// Stmt itself is in child0
void *stmt_list_head(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* head = node->child0;
    if (head != NULL) {
        return head;
    }
    return NULL;
}

/*
 * ptr: pointer to an AST node for an expression list; expr_list_rest() returns 
 * a pointer to the AST of the rest of this list (i.e., the pointer to the
 * next node in the list).
 */
void *stmt_list_rest(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* curr = node->child1;
    if (curr != NULL) {
        return curr;
    }
    return NULL;
}

void *expr_list_head(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* args = node->child0;
    if (args != NULL) {
        return args;
    }
    return NULL;
}

void *expr_list_rest(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* curr = node->child1;
    if (curr != NULL) {
        return curr;
    }
    return NULL;
}

char *expr_id_name(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    char* id = node->name;
    return id;
}

int expr_intconst_val(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->num;  // Assuming 0 as a default value, change if necessary
}

void *expr_operand_1(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child0;
}

void *expr_operand_2(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child1;
}

void *stmt_if_expr(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child0;//->child0;
}

void *stmt_if_then(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* then_node = node->child1;
    if (then_node != NULL) {
        return then_node->child0;
    }
    return NULL;//node->child1;
}

void *stmt_if_else(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* else_node = node->child2;
    if (else_node != NULL) {
        return else_node->child0;
    }
    return NULL;
}

char *stmt_assg_lhs(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child0->name;
}

void *stmt_assg_rhs(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child1;
}

void *stmt_while_expr(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* exp_node = node->child0;
    if (exp_node != NULL) {
        return exp_node;
    }
    return NULL; 
}

void *stmt_while_body(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    ASTnode* body_node = node->child1;
    if (body_node != NULL) {
        return body_node->child0;
    }
    return NULL;
}

void *stmt_return_expr(void *ptr) {
    ASTnode* node = (ASTnode*)ptr;
    return node->child0;
}

