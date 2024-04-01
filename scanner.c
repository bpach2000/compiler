
/*
Saumya Debray and Brooke Adkins
CSC 453
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

char* lexeme = NULL; /* a pointer to the current lexeme */
char buf[256]; // Assuming 255 max number of digits
int current_line = 1; // variable to store the current line number

// TODO: extend scanner so that it keeps track of line numbers to report errors

int isOperator(char* ptr, char ch) {
    *ptr++ = ch;
    *ptr = '\0';

    if (ch == '+') return opADD;
    if (ch == '-') return opSUB;
    if (ch == '*') return opMUL;
    if (ch == '/') return opDIV;

    if (ch == '!') {
        ch = getchar();
        if (ch == '=') {
            *ptr++ = ch;
            *ptr = '\0';
            return opNE;
        } else {
            ungetc(ch, stdin);
            return opNOT;
        }
    }

    if (ch == '=') {
        ch = getchar();
        if (ch == '=') {
            *ptr++ = ch;
            *ptr = '\0';
            return opEQ;
        } else {
            ungetc(ch, stdin);
            return opASSG;
        }
    }

    if (ch == '<') {
        ch = getchar();
        if (ch == '=') {
            *ptr++ = ch;
            *ptr = '\0';
            return opLE;
        } else {
            ungetc(ch, stdin);
            return opLT;
        }
    }

    if (ch == '>') {
        ch = getchar();
        if (ch == '=') {
            *ptr++ = ch;
            *ptr = '\0';
            return opGE;
        } else {
            ungetc(ch, stdin);
            return opGT;
        }
    }

    if (ch == '&') {
        ch = getchar();
        if (ch == '&') {
            *ptr++ = ch;
            *ptr = '\0';
            return opAND;
        }
    }

    if (ch == '|') {
        ch = getchar();
        if (ch == '|') {
            *ptr++ = ch;
            *ptr = '\0';
            return opOR;
        }
    }

    return UNDEF;
}

int isPunctuation(char* ptr, char ch) {
    *ptr++ = ch;
    *ptr = '\0';

    if (ch == EOF) return EOF;
    if (ch == '(') return LPAREN;
    if (ch == ')') return RPAREN;
    if (ch == '{') return LBRACE;
    if (ch == '}') return RBRACE;
    if (ch == ',') return COMMA;
    if (ch == ';') return SEMI;

    return UNDEF;
}

void get_comments() {
    char ch;
    while (1) {
        ch = getchar();
        if (ch == '*') {
            ch = getchar();
            if (ch == '/') {
                return;
            } else {
                ungetc(ch, stdin);
            }
        } else if (ch == EOF) {
            return;
        }
    }
}

int get_token() {
    char ch = getchar();
    int chType = UNDEF;
    char* ptr = buf;

    // Check for whitespaces
    while (isspace(ch)) {
        if (ch == '\n') {
            current_line++;
        }
        ch = getchar();
    }

    // Check for comments
    if (ch == '/') {
        ch = getchar();
        if (ch == '*') {
            // Call the function to handle block comments
            get_comments();
            return get_token();
        } else {
            ungetc(ch, stdin);
            lexeme = (char*)malloc(strlen(buf) + 1);
            strcpy(lexeme, "/");
            return opDIV;
        }
    }

    // Operators
    chType = isOperator(ptr, ch);
    if (chType != UNDEF) {
        lexeme = (char*)malloc(strlen(buf) + 1);
        strcpy(lexeme, buf);
        return chType;
    }

    // Punctuation
    chType = isPunctuation(ptr, ch);
    if (chType != UNDEF) {
        lexeme = (char*)malloc(strlen(buf) + 1);
        strcpy(lexeme, buf);
        return chType;
    }

    // INTCON
    if (isdigit(ch)) {
        ptr = buf;
        *ptr++ = ch;

        ch = getchar();
        while (isdigit(ch)) {
            *ptr++ = ch;
            ch = getchar();
        }

        *ptr = '\0';

        if (ch != EOF) {
            ungetc(ch, stdin);
        }

        lexeme = (char*)malloc(strlen(buf) + 1);
        strcpy(lexeme, buf);
        return INTCON;
    } 

    // ID & KEYWORDS
    if (isalpha(ch)) {
        ptr = buf;
        *ptr++ = ch;

        ch = getchar();
        while (isalnum(ch) || ch == '_') {
            *ptr++ = ch;
            ch = getchar();
        }

        *ptr = '\0';

        if (ch != EOF) {
            ungetc(ch, stdin);
        }

        lexeme = (char*)malloc(strlen(buf) + 1);
        strcpy(lexeme, buf);

        if (strcmp(buf, "int") == 0) return kwINT;
        if (strcmp(buf, "if") == 0) return kwIF;
        if (strcmp(buf, "else") == 0) return kwELSE;
        if (strcmp(buf, "while") == 0) return kwWHILE;
        if (strcmp(buf, "return") == 0) return kwRETURN;
        return ID;
    }

    // Handle the case when the character is not defined
    lexeme = (char*)malloc(strlen(buf) + 1);
    strcpy(lexeme, buf);
    return UNDEF;
}