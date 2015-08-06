#ifndef SYNTACTIC_H_INCLUDED
#define SYNTACTIC_H_INCLUDED
extern int errorCount;

int setCode(char temp[]);
int isMatch(char temp[]);
void error();
void SNS();

void SMT();
void DEC_SMT();
void INPUT_SMT();
void OUTPUT_SMT();
void ASSIGN_SMT();
void LOOP_SMT();
void FOR_SMT();
void DO_SMT();
void WHILE_SMT();
void DO_SMT();
void FOR_SMT();
void COND_SMT();
void SWITCH_SMT();
void OPTION();
int DATA_TYPE();
int CONST();
int EXPR();

int ARITH_EXPR();
int ARITH_OPERATOR();
int TERM();

int LOG_EXPR();
int LOG_TERM();
int LOG_OPERATOR();

int REL_EXPR();
int REL_TERM();
int REL_OPERATOR();
int SYMBOLS();

void countLines();

#endif
