
/*
 * File: scanner.h
 * Author: Saumya Debray
 * Purpose: Lists tokens and their values for use in the CSC 453 project
 */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "ast.h"
/*
 * The enum Token defines integer values for the various tokens.  These
 * are the values returned by the scanner.
 */
typedef enum {
  UNDEF     /* undefined */,
  ID        /* identifier: e.g., x, abc, p_q_12 */,
  INTCON    /* integer constant: e.g., 12345 */,
  LPAREN    /* '(' : Left parenthesis */,
  RPAREN    /* ')' : Right parenthesis */,
  LBRACE    /* '{' : Left curly brace */,
  RBRACE    /* '}' : Right curly brace */,
  COMMA     /* ',' : Comma */,
  SEMI      /*	;  : Semicolon */,
  kwINT     /*	int */,
  kwIF      /*	if */,
  kwELSE    /*	else */,
  kwWHILE   /*	while */,
  kwRETURN  /*	return */,
  opASSG    /*	= : Assignment */,
  opADD     /*	+ : addition */,
  opSUB     /*	– : subtraction */,
  opMUL     /*	* : multiplication */,
  opDIV     /*	/ : division */,
  opEQ      /*	== : Op: equals */,
  opNE      /*	!= : op: not-equals */,
  opGT      /*	>  : Op: greater-than */,
  opGE      /*	>= : Op: greater-or-equal */,
  opLT      /*	<  : Op: less-than */,
  opLE      /*	<= : Op: less-or-equal */,
  opAND     /*	&& : Op: logical-and */,
  opOR      /*	|| : Op: logical-or */,
  opNOT     /* ! : Op: logical-not */
} Token;

int get_token(void);
extern char* lexeme;
extern int current_line;

// Function declarations
extern void match(Token expected);
extern ASTnode* opt_expr_list();
extern ASTnode* expr_list();
extern ASTnode* bool_exp();
extern ASTnode* arith_exp();
extern ASTnode* arith_exp_not_empty();
extern ASTnode* relop();
extern void fn_call();
extern void assg_stmt();
extern ASTnode* stmt();
extern ASTnode* if_stmt();
extern ASTnode* while_stmt();
extern ASTnode* return_stmt();
extern void type();
extern ASTnode* id_list();
extern ASTnode* var_decl();
extern ASTnode* formals();
extern ASTnode* opt_formals();
extern ASTnode* opt_var_decls();
extern ASTnode* opt_stmt_list();
extern void func_defn();
extern void decl_or_func();
extern void prog();

#endif  /* __SCANNER_H__ */