#ifndef __GEN_CODE_H__
#define __GEN_CODE_H__

#include "symtbl.h"

// // Forward declaration of ASTnode
// typedef struct ast_node ASTnode;

// Operand type enum
typedef enum {
    OP_INTCONST,
    OP_SYM,
} OperandType;

// Operand structure
typedef struct {
    OperandType operand_type;
    union {
        int iconst;
        SymTable *stptr;
    } val;
} Operand;

// Operator enum
typedef enum Operator {
    GC_PLUS,
    GC_MINUS,
    GC_MULT,
    GC_DIV,
    GC_UMINUS,
    GC_ASSG,
    GC_GOTO,
    GC_IF,
    GC_LABEL,
    GC_ENTER,
    GC_LEAVE,
    GC_PARAM,
    GC_CALL,
    GC_RETURN_VOID,
    GC_RETURN,
    GC_RETRIEVE
} Operator;

// Instruction structure
typedef struct Instr {
    Operator op;
    Operand* src1;
    Operand* src2;
    Operand* dst;
    struct Instr* next;
} Instr;

// Function prototypes
void print_line();
void print_instr(void *instr);
SymTable* newtemp(int t);
Instr* newinstr(Operator op, Operand* arg1, Operand* arg2, Operand* dst);
void newlabel();
void printInstr(Instr *instruction);
void gen_code(void *e);

#endif  /* __GEN_CODE_H__ */





// #ifndef __GEN_CODE_H__
// #define __GEN_CODE_H__

// #include "symtbl.h"
// #include "ast.h"
 
// // operand is either symtbl or int const
// typedef enum {
//     OP_INTCONST,
//     OP_SYM,
// } OperandType;

// // src1, scr2 and dst will either be a symtbl node or int constant
// typedef struct {
//     OperandType operand_type;
//     union {
//         int iconst;
//         SymTable *stptr;
//     } val;
// } Operand;

// typedef enum Operator {
//     // Assignment and Copy Statements
//     GC_PLUS,
//     GC_MINUS,
//     GC_MULT,
//     GC_DIV,
//     GC_UMINUS,
//     GC_ASSG,
//     GC_GOTO,
//     GC_IF,
//     GC_LABEL,
//     GC_ENTER,
//     GC_LEAVE,
//     GC_PARAM,
//     GC_CALL,
//     GC_RETURN_VOID,
//     GC_RETURN,
//     GC_RETRIEVE
// } Operator;

// typedef struct {
//     Operator op; 
//     Operand* src1;
//     Operand* src2;
//     Operand* dst;
//     struct Intsr* next; 
// } Instr;

// // create a symbol table entry for a new temporary
// // return a pointer to this ST entry.
// SymTable* newtemp(int t);

// // create a new instruction, fill in the arguments
// // supplied
// // return a pointer to the result
// Instr* newinstr(Operator op, Operand* arg1, Operand* arg2, Operand* dst);

// // return a new label instruction
// // Instr *newlabel() {  
// void newlabel();

// // Print out the 3address code
// void printInstr(Instr *instruction);

// void gen_code(ASTnode *e);

// #endif  /* __GEN_CODE_H__ */