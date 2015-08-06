#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include "SEMANTIC.h"
#include "SYNTACTIC.h"

/*
    Gino de Jesus
    Jojo San Juan
    Carlo Biscocho
    Ryan Reyes
    Dian Roxette Tagra
    Kenji Tinga
    BSCS 3-1N Compiler Design
    S&S Programming Language
*/

FILE *APT;
char Symbol[50], symName[50], errorMessage[50], m[50], currentTYPE[10], APTfile[70];
char finalValue[50], finalValue2[50];
int assignUndeclared = 0, declaration = 0;

typedef struct tree{
    char name[50];
    int childCount;
    int level, lineNum;
    char value[50], type[20];
    struct tree
     *children[100], *parent, *sibling;
}APTree;

typedef struct identifiers{
    int isUsed;
    struct tree *nodeIdent;
    struct identifiers *next, *prev;
}idents;
idents *head, *tail, *util, *declared, *typeChecking, *currentID, *currentID2, *tempID, *a;

struct stack{
    struct tree *node;
    struct stack *next, *prev;
}*top, *curr;

struct mainStack{
    struct tree *node;
    struct stack *next, *prev;
}*top2, *curr2, *temp2, *symbol, *symbol2, *iter, *currentNodeIdent;

struct tree *aCurr, *aRoot, *aTemp, *aTraverse, *semanRoot;

int semlength = 0;

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
void push(struct tree *temp);
void pop();
void push2(struct tree *temp);
void displaystack();
int stackSize();
void DFSearch();
void getNext();
void getPrev();
int match(char temp[50]);

void sError();
void notUsedError(char var[]);
void notInitialized(char var[]);
int isDeclared(char* name);
void enqueueId(APTree* node);
void setType(char* type);
void setValue(char *value);
void getNext();
int data_type();
void TypeCheckAndIsUsed();
void typeError(char tp[], char tp2[]);
void breakContinueError(char var[]);

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
void stepsize();

struct mainStack *tempSym, *tempSym2, *tempSym3, *tempSym4, *tempSym5;

int oneSmt = 0;

idents* SEMANTIC(struct tree *tempRoot){
    remove("AnnonatedParseTree.sns");
    APT = fopen("AnnonatedParseTree.sns","aw+");
    semlength = oneSmt = 0;
    head = tail = curr = util = NULL;
    aTraverse = aRoot = aTemp = aCurr = NULL;
    top = curr = NULL;
    top2 = curr2 = temp2 = symbol = symbol2 = iter = NULL;
    tempID = a = head = tail = util = declared = currentID = typeChecking = currentID2 = currentNodeIdent =  NULL;
    tempSym = tempSym2 = tempSym3 = tempSym4 = tempSym5 = NULL;
    semanRoot = tempRoot;

    DFSearch();
    sns();

    displaystack();

    TypeCheckAndIsUsed();
    //displayIdentifiers();
    fclose(APT);

    return head;
}

void sns(){
    if(match("SNS")) getNext();
    if(match("meth_main")) getNext();
    if(match("main")) getNext();
    if(match("start")){
        getNext();
        getNext();
    }
    smt();
    if(symbol!=NULL)
        if(match("finish")){
            getNext();
        }
}

void smt(){
    if(oneSmt){
        oneSmt = 0;
        if(match("SMT")) getNext();
        if(match("DEC_SMT")) dec_smt();
        else if(match("ASSIGN_SMT")) assign_smt();
        else if(match("OUTPUT_SMT")) output_smt();
        else if(match("INPUT_SMT")) input_smt();
        else if(match("LOOP_SMT")) loop_smt();
        else if(match("COND_SMT")) cond_smt();
        else if(match("SWITCH_SMT"))  switch_smt();
        else if(match("st_cont")||match("st_break")){
            getNext();
            if(symbol->node->parent->parent->parent->parent!=NULL){
                if(!strcmp(symbol->node->parent->parent->parent->parent->name, "LOOP_SMT")
                   ||!strcmp(symbol->node->parent->parent->parent->parent->name, "SWITCH_SMT"));
                        else breakContinueError(symbol->node->name);
            }
            else breakContinueError(symbol->node->name);

            getNext();

            if(match("term_sym")){
                getNext();
                getNext();
            }
        }
        else return;

        return;
    }
    else{
        if(match("SMT")) getNext();
        if(match("DEC_SMT")) dec_smt();
        else if(match("ASSIGN_SMT")) assign_smt();
        else if(match("OUTPUT_SMT")) output_smt();
        else if(match("INPUT_SMT")) input_smt();
        else if(match("LOOP_SMT")) loop_smt();
        else if(match("COND_SMT")) cond_smt();
        else if(match("SWITCH_SMT"))  switch_smt();
        else if(match("st_cont")||match("st_break")){
            getNext();

            if(symbol->node->parent->parent->parent->parent!=NULL){
                if(!strcmp(symbol->node->parent->parent->parent->parent->name, "LOOP_SMT")
                   ||!strcmp(symbol->node->parent->parent->parent->parent->name, "SWITCH_SMT"));
                        else breakContinueError(symbol->node->name);
            }
            else breakContinueError(symbol->node->name);

            getNext();

            if(match("term_sym")){
                getNext();
                getNext();
            }
        }
        else return;
    }

    if(!strcmp("xFINISH",symbol->node->name)) return;
        else smt();
}

void loop_smt(){
    if(match("LOOP_SMT")) getNext();
    if(match("WHILE_SMT")) while_smt();
    else if(match("FOR_SMT")) for_smt();
    else if(match("DO_SMT")) do_smt();
}


void switch_smt(){
    char* val = malloc(100);
    if(match("SWITCH_SMT")) getNext();
    if(match("cond_swt")){
        getNext();
        getNext();
    }
    if(match("oParen")){
        getNext();
        getNext();
    }

    val = expr();

    if(match("cParen")){
        getNext();
        getNext();
    }
    if(match("oCurl")){
        getNext();
        getNext();
    }
    if(match("OPTION")) option();

    if(match("cCurl")){
        getNext();
        getNext();
    }
}//SWITCH

void option(){
    char* val = malloc(100);
    if(match("OPTION")) getNext();
    if(match("cond_opt")){
        getNext();
        getNext();
    }

    val = expr();
    assignUndeclared = 0;

    if(match("colon")){
        getNext();
        getNext();
    }

    smt();

    if(match("OPTION")) option();
    else{
        if(match("cond_def")){
            getNext();
            getNext();
        }
        if(match("colon")){
            getNext();
            getNext();
        }

        smt();

        if(match("st_break")
           ||match("st_cont")){
            getNext();
            getNext();
        }
        if(match("term_sym")){
            getNext();
            getNext();
        }
    }
}

void cond_smt(){
    char* val = malloc(100);
    if(match("COND_SMT")) getNext();
    if(match("cond_if")){
        getNext();
        getNext();
    }
    if(match("oParen")){
        getNext();
        getNext();
    }
    val = expr();
    assignUndeclared = 0;

    if(match("cParen")){
        getNext();
        getNext();
    }

    if(match("cond_then")){
        getNext();
        getNext();
    }
    if(match("oCurl")){
        getNext();
        getNext();

        smt();

        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
    else{
        oneSmt = 1;

        smt();

        if(match("cCurl")){
            getNext();
            getNext();
        }
    }

    if(match("cond_else")){
        getNext();
        getNext();

        if(match("cond_if")){

            cond_smt();
        }
        else if(match("oCurl")){
            getNext();
            getNext();

            smt();

            if(match("cCurl")){
                getNext();
                getNext();
            }
        }
        else{
            oneSmt = 1;
            smt();
            if(match("cCurl")){
                getNext();
                getNext();
            }
        }

    }
}


void for_smt(){
    char* val = malloc(100);
    if(match("FOR_SMT")) getNext();
    if(match("lp_for")){
        getNext();
        getNext();
    }

    if(match("oParen")){
        getNext();
        getNext();
    }
    if(match("term_sym")){
        getNext();
        getNext();
    }
    else if(match("DEC_SMT")) dec_smt();
    else if(match("ASSIGN_SMT")) assign_smt();

    assignUndeclared = 0;

    if(match("term_sym")){
        getNext();
        getNext();
    }
    else val = expr();

    if(match("term_sym")){
        getNext();
        getNext();
    }

    stepsize();

    if(match("cParen")){
        getNext();
        getNext();
    }
    if(match("oCurl")){
        getNext();
        getNext();
            smt();

        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
    else{

        oneSmt = 1;
        smt();
        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
}

void do_smt(){
    char* val = malloc(100);
    if(match("DO_SMT")) getNext();
    if(match("lp_do")){
        getNext();
        getNext();
    }

    if(match("oCurl")){
        getNext();
        getNext();

        smt();

        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
    else{
        oneSmt = 1;
        smt();
        if(match("cCurl")){
            getNext();
            getNext();
        }
    }

    if(match("lp_while")){
        getNext();
        getNext();
    }
    if(match("oParen")){
        getNext();
        getNext();
    }

    val = expr();
    assignUndeclared = 0;

    if(match("cParen")){
        getNext();
        getNext();
    }
    if(match("term_sym")){
        getNext();
        getNext();
    }
}

void while_smt(){
    char* val = malloc(100);
    if(match("WHILE_SMT")) getNext();
    if(match("lp_while")){
        getNext();
        getNext();
    }
    if(match("oParen")){
        getNext();
        getNext();
    }

    val = expr();
    assignUndeclared = 0;

    if(match("cParen")){
        getNext();
        getNext();
    }
    if(match("oCurl")){
        getNext();
        getNext();

        smt();

        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
    else{
        oneSmt = 1;
        smt();
        if(match("cCurl")){
            getNext();
            getNext();
        }
    }
}

void input_smt(){
    int declare = 0;
    if(match("INPUT_SMT")) getNext();
    if(match("st_read")){
        getNext();
        getNext();
    }
    if(match("ident")||match("inval_ident")) getNext();

    declare = isDeclared(symName);

    if(declare){
        getNext();
    }
    else{
        sError();
        getNext();
    }
    if(match("comma")){
        getNext();
        getNext();
    }
    if(type_spec()){

        if(match("%i")){
            if(declare){
                if(strcmp(declared->nodeIdent->type,"int")){
                    sprintf(m, "Invalid Type Specifier < %s > - < %s >", declared->nodeIdent->type, symbol->node->name);
                    sError();
                }
            }
        }
        else if(match("%f")){
            if(declare){
                if(strcmp(declared->nodeIdent->type,"float")){
                    sprintf(m, "Invalid Type Specifier < %s > - < %s >", declared->nodeIdent->type, symbol->node->name);
                    sError();
                }
            }
        }
        else if(match("%s")){
            if(declare){
                if(strcmp(declared->nodeIdent->type,"string")){
                    sprintf(m, "Invalid Type Specifier < %s > - < %s >", declared->nodeIdent->type, symbol->node->name);
                    sError();
                }
            }
        }
        else if(match("%c")){
            if(declare){
                if(strcmp(declared->nodeIdent->type,"char")){
                    sprintf(m, "Invalid Type Specifier < %s > - < %s >", declared->nodeIdent->type, symbol->node->name);
                    sError();
                }
            }
        }
        else if(match("%b")){
            if(declare){
                if(strcmp(declared->nodeIdent->type,"bool")){
                    sprintf(m, "Invalid Type Specifier < %s > - < %s >", declared->nodeIdent->type, symbol->node->name);
                    sError();
                }
            }
        }
        getNext();
    }

    if(match("term_sym")){
        getNext();
        getNext();
    }
    declare = 0;
}

int type_spec(){
    if(match("type_int")
       ||match("type_float")
       ||match("type_char")
       ||match("type_str")
       ||match("type_bool")){
        getNext();
        return 1;
    }
    return 0;
}

void assign_smt(){
    int declare = 1;
    char *val = malloc(70);
    float num = 0;
    int num2 = 0;

    if(match("ASSIGN_SMT")) getNext();
    if(match("UNARY_OP")){
        getNext();

        if(match("incre")){
            getNext();
            getNext();
            if(match("ident")||match("inval_ident")){
                getNext();
                if(isDeclared(symbol->node->name)){
                    currentID->isUsed = 1;
                    num = (float)atof(currentID->nodeIdent->value);
                    num++;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                else{
                    sError();
                    strcpy(val, "");
                }
            }
            getNext();
        }
        else if(match("decre")){
            getNext();
            getNext();
            if(match("ident")||match("inval_ident")){
                getNext();
                curr2 = symbol;
                if(isDeclared(symbol->node->name)){
                    currentID->isUsed = 1;
                    num = (float)atof(currentID->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                else{
                    sError();
                    strcpy(val, "");
                }
            }
        }
        getNext();
    }//unary_operator
    else if(match("ident")||match("inval_ident")){
        getNext();

        declare = isDeclared(symName);
        tempID = currentID;

        if(declare){
            currentID->isUsed = 1;
            getNext();
            if(unary()){
                if(match("++")){
                    getNext();
                    num = (float)atof(currentID->nodeIdent->value);
                    num++;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);

                }
                else if(match("--")){
                    getNext();
                    num = (float)atof(currentID->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                setValue(currentID->nodeIdent->value);
            }
            else if(match("op_assign")){
                getNext();
                if(match("=")) getNext();
                char *tempStr = malloc(70);
                strcpy(finalValue2, expr());

                if(!assignUndeclared && declare){
                    sprintf(tempID->nodeIdent->value, "%s", finalValue2);
                    sprintf(tempID->nodeIdent->parent->value, "%s", finalValue);
                }
                    //if(!strcmp(tempID->nodeIdent->value, "")) sprintf(tempID->nodeIdent->value, "%s", finalValue2);
                assignUndeclared = 0;
                //printf("%s\n",tempID->nodeIdent->value);
            }
        }
        else{
            sError();
            getNext();
        }
    }

    if(match("term_sym")){
        getNext();
        if(match(";")) getNext();
    }
    declare = 0;
}

void stepsize(){
    int declare = 1;
    char *val = malloc(70);
    float num = 0;
    int num2 = 0;

    if(match("STEPSIZE")) getNext();
    if(match("UNARY_OP")){
        getNext();

        if(match("incre")){
            getNext();
            getNext();
            if(match("ident")||match("inval_ident")){
                getNext();
                if(isDeclared(symbol->node->name)){
                    num = (float)atof(currentID->nodeIdent->value);
                    num++;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                else{
                    sError();
                    strcpy(val, "");
                }
            }
            getNext();
        }
        else if(match("decre")){
            getNext();
            getNext();
            if(match("ident")||match("inval_ident")){
                getNext();
                curr2 = symbol;
                if(isDeclared(symbol->node->name)){
                    num = (float)atof(currentID->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                else{
                    sError();
                    strcpy(val, "");
                }
            }
        }
        getNext();
    }//unary_operator
    else if(match("ident")||match("inval_ident")){
        getNext();

        declare = isDeclared(symName);
        tempID = currentID;

        if(declare){
            getNext();
            if(unary()){
                if(match("++")){
                    getNext();
                    num = (float)atof(currentID->nodeIdent->value);
                    num++;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);

                }
                else if(match("--")){
                    getNext();
                    num = (float)atof(currentID->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    sprintf(currentID->nodeIdent->value,"%s",val);
                    sprintf(currentID->nodeIdent->parent->value,"%s",val);
                }
                setValue(currentID->nodeIdent->value);
            }
            else if(match("op_assign")){
                getNext();
                if(match("=")) getNext();

                strcpy(finalValue2, expr());

                if(!assignUndeclared && declare){

                    //if(!strcmp(tempID->nodeIdent->value, "")) sprintf(tempID->nodeIdent->value, "%s", finalValue2);
                    sprintf(tempID->nodeIdent->value, "%s", finalValue2);
                    sprintf(tempID->nodeIdent->parent->value, "%s", finalValue);
                }
                assignUndeclared = 0;
                //printf("%s\n",tempID->nodeIdent->value);
            }
        }
        else{
            sError();
            getNext();
        }
    }
    declare = 0;
}

void output_smt(){
    int declare = 0;
    if(match("OUTPUT_SMT")) getNext();
    if(match("st_print")){
        getNext();
        getNext();
    }
    if(match("ident")||match("inval_ident")){
        getNext();
        declare = isDeclared(symName);

        if(declare){
            sprintf(symbol->prev->node->value, "%s", currentID->nodeIdent->value);
            sprintf(symbol->prev->node->type, "%s", currentID->nodeIdent->type);
            sprintf(symbol->node->value, "%s", currentID->nodeIdent->value);
            sprintf(symbol->node->type, "%s", currentID->nodeIdent->type);
            getNext();
        }
        else{
            sError();
            getNext();
        }
    }
    else if(match("val_str")){
        char *tempStr = malloc(70);

        getNext();

        sprintf(tempStr, "%s", symbol->node->name);
        sprintf(symbol->node->value, "%s", tempStr);
        sprintf(symbol->prev->node->value, "%s", tempStr);
        sprintf(symbol->node->type, "string");
        sprintf(symbol->prev->node->type, "string");

        getNext();
    }

    if(match("ccat_out")){
        getNext();
        getNext();
        output_smt();
    }
    else if(match("term_sym")){
        getNext();
        getNext();
    }
    declare = 0;
}//output_smt()

void dec_smt(){
    declaration = 1;

    int isRepeat = 0;
    if(match("DEC_SMT")) getNext();
    if(match("DATA_TYPE")) getNext();
    if(data_type()){
        getNext();
        strcpy(currentTYPE, symName);
        setType(currentTYPE);
        getNext();
    }
    if(match("ident")||match("inval_ident")){
        setType(currentTYPE);

        getNext();

        if(isDeclared(symName)){
            isRepeat = 1;
            sprintf(m, "Multiple Declarations of Variables");
            sError();
        }
        else if(!strcmp(symbol->prev->node->name, "inval_ident"));
        else{
            enqueueId(symbol->node);
            setType(currentTYPE);
        }

        getNext();
    }
    if(match("term_sym")){
        getNext();
        if(match(";")) getNext();
    }
    else if(match("op_assign")){
        getNext();
        if(match("=")){
            getNext();
            char *tempStr = malloc(70);
            strcpy(finalValue, expr());
            if(!isRepeat && !assignUndeclared){
                strcpy(currentID2->nodeIdent->value, finalValue);
                strcpy(currentID2->nodeIdent->parent->value, finalValue);
            }
            assignUndeclared = 0;
        }
    }

    if(match("comma")){
        getNext();
        getNext();
        dec_smt();
    }
    else if(match("term_sym")){
        getNext();
        if(match(";")) getNext();
    }

    isRepeat = declaration = 0;
}


int data_type(){
    if(match("dt_bool")
       ||match("dt_char")
       ||match("dt_float")
       ||match("dt_int")
       ||match("dt_string")){
        return 1;
    }
    return 0;
}//data_type()


int constant(){
    if(match("val_int")
       ||match("val_float")
       ||match("val_str")
       ||match("val_char")
       ||match("inval_sym")
       ||match("inval_num")
       ||match("inval_str")
       ||match("inval_char")
       ||match("bl_false")
       ||match("bl_true")){
        getNext();
        return 1;
    }
    return 0;
}//constant()

char* expr(){
    char* val = malloc(100);
    char temp[50];

    if(match("EXPR")){
        getNext();
        val = log_exp();
        set(val);
        getPrev();
        set(val);
    }

    symbol2 = NULL;
    //printf("%s\t===\n",symbol->node->name);
    return val;
}//EXPR()

char* log_exp(){
    char* val = malloc(100);
    int logcheck = 0;
    float num;

    if(match("LOG_EXPR")){
        getNext();
        if(match("log_not")){
            getNext();
            getNext();
            logcheck = 1;
        }
        val = log_term();
        if(logcheck){
            num = atof(val);
            if(!strcmp(val,"true")) sprintf(val, "false");
            else if(!strcmp(val,"false")) sprintf(val, "true");
            else if(num != 0){
                num = 0;
                sprintf(val, "%d", num);
            }
            logcheck = 0;
        }
        set(val);
        getPrev();
    }


    if(match("LOG_OPERATOR")){
        tempSym4 = symbol2;
        val = log_opr(val);
        symbol2 = tempSym4;
    }
    /*
    if(match("log_not")){
        current = current->parent;
    */
    return val;
}//LOG_EXPR()

char* log_term(){
    char* val = malloc(100);

    if(match("LOG_TERM")){
        getNext();
        val = rel_expr();
        set(val);
        getPrev();
    }

    return val;
}//LOG_TERM()

char* log_opr(char *val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    int logcheck = 0;
    float num;

    if(match("LOG_OPERATOR")){
        getNext();
        if(match("log_and")){
            getNext();
            getNext();
            if(match("log_not")){
                getNext();
                getNext();
                logcheck = 1;
            }

            tempVal = log_term();

            if(logcheck){
                num = atof(tempVal);
                if(!strcmp(tempVal,"true")) sprintf(tempVal, "false");
                else if(!strcmp(tempVal,"false")) sprintf(tempVal, "true");
                else if(num != 0){
                    num = 0;
                    sprintf(tempVal, "%d", num);
                }
                logcheck = 0;
            }

            set(tempVal);

            if((!strcmp(val, "true") && !strcmp(tempVal, "true"))
                ||(atof(val)!=0 && atof(tempVal)!=0)
                ||(atof(val)!=0 && !strcmp(tempVal, "true"))
                ||(!strcmp(val, "true") && atof(tempVal)!=0)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("log_or")){
            getNext();
            getNext();
            if(match("log_not")){
                getNext();
                getNext();
                logcheck = 1;
            }

            tempVal = log_term();

            if(logcheck){
                num = atof(tempVal);
                if(!strcmp(tempVal,"true")) sprintf(tempVal, "false");
                else if(!strcmp(tempVal,"false")) sprintf(tempVal, "true");
                else if(num != 0){
                    num = 0;
                    sprintf(tempVal, "%d", num);
                }
                logcheck = 0;
            }

            set(tempVal);
            if((!strcmp(val, "true")||!strcmp(tempVal, "true"))
                ||(atof(val)!=0 || atof(tempVal)!=0)
                ||(atof(val)!=0 || !strcmp(tempVal, "true"))
                ||(!strcmp(val, "true") || atof(tempVal)!=0)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        if(match("LOG_OPERATOR")) val2 = log_opr(val2);
    }
    return val2;
}

char* rel_expr(){
    char* val = malloc(100);

    if(match("REL_EXPR")){
        getNext();
        val = rel_term();
        set(val);
        getPrev();
    }

    if(match("REL_OPERATOR")){
        tempSym3 = symbol2;
        val = rel_opr(val);
        symbol2 = tempSym3;
    }
    return val;
}

char* rel_opr(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);

    if(match("REL_OPERATOR")){
        getNext();

        if(match("rel_not")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(!strcmp(val, tempVal)){
                strcpy(val2, "false");
            }
            else strcpy(val2, "true");
        }
        else if(match("rel_equal")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(!strcmp(val, tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_great")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(atof(val)>atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_less")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(atof(val)<atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_lqual")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(atof(val)<=atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_gqual")){
            getNext();
            getNext();
            tempVal = rel_term();
            set(tempVal);
            if(atof(val)>=atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        if(match("REL_OPERATOR")) rel_opr(val2);
    }

    return val2;
}

char* rel_term(){
    char* val = malloc(100);

    if(match("REL_TERM")){
        getNext();
        val = arith_expr();
        set(val);
        getPrev();
    }

    return val;
}//REL_TERM

char* arith_expr(){
    char* val = malloc(100);

    if(match("ARITH_EXPR")){
        getNext();
        val = term();
        set(val);
    }

    getPrev();
    set(val);
//printf("%s\n",symbol2->node->name);
    if(match("OPERATOR")){
        tempSym = symbol2;
        val = opr(val);
        symbol2 = tempSym;
    }

    return val;
}//ARITH_OPERATOR()

char* opr(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    float num;
    int num2;

    if(match("OPERATOR")){
        getNext();
        if(match("math_add")){
            getNext();
            getNext();
            tempVal = term();
            num = atof(val) + atof(tempVal);
            set(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        else if(match("math_sub")){
            getNext();
            getNext();
            tempVal = term();
            num = atof(val) - atof(tempVal);
            set(tempVal);

            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        if(match("OPERATOR")) val2 = opr(val2);
    }

    return val2;
}

char* term(){
    char* val = malloc(100);

    if(match("TERM")){
        getNext();
        val = factor();
        set(val);
    }

    if(match("OPERATOR1")){
        tempSym2 = symbol2;
        val = opr1(val);
        symbol2 = tempSym2;
    }

    getPrev();
    return val;
}//TERM()

char* opr1(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    float num;
    int num2;

    if(match("OPERATOR1")){
        getNext();

        if(match("math_mul")){
            getNext();
            getNext();
            tempVal = factor();
            num = atof(val) * atof(tempVal);

            set(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        else if(match("math_div")){
            getNext();
            getNext();
            tempVal = factor();
            if(atof(tempVal) == 0){
                strcpy(m, "Cannot divide a number by zero");
                sError();
                strcpy(val2, "0");
            }
            else{
                num = atof(val) / atof(tempVal);
                set(tempVal);
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(val2, "%d", num2);
                }
                else sprintf(val2, "%f", num);
            }
        }
        else if(match("math_mod")){
            getNext();
            getNext();
            tempVal = factor();
            if(fmod(atof(val),1) != 0||fmod(atof(tempVal),1) != 0){
                strcpy(m, "Ivalid Operands of Type of Float");
                sError();
                strcpy(val2,"0");
            }
            else{
                num = fmod((float)atof(val),(float)atof(tempVal));
                set(tempVal);
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(val2, "%d", num2);
                }
                else sprintf(val2, "%f", num);
            }
        }

        if(match("OPERATOR1")) val2 = opr1(val2);
    }

    return val2;
}

char* factor(){
    char* val = malloc(100);
    char temp[20];
    char* tempVal = malloc(100);
    int num2;
    float num;

    if(match("FACTOR")){
        getNext();
        if(match("CONST")){
            symbol2 = symbol;
            getNext();
            if((match("val_str")||match("val_char"))
               && (!strcmp(symbol->prev->prev->prev->prev->prev->prev->prev->prev->node->name, "REL_OPERATOR")
                    || !strcmp(symbol->prev->prev->prev->prev->prev->prev->prev->prev->prev->prev->node->name, "LOG_OPERATOR")
                    || !strcmp(symbol->prev->prev->prev->prev->prev->prev->node->name, "OPERATOR")
                    || !strcmp(symbol->prev->prev->prev->prev->prev->node->name, "OPERATOR1")
                    || !strcmp(symbol->next->next->node->name, "REL_OPERATOR")
                    || !strcmp(symbol->next->next->node->name, "LOG_OPERATOR")
                    || !strcmp(symbol->next->next->node->name, "OPERATOR")
                    || !strcmp(symbol->next->next->node->name, "OPERATOR1"))){
                strcpy(m, "Invalid Operand of Type of String / Character ");
                sError();
                getNext();
                strcpy(val, "0");
                getNext();
                set(val);
                getPrev();
            }
            else if(constant()){
                strcpy(val, symbol->node->name);
                strcpy(temp,val);
                getNext();
                set(val);
                getPrev();
            }
        }//constants

        else if(match("ident")||match("inval_ident")){
            symbol2 = symbol->next;
            getNext();
            if(isDeclared(symbol->node->name)){
                getNext();
                if(!strcmp(currentID->nodeIdent->value, "")) notInitialized(currentID->nodeIdent->name);
                if(unary()){
                    if(match("++")){
                        getNext();
                        num = (float)atof(currentID->nodeIdent->value);
                        num++;
                        if(fmod(num, 1) == 0){
                            num2 = (int)(num);
                            sprintf(val, "%d", num2);
                        }else sprintf(val, "%f", num);
                        sprintf(currentID->nodeIdent->value,"%s",val);
                        sprintf(currentID->nodeIdent->parent->value,"%s",val);
                    }
                    else if(match("--")){
                        getNext();
                        num = (float)atof(currentID->nodeIdent->value);
                        num--;
                        if(fmod(num, 1) == 0){
                            num2 = (int)(num);
                            sprintf(val, "%d", num2);
                        }else sprintf(val, "%f", num);
                        sprintf(currentID->nodeIdent->value,"%s",val);
                        sprintf(currentID->nodeIdent->parent->value,"%s",val);
                    }
                }
                else sprintf(val, "%s", currentID->nodeIdent->value);
                //strcpy(val,currentID->nodeIdent->value);
                strcpy(temp,val);
                //getNext();
                set(val);
                getPrev();
                set(val);
                getPrev();
            }
            else{
                assignUndeclared = 1;
                sError();
                strcpy(val, "");
                getNext();
                set(val);
                getPrev();
                set(val);
                getPrev();
            }//ident
        }//identifiers

        else if(match("oParen")){
            tempSym5 = symbol->prev;
            getNext();
            getNext();

            tempVal = expr();
            symbol2 = tempSym5;
            if(match("cParen")){
                getNext();
                getNext();

            }
            strcpy(val, tempVal);
        }//parenthesis

        else if(match("UNARY_OP")){
            tempSym5 = symbol->prev;
            getNext();
            if(match("incre")){
                getNext();
                getNext();
                if(match("ident")||match("inval_ident")){
                    getNext();
                    if(isDeclared(symbol->node->name)){
                        if(!strcmp(currentID->nodeIdent->value, "")) notInitialized(currentID->nodeIdent->name);
                        num = (float)atof(currentID->nodeIdent->value);
                        num++;
                        if(fmod(num, 1) == 0){
                            num2 = (int)(num);
                            sprintf(val, "%d", num2);
                        }
                        else sprintf(val, "%f", num);
                        sprintf(currentID->nodeIdent->value,"%s",val);
                        sprintf(currentID->nodeIdent->parent->value,"%s",val);
                        symbol2 = tempSym5;
                        getNext();
                        set(val);
                        getPrev();
                        set(val);
                        getPrev();
                    }
                    else{
                        sError();
                        strcpy(val, "");
                        symbol2 = tempSym5;
                        getNext();
                        set(val);
                        getPrev();
                        set(val);
                        getPrev();
                    }
                }
            }
            else if(match("decre")){
                getNext();
                getNext();
                if(match("ident")||match("inval_ident")){
                    getNext();
                    if(isDeclared(symbol->node->name)){
                        if(!strcmp(currentID->nodeIdent->value, "")) notInitialized(currentID->nodeIdent->name);
                        num = (float)atof(currentID->nodeIdent->value);
                        num--;
                        if(fmod(num, 1) == 0){
                            num2 = (int)(num);
                            num2;
                            sprintf(val, "%d", num2);
                        }
                        else sprintf(val, "%f", num);
                        sprintf(currentID->nodeIdent->value,"%s",val);
                        sprintf(currentID->nodeIdent->parent->value,"%s",val);
                        symbol2 = tempSym5;
                        getNext();
                        set(val);
                        getPrev();
                        set(val);
                        getPrev();
                    }
                    else{
                        sError();
                        strcpy(val, "");
                        symbol2 = tempSym5;
                        getNext();
                        set(val);
                        getPrev();
                        set(val);
                        getPrev();
                    }
                }
            }
        }//unary_operator
    }
    return val;
}

int unary(){
    if(match("UNARY_OP")) getNext();
    if(match("incre")||match("decre")){
        getNext();
        return 1;
   }
   return 0;
}

void DFSearch(){
    int i;
    aTraverse = semanRoot;
    push(aTraverse);
    while(stackSize()){
        pop();
        for(i = aTraverse->childCount; i >= 0 ; i--){
            if(aTraverse->children[i] !=NULL ){
                push(aTraverse->children[i]);
            }
        }
        if(top!=NULL) aTraverse = top->node;
    }
}

void pop(){
    if(top == NULL) return;
    else{
        push2(top->node);
        top = (top->prev != NULL)? top->prev: NULL;
        if(top != NULL) top->next = NULL;
    }
    //printf("===========%s\n",front->node->data);
}


void push2(struct tree *temp2){
    curr2 = (struct mainStack*)malloc(sizeof(struct mainStack));
    curr2->next = curr2->prev = NULL;

    if(top2 == NULL){
        top2 = symbol = curr2;
        curr2->node = temp2;
    }
    else{
        top2->next = curr2;
        curr2->prev = top2;
        top2 = curr2;
        curr2->node = temp2;
    }
    top2->next = NULL;
}


void push(struct tree *temp1){
    curr = (struct stack*)malloc(sizeof(struct stack));
    curr->next = curr->prev = NULL;
    if(top == NULL){
        top = curr;
        curr->node = temp1;
    }
    else{
        top->next = curr;
        curr->prev = top;
        top = curr;
        curr->node = temp1;
    }
    top->next = NULL;
}


void displaystack(){
    for(temp2 = top2; temp2!=NULL; temp2 = temp2->prev){
        sprintf(APTfile, "<VALUE ( %s ) TYPE ( %s )> Line - %d * ( Level: %d ) -- < %s > Parent: %s \n", temp2->node->value, temp2->node->type, temp2->node->lineNum, temp2->node->level, temp2->node->name, temp2->node->parent->name);
        fprintf(APT, APTfile);
    }
}

int stackSize(){
    int size = 0;
    struct stack *temp;
    for(temp = top; temp!=NULL; temp = temp->prev){
        size++;
    }
    return size;
}

void getNext(){
    symbol = (symbol->next != NULL)? symbol->next: NULL;
    strcpy(symName, symbol->node->name);
}

void getPrev(){
    symbol2 = (symbol2->prev != NULL)? symbol2->prev: NULL;
}

int match(char temp[50]){
    if(!strcmp(symbol->node->name, temp)) return 1;
        return 0;
}

int adMatch(char temp[50]){
    if(!strcmp(iter->node->name, temp)) return 1;
        return 0;
}

void setValue(char* value){
    float num;
    strcpy(curr2->node->value, value);

    if(!strcmp(value, ""));
    else if(!strcmp(value, "false")
       ||!strcmp(value, "true")) strcpy(curr2->node->type, "bool");
    else if(value[0] == '"') strcpy(curr2->node->type, "string");
    else if(value[0] == '\'') strcpy(curr2->node->type, "char");
    else{
        num = atof(value);
        if(fmod(num, 1) == 0){
            strcpy(curr2->node->type, "int");
        }
        else strcpy(curr2->node->type, "float");
    }
}

void set(char* value){
    float num;
    strcpy(symbol2->node->value, value);

    if(!strcmp(value, ""));
    else if(!strcmp(value, "false")
       ||!strcmp(value, "true")) strcpy(symbol2->node->type, "bool");
    else if(value[0] == '"') strcpy(symbol2->node->type, "string");
    else if(value[0] == '\'') strcpy(symbol2->node->type, "char");
    else{
        num = atof(value);
        if(fmod(num, 1) == 0){
            strcpy(symbol2->node->type, "int");
        }
        else strcpy(symbol2->node->type, "float");
    }
}

void setType(char* type){
    strcpy(symbol->node->type, type);
}

void enqueueId(APTree* node){
    util = (struct identifiers*) malloc(sizeof(struct identifiers));
    util -> next = util -> prev = NULL;

    if (head == NULL){
        head = tail = util;
        util->nodeIdent = node;
    }//if
    else{
        tail -> next = util;
        util-> prev = tail;
        tail = util;
        util->nodeIdent = node;
    }//else

    currentID2 = util;
}

void displayIdentifiers(){
    for(a = head; a!=NULL; a = a->next)
    printf("IDENT: %s\t VALUE: %s\t TYPE: %s\n", a->nodeIdent->name, a->nodeIdent->value, a->nodeIdent->type);
}

void TypeCheckAndIsUsed(){
    int isFloat = 0, isString = 0, isChar = 0, isBool = 0, i = 0;
    char tp[20], tp2[20];
    if(head!=NULL){
        for(typeChecking = head; typeChecking!=NULL; typeChecking = typeChecking->next){
            if(typeChecking->isUsed != 1) notUsedError(typeChecking->nodeIdent->name);
            strcpy(tp, typeChecking->nodeIdent->value);
            strcpy(tp2, typeChecking->nodeIdent->type);
            for(i = 0; i < strlen(typeChecking->nodeIdent->value); i++){
                if(isalpha(typeChecking->nodeIdent->value[i])){
                    isString = 1;
                    break;
                }
                else if(typeChecking->nodeIdent->value[i]=='.'){
                    isFloat = 1;
                    break;
                }
                else if(typeChecking->nodeIdent->value[i]=='"'){
                    isString = 1;
                    break;
                }
                else if(typeChecking->nodeIdent->value[i]=='\''){
                    isChar = 1;
                    break;
                }
            }

            if(!strcmp(typeChecking->nodeIdent->value,"true")||!strcmp(typeChecking->nodeIdent->value,"false")) isBool = 1;

            if(!strcmp(typeChecking->nodeIdent->type, "int") && isFloat) sprintf(typeChecking->nodeIdent->value, "%d", atoi(typeChecking->nodeIdent->value));
            else if(isBool && !strcmp(typeChecking->nodeIdent->type, "bool"));
            else if((!isBool && !isChar && !isString && !strcmp(typeChecking->nodeIdent->type, "float")));
            else if((!isBool && !isChar && !isFloat && !isString && !strcmp(typeChecking->nodeIdent->type, "int")));
            else if((!isBool && isString && !strcmp(typeChecking->nodeIdent->type, "string")));
            else if((!isBool && isChar && !isString && !isFloat && !strcmp(typeChecking->nodeIdent->type, "char")));
            else if(typeChecking->nodeIdent->value[0] == '\0');
                else typeError(tp, tp2);
            isFloat = isString = isChar = isBool = 0;
        }
    }
    return 0;
}

int isDeclared(char* name){
    if(head!=NULL){
        for(declared = head; declared!=NULL; declared = declared->next){
            if(!strcmp(declared->nodeIdent->name, name)){
                currentID = declared;
                if(!declaration) declared->isUsed = 1;
                return 1;
            }
        }
        currentID = NULL;
    }
    sprintf(m, "Undeclared Variable < %s >", name);
    return 0;
}

void sError(){
    sprintf(errorMessage, "<line %d> -- %s!\n", symbol->node->lineNum, m);
    Message(errorMessage);
    semlength = semlength + strlen(errorMessage);
    setSem(1,semlength);
    errorCount++;
}//error()

void notUsedError(char var[]){
    sprintf(m, "Variable < %s > not used!", var);
    sprintf(errorMessage, "* %s!\n", m);
    Message(errorMessage);
    semlength = semlength + strlen(errorMessage);
    setSem(1,semlength);
    //errorCount++;
}//error()

void notInitialized(char var[]){
    sprintf(m, "Variable < %s > not initialized!", var);
    sprintf(errorMessage, "* %s!\n", m);
    Message(errorMessage);
    semlength = semlength + strlen(errorMessage);
    setSem(1,semlength);
    //errorCount++;
}//error()

void breakContinueError(char var[]){
    sprintf(m, "%s statement not within loop or switch!", var);
    sprintf(errorMessage, "<line %d> -- %s!\n", symbol->node->lineNum, m);
    Message(errorMessage);
    semlength = semlength + strlen(errorMessage);
    setSem(1,semlength);
    errorCount++;
}//error()

void typeError(char tp[], char tp2[]){
    sprintf(m, "Incompatible Types < %s > and < %s >", tp, tp2);
    sprintf(errorMessage, "<line %d> -- %s!\n", typeChecking->nodeIdent->lineNum, m);
    Message(errorMessage);
    semlength = semlength + strlen(errorMessage);
    setSem(1,semlength);
    errorCount++;
}//error()
