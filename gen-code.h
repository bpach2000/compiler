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
    IF_EQ,          
    IF_NE,              
    //   LE,               /* <= */
    IF_LT,             
    //   GE,               /* >= */
    IF_GT, 
    //GC_IF,
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
Instr* newlabel();
void printInstr(Instr *instruction);
void gen_code(void *e);

#endif  /* __GEN_CODE_H__ */



