#ifndef SEMANTIC_H_INCLUDED
#define SEMANTIC_H_INCLUDED

char* log_exp();
char* log_term();
char* rel_expr();
char* rel_term();
char* arith_expr();
char* term();
char* factor();
char* log_opr(char *val);
char* rel_opr(char *val);
char* opr(char *val);
char* opr1(char *val);
int constant();

char* expr();
void evalExpression();
void pop();
void displaystack();
int stackSize();
void DFSearch();
void getNext();
void getPrev();
int match(char temp[50]);

void sError();
int isDeclared(char* name);

void setType(char* type);
void setValue(char *value);
void getNext();
int data_type();
void TypeCheckAndIsUsed();
void typeError(char tp[], char tp2[]);

void sns();
void smt();
void output_smt();
void assign_smt();
void dec_smt();
void input_smt();
void loop_smt();
void while_smt();
void for_smt();
void cond_smt();
void switch_smt();
void option();
int type_spec();

#endif
