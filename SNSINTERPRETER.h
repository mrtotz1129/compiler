#ifndef SNSINTERPRETER_H_INCLUDED
#define SNSINTERPRETER_H_INCLUDED

void iPop();
void displayDFS();
int isEmpty();
void DFS();
void next();

void getCurrentID(char* name);
int iMATCH(char temp[50]);
void iSNS();
void iSMT();
void iOUTPUT_SMT();
void iDEC_SMT();
void iLOOP_SMT();
void iWHILE_SMT();
void iDO_SMT();
void iFOR_SMT();
void iCOND_SMT();
void iOPTION();
void iSWITCH_SMT();
int iTYPE_SPEC();

char* iLOG_EXPR();
char* iLOG_TERM();
char* iREL_EXPR();
char* iREL_TERM();
char* iARITH_EXPR();
char* iTERM();
char* iFACTOR();
char* iLOG_OPR(char *val);
char* iREL_OPR(char *val);
char* iOPR(char *val);
char* iOPR1(char *val);
char* iEXPR();
int iCONST();

#endif
