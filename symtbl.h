// symtbl.h

#ifndef SYMTBL_H
#define SYMTBL_H

// Define the structure for the symbol table entry
typedef struct SymTable {
    char name[1042];
    int info;  // 0 for variable, 1 for function
    int args;  
    int local_var_num; // number of local variables for function definitions
    int offset_local_var;
    int offset_formals;
    int is_global; // 1 if glbal variable
    struct SymTable* next;
} SymTable;

// Function declarations
extern int chk_decl_flag;
extern int current_line;
extern int local;
extern int global;
extern SymTable* local_table;
extern SymTable* global_table;

SymTable* get_lexeme_name_from_symTbl(SymTable* symTable, char* id);
int count_table(SymTable* symTable);
SymTable *get_symtbl(SymTable* symTable, char* id, int info);
void print_symbol_table(SymTable* symTable, char* name);
int symtbl_look_up(SymTable* symTable, char* id);
void symtbl_check_for_errors(char* id);
SymTable* add_table_to_linked_list(SymTable* symTable, char* id, int info, int args, int num_local_vars, int offset_local_var, int offset_formals, int is_global);
SymTable* symtbl_add(char* id, int info, int args, int num_local_vars, int offset_local_var, int offset_formals, int is_global);
void set_local_scope();
void set_global_scope();

#endif // SYMTBL_H
