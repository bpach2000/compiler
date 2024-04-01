#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtbl.h"

// Look for the id in the symbol table
SymTable *get_lexeme_name_from_symTbl(SymTable* symTable, char* id) {
    SymTable* current = symTable;

    while (current != NULL) {
        if (strcmp(id, current->name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int count_table(SymTable* symTable) {
    SymTable* current = symTable;
    int count = 0;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

// Look for the id in the symbol table
int symtbl_look_up(SymTable* symTable, char* id) {
    SymTable* current = symTable;

    while (current != NULL) {
        //printf("current->name %s and lexeme is %s\n", current->name, id);
        if (strcmp(id, current->name) == 0) {
            //printf("current->name %s\n", current->name);
            return 1; // Id already in table
        }
        current = current->next;
    }

    // We did not find the identifier
    return 0;
}

// Look for the id in the symbol table and return symboltable 
SymTable *get_symtbl(SymTable* symTable, char* id, int info) {
    SymTable* current = symTable;

    while (current != NULL) {
        //printf("current->name %s and lexeme is %s\n", current->name, id);
        if (strcmp(id, current->name) == 0 && info == current->info) {
            return current; 
        }
        current = current->next;
    }

    // We did not find the identifier
    return NULL;
}

void symtbl_check_for_errors(char* id) {
    if (chk_decl_flag != 0) {//if (chk_decl_flag == 1) {
        fprintf(stderr, "ERROR LINE %d\n", current_line);
        exit(1);
    }
}

SymTable *add_table_to_linked_list(SymTable* symTable, char* id, int info, int args, int num_local_vars, int offset_local_var, int offset_formals, int is_global) {
    // Check if the table is already in the linked list
    if (symtbl_look_up(symTable, id) == 1 && chk_decl_flag != 0) {
        symtbl_check_for_errors(id);
    }

    SymTable* new_entry = malloc(sizeof(SymTable));
    if (new_entry == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory.\n");
        exit(1);
    }

    strcpy(new_entry->name, id);
    new_entry->info = info;
    new_entry->args = args;
    new_entry->offset_local_var = offset_local_var;
    new_entry->offset_formals = offset_formals;
    new_entry->local_var_num = num_local_vars;
    new_entry->is_global = is_global;
    new_entry->next = NULL;

    // Traverse to the end of the linked list
    SymTable* current = symTable;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append the new entry to the end
    current->next = new_entry;

    return new_entry;
}

// Create a new symbol table
SymTable *symtbl_add(char* id, int info, int args, int num_local_vars, int offset_local_var, int offset_formals, int is_global) {
    //match(id);
    if (local == 1) {
        //printf("Added %s to local table and its a %d on line %d\n", id, info, current_line);
        if (local_table == NULL) {
            local_table = malloc(sizeof(SymTable));
            strcpy(local_table->name, id);
            local_table->info = info;
            local_table->args = args;
            local_table->local_var_num = num_local_vars;
            local_table->offset_local_var = offset_local_var;
            local_table->offset_formals = offset_formals;
            local_table->is_global = is_global;
            local_table->next = NULL;
            return local_table;
        } else {
            return add_table_to_linked_list(local_table, id, info, args, num_local_vars, offset_local_var, offset_formals, is_global);
        }
    } else if (local == 0) {
        //printf("Added %s to global table and its a %d on line %d\n", id, info, current_line);
        //printf("inside symtbl_add global\n");
        if (global_table == NULL) {
            global_table = malloc(sizeof(SymTable));
            strcpy(global_table->name, id);
            global_table->info = info;
            global_table->args = args;
            global_table->local_var_num = num_local_vars;
            global_table->offset_local_var = offset_local_var;
            global_table->offset_formals = offset_formals;
            global_table->is_global = is_global;
            global_table->next = NULL;
            return global_table;
        } else {
            return add_table_to_linked_list(global_table, id, info, args, num_local_vars, offset_local_var, offset_formals, is_global);
        }
    }

    return NULL;
}

void set_local_scope() {
    //printf("\nEnter local scope\n");
    local = 1;
}

void set_global_scope() {
    //printf("\nEnter global scope\n");
    local = 0;
    
    // Free the local table
    SymTable* cur = local_table;
    while (cur != NULL) {
        SymTable* temp = cur;
        cur = cur->next;
        free(temp);
    }
    local_table = NULL;
}

// Function to print out the current symbol table's name and info
void print_symbol_table(SymTable* symTable, char* name) {
    if (symTable == NULL) {
        printf("%s table is empty.\n", name);
        return;
    }

    printf("%s Symbol Table:\n", name);
    printf("Name\tType\t\targs\tlocalVars\toffLVar\toffFormals\n");
    printf("------------------------------------------------------------------\n");
    while (symTable != NULL) {
        printf("%s\t%s", symTable->name, symTable->info == 0 ? "Variable" : "Function");
        printf("\t%d\t%d\t\t%d\t%d\n", symTable->args, symTable->local_var_num, symTable->offset_local_var, symTable->offset_formals);
        symTable = symTable->next;
    }
    printf("\n");
}
