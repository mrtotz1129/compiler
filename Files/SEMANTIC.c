#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "SEMANTIC.h"

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

typedef struct tree{
    char name[50];
    int childCount;
    int level;
    char value[50], type[20];
    struct tree *children[100], *parent;
}APT;

struct tree *semanRoot;

struct tree *aCurr, *aRoot, *aTemp, *aTraverse;

struct identifiers{
    char name[50], type[50], value[50];
    struct identifiers *next, *prev;
}*head, *tail, *curr, *repeat, *z, *currType;

sruct sym{
    char lex[50],tok[50];
    int acceptState, lineNum;
    struct sym *next, *prev;
}*head, *tail, *curr, *symbol, *i;*/

struct identifiers *currIdentifier;

char lexeme[50], token[50], errorMsg[50], tempcurr[20], m[50], currcurrentName[50], currType[50];
int state, line, y = 0, errorCount= 0;
int log = 0, rel = 0, arith = 0, fNum = 0, iNum = 0;
char exprValue[50];

void getNext();
void getSym();
int match(char temp[50]);
int adMatch(char temp[50]);
void enqueuecurr();
int isDeclared();
int isNULL();
int isRepeated();
int typeCheck(char type[], char typeVal[]);
void getcurrType(char tempName[]);
void sError();
void getSym();
void do_smt();
void for_smt();
void while_smt();
void switch_smt();
void option();
void sns();

void smt();
void dec_smt();
void assign_smt();
void output_smt();
void input_smt();
void loop_smt();
void cond_smt();
void switch_smt();
int data_type();
int constant();
int expr();
int bool_val();
int arith_expr();
int rel_expr();
int log_expr();
int arith_operator();
int arith_operator1();
int rel_operator();
int log_operator();
int term();
int factor();
int rel_term();
int log_term();

int SEMANTIC(struct tree *tempRoot){
    semanRoot = tempRoot;
    remove("AnnonatedParseTree.sns");
    APT = fopen("AnnonatedParseTree.sns","aw+");
    head = tail = curr = NULL;
    aTraverse = aRoot = aTemp = aCurr = NULL;
    head = tail = curr = NULL;
    getSym();
    strcpy(m,"");
    t = head;
    sns();
    fclose(APT);
    return errorCountCount++;
}

void sns(){
    if(match("meth_main")){
    fprintf(APT, "SNS -> ");
        fprintf(APT, "meth_main");
        getNext();
    }
    if(match("start")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    smt();

    if(symbol->next == NULL){
        if(match("finish")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        }
    }

}//sns()

void smt(){
    if(data_type()){
        fprintf(APT,"\n
                SMT ( ");
        dec_smt();
    }
    else if(match("current")){
        fprintf(APT,"\nSMT ( ");
        assign_smt();
    }
    else if(match("st_print")){
        fprintf(APT,"\nSMT ( ");
        fprintf(APT, "OUTPUT_SMT(");
        output_smt();
    }
    else if(match("st_read")){
        fprintf(APT,"\nSMT ( ");
        input_smt();
    }
    else if(match("lp_while")
            ||match("lp_do")
            ||match("lp_for")){
        fprintf(APT,"\nSMT ( ");
        loop_smt();
    }
    else if(match("cond_if")){
        fprintf(APT,"\nSMT ( ");
        cond_smt();
    }
    else if(match("cond_swt")){
        fprintf(APT,"\nSMT ( ");
        switch_smt();
    }
    else{
        if(symbol->next == NULL) return;
        else getNext();
    }

    if(!match("finish")) smt();
        else return;
}

void cond_smt(){
    if(match("cond_if")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("oParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(log_expr()); else (rel_expr());
    if(match("cParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("cond_then")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    smt();
    if(match("cCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("cond_else")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();

        if(match("cond_if")) cond_smt();

        else if(match("oCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
            smt();
            if(match("cCurl")){
                fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                getNext();
            }
        }
    }
    fprintf(APT, " )");
}//COND

void switch_smt(){
    if(match("cond_swt")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("oParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("inval_current")
       ||match("inval_num")
       ||match("inval_sym")
       ||match("inval_str")
       ||match("inval_char")
       ||match("current")
       ||match("val_int")
       ||match("val_float")
       ||match("val_char")
       ||match("val_str")){
            //if(match("inval_current")||match("inval_num")||match("inval_sym")||match("inval_str")||match("inval_char")) error();
            fprintf(APT,"  %s( %s )", token, lexeme);
            getNext();
    }

    if(match("cParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("cond_opt")) option();

    if(match("cCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    fprintf(APT, " )");
}//SWITCH

void option(){
    if(match("cond_opt")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("inval_current")
       ||match("inval_num")
       ||match("inval_sym")
       ||match("inval_str")
       ||match("inval_char")
       ||match("current")
       ||match("val_int")
       ||match("val_float")
       ||match("val_char")
       ||match("val_str")){

        //if(match("inval_current")||match("inval_num")||match("inval_sym")||match("inval_str")||match("inval_char")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("colon")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    smt();
    if(match("st_break")||match("st_cont")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("term_sym")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("cCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("cond_opt")) option();
    else {
        if(match("cond_def")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        if(match("colon")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        if(match("oCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        smt();
        if(match("st_break")||match("st_cont")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        if(match("term_sym")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        if(match("cCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
    }
    fprintf(APT, " )");
}//option()

void loop_smt(){
    fprintf(APT, "LOOP_SMT( ");
    if(match("lp_while")) while_smt();
    else if(match("lp_do")) do_smt();
    else if(match("lp_for")) for_smt();
    fprintf(APT, " )");
}

void for_smt(){
    fprintf(APT, "FOR_SMT( ");
    if(match("lp_for")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("oParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();

        if(data_type()){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
        assign_smt();

        if(log_expr()); else (rel_expr());

        if(match("term_sym")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }

        if(match("current")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
            if(match("incre")||match("decre")){
                fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                getNext();
            }
            else if(match("op_assign")){
                fprintf(APT," %s( %s )", token, lexeme);
                getNext();
                if(!ARITH_EXPR());//;;;;
            }
        }
        else if(match("incre")||match("decre")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();

            if(match("current")){
                fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                getNext();
            }
        }

        if(match("cParen")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
    }

    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
        smt();
        if(match("cCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
    }
    fprintf(APT, " ) ");
}

void do_smt(){
    if(match("lp_do")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
        smt();
        if(match("cCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }

        if(match("lp_while")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
            if(match("oParen")){
                fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                getNext();

                if(log_expr()); else (rel_expr());

                if(match("cParen")){
                    fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                    getNext();
                }
                if(match("term_sym")){
                    fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
                    getNext();
                }
            }
        }
    }
    fprintf(APT, " ) ");
}

void while_smt(){
    if(match("lp_while")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("oParen")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();

        if(log_expr()); else (rel_expr());

        if(match("cParen")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
    }

    if(match("oCurl")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
        smt();
        if(match("cCurl")){
            fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
            getNext();
        }
    }
    fprintf(APT, " ) ");
}//WHILE

void input_smt(){
    fprintf(APT, "INPUT_SMT(");
    if(match("st_read")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("current")){
        strcpy(currcurrentName, symbol->lex);
        if(isDeclared()&&isNULL()){
            getcurrType(currcurrentName);
            fprintf(APT, "%s( %s.value = %s ) ",symbol->tok, symbol->lex, x->value);
            getNext();
        }
        else{
            fprintf(APT, "%s( %s.value = NULL ) ",symbol->tok, symbol->lex);
            sError();
            getNext();
        }
    }
    if(match("comma")){
        fprintf(APT," %s( %s ) )", symbol->tok , symbol->lex);
        getNext();
    }
    if(data_type()){
        if(head!=NULL) if(!typeCheck(currIdentifier->type, symbol->tok)) sError();
        fprintf(APT," %s( %s ) )", symbol->tok, symbol->lex);
        getNext();
    }
    if(match("term_sym")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    fprintf(APT, " )");
}//input_smt()

void output_smt(){
    if(match("st_print")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("current")){
        if(isDeclared()&&isNULL()){
            fprintf(APT, "%s( %s.value = %s ) ",symbol->tok, symbol->lex, x->value);
            if(head!=NULL) strcpy(curr->value,symbol->lex);
            getNext();
        }
        else{
            fprintf(APT, "%s( %s.value = NULL ) ",symbol->tok, symbol->lex);
            if(head!=NULL) strcpy(curr->value,"NULL");
            sError();
            getNext();
        }
    }
    else if(match("val_str")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }

    if(match("ccat_out")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
        output_smt();
    }
    else if(match("term_sym")){
        fprintf(APT," %s( %s )", symbol->tok, symbol->lex);
        getNext();
    }
    fprintf(APT, " )");
}//output_smt()

void assign_smt(){
    if(match("current")
       ||match("inval_int")){
        fprintf(APT, "ASSIGN_SMT( ");
        strcpy(currcurrentName,symbol->lex);

        if(isDeclared()){
            fprintf(APT, "%s( %s.value = %s ) ",symbol->tok, symbol->lex, x->value);
            getNext();
        }
        else{
            fprintf(APT, "%s( %s.value = NULL ) ",symbol->tok, symbol->lex);
            if(head!=NULL) strcpy(curr->value,"NULL");
            sError();
            getNext();
        }

        if(match("incre")
           ||match("decre")){
            fprintf(APT, "%s( %s ) ) ",symbol->tok, symbol->lex);
            getNext();
            if(match("term_sym")){
                fprintf(APT, "%s( %s ) ) ",symbol->tok, symbol->lex);
                getNext();
            }
        }
        else if(match("op_assign")){
            fprintf(APT, "%s( %s ) ) ",symbol->tok, symbol->lex);
            getNext();
            if(expr()){
                //getNext();
                if(log||rel){
                    if(!typeCheck(currIdentifier->type, "dt_bool")) sError();
                    log = rel = 0;
                }
                else if(arith){
                    if(iNum){
                        if(!typeCheck(currIdentifier->type, "dt_int")) sError();
                    }
                    else if(fNum){
                        if(!typeCheck(currIdentifier->type, "dt_float")) sError();
                    }
                    arith = iNum = fNum = 0;
                }
            }
            else if(constant()){
                getcurrType(currcurrentName);
                if(head!=NULL) if(!typeCheck(tempcurr,symbol->tok)) sError();
                fprintf(APT, "%s( value = %s ) ) ",symbol->tok, symbol->lex);
                strcpy(curr->value, symbol->lex);
                getNext();
            }
            else if(match("current")
            ||match("inval_current")){
                if(isDeclared()&&isNULL()){
                    getcurrType(currcurrentName);
                    if(!typeCheck(tempcurr, currIdentifier->type)) sError();
                    fprintf(APT, "%s( %s.value = %s ) ",symbol->tok, symbol->lex, x->value);
                    printf("%s\t%s\t%s\n",n->name, n ->value, n->type);
                    strcpy(n->value, currIdentifier->value);
                    getNext();printf("%s\t%s\t%s\n",currIdentifier->name,currIdentifier->value,currIdentifier->type);
                }
                else{
                    if(head!=NULL) strcpy(curr->value,"NULL");
                    sError();
                    getcurrType(currcurrentName);
                    if(head!=NULL) if(!typeCheck(curr->type, tempcurr)) sError();
                    getNext();
                }
            }
            if(match("term_sym")){
                fprintf(APT, "%s( %s )",symbol->tok, symbol->lex);
                getNext();
            }
        }
    }
    fprintf(APT, " )");
}

void dec_smt(){
    if(data_type()){
        fprintf(APT, "DEC_SMT( ");
        fprintf(APT, "DATA_TYPE( ");
        fprintf(APT, "%s( %s ) ) ",symbol->tok, symbol->lex);
        strcpy(currType, symbol->tok);
        getNext();
    }

    if(match("term_sym")){
        fprintf(APT, "%s ( %s ) ",symbol->tok, symbol->lex);
        getNext();
    }
    else if(match("current")){
        fprintf(APT, "%s ( %s ) ",symbol->tok, symbol->lex);
        if(isRepeated()11) sError();
        else enqueuecurr();
        //printf("%s\t%s\t%s\n",curr->type,curr->name,curr->value);
        getNext();

        if(match("op_assign")){
            fprintf(APT, "%s ( %s ) ",symbol->tok, symbol->lex);
            getNext();
        }
        if(expr()){
            //getNext();
            if(log||rel){
                if(!typeCheck(curr->type, "dt_bool")) sError();
                log = rel = 0;
            }
            else if(arith){
                if(iNum){
                    if(!typeCheck(curr->type, "dt_int")) sError();
                }
                else if(fNum){
                    if(!typeCheck(curr->type, "dt_float")) sError();
                }
                arith = iNum = fNum = 0;
            }
            if(match("comma")){
                fprintf(APT, "%s( %s ) ",symbol->tok, symbol->lex);
                getNext();
                dec_smt();
                return;
            }
        }
        else if(constant()){
            if(!typeCheck(curr->type,symbol->tok)) sError();
            fprintf(APT, "%s( value = %s ) ) ",symbol->tok, symbol->lex);
            strcpy(curr->value,symbol->lex);
            getNext();
            if(match("comma")){
                fprintf(APT, "%s( %s ) ",symbol->tok, symbol->lex);
                getNext();
                dec_smt();
                return;
            }
        }
        else if(match("current")||match("inval_current")){
            if(isDeclared()&&isNULL()){
                getcurrType(symbol->lex);
                if(head!=NULL) if(!typeCheck(curr->type, tempcurr)) sError();
                fprintf(APT, "%s( %s.value = %s ) ",symbol->tok, symbol->lex, x->value);
                if(head!=NULL) strcpy(curr->value,currIdentifier->value);
                getNext();
            }
            else{
                fprintf(APT, "%s( %s.value = NULL ) ",symbol->tok, symbol->lex);
                if(head!=NULL) strcpy(curr->value, "NULL");
                sError();
                getNext();
            }
            if(match("comma")){
                fprintf(APT, "%s( %s ) ",symbol->tok, symbol->lex);
                getNext();
                dec_smt();
                return;
            }
        }
        else if(match("comma")){
            getNext();
            dec_smt();
        }
    }
    if(match("term_sym")){
        fprintf(APT, "%s( %s )",symbol->tok, symbol->lex);
        getNext();
    }
    fprintf(APT, " )");
}

int data_type(){
    if(match("dt_bool")
       ||match("dt_int")
       ||match("dt_float")
       ||match("dt_char")
       ||match("dt_string")){
        return 1;
    }
    return 0;
}
int bool_val(){
    if(match("bl_true")
       ||match("bl_false")){
        return 1;
    }return 0;
}//BOOL_VAL()


int constant(){
    if(match("val_int")
       ||match("val_float")
       ||match("val_str")
       ||match("val_char")
       ||match("inval_sym")
       ||match("inval_num")
       ||match("inval_str")
       ||match("inval_char")
       ||BOOL_VAL()){
       if(!strcmp(token,"inval_sym")
          ||!strcmp(token,"inval_num")
          ||!strcmp(token,"inval_str")
          ||!strcmp(token,"inval_char")){
          error();
        }


        insertASTNode(current, "CONST", currensymbol->level + 1, 1);

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);

        current = currensymbol->parensymbol->parent;

        getNextToken();
        return 1;
    }
    return 0;
}//CONST()

int expr(){
    insertASTNode(current, "EXPR", currensymbol->level + 1, 1);

    if(LOG_EXPR()){
        isAcceptedExpr = 0;
        current = currensymbol->parent;
        return 1;
    }
    isAcceptedExpr = 0;
    current = currensymbol->parent;
    return 0;
}//EXPR()

int log_expr(){
    isLog = 0;
    insertASTNode(current, "LOG_EXPR", currensymbol->level + 1, 1);

    if(LOG_TERM()) isAcceptedExpr = 1;

    if(match("log_not")){

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parent;
        getNextToken();
    }

    chk = fileline-1;

    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"cParen"))
            break;
        if(!strcmp(chkexp,"log_and")
           ||!strcmp(chkexp,"log_or")){
            isLog = 1;
            break;
        }
        if(!strcmp(chkexp,"term_sym")
           ||!strcmp(chkexp,"oCurl")
           ||!strcmp(chkexp,"cCurl")){
            break;
       }
    }

    if(isLog==0){
        isLog = 0;
        current = currensymbol->parent;
        return (isAcceptedExpr == 1)? 1: 0;
    }
    else isLog = 0;

    if(isAcceptedExpr) isAcceptedExpr = 0;

    if(log_operator()){
       current = currensymbol->parent;//asdasdasdasdasdasdas
        return 1;
    }
    current = currensymbol->parent;
    return 0;
}//LOG_EXPR()

int LOG_TERM(){
    insertASTNode(current, "LOG_TERM", currensymbol->level + 1, 1);

    if(REL_EXPR()){
        current =  currensymbol->parent;
        return 1;
    }

    current =  currensymbol->parent;
    return 0;
}//LOG_TERM()

int log_operator(){
    if(match("log_and")
       ||match("log_or")){
        insertASTNode(current, "log_operator", currensymbol->level + 1, 1);

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parensymbol->parent;


        getNextToken();

        if(match("log_not")){

            insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
            current = currensymbol->parent;
            getNextToken();
        }
        if(LOG_TERM()){
            current = currensymbol->parent;
            return 1;
        }
        current = currensymbol->parent;
        return 0;
    }
    current = currensymbol->parent;
    return 0;
}//log_operator ()

int REL_EXPR(){
    isRel = 0;
    insertASTNode(current, "REL_EXPR", currensymbol->level + 1, 1);

    if(REL_TERM()) isAcceptedExpr = 1;

    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"cParen"))
            break;
        /*if(!strcmp(chkexp,"log_not")
           ||!strcmp(chkexp,"log_and")
           ||!strcmp(chkexp,"log_or")){
               isRel = 0;//printf("%s\t%s\n",currensymbol->data,lexeme);
               break;
        }*/ /*
        if(!strcmp(chkexp,"rel_not")
           ||!strcmp(chkexp,"rel_great")
           ||!strcmp(chkexp,"rel_gqual")
           ||!strcmp(chkexp,"rel_less")
           ||!strcmp(chkexp,"rel_equal")
           ||!strcmp(chkexp,"rel_lqual")){
                isRel = 1;
                break;
        }
        if(!strcmp(chkexp,"term_sym")
           ||!strcmp(chkexp,"oCurl")
           ||!strcmp(chkexp,"cCurl")){
                break;
        }
    }

    if(isRel==0){
        isRel = 0;
            //current = currensymbol->parent;//asfasfasfasfas

        return (isAcceptedExpr == 1)? 1: 0;
    }
    else isRel = 0;

    if(isAcceptedExpr) isAcceptedExpr = 0;

    if(rel_operator()){
        current = currensymbol->parent;//asfasfasfasfas
        return 1;
    }
    current = currensymbol->parent;
    return 0;
}

int rel_operator(){
    if(match("rel_not")
       ||match("rel_great")
       ||match("rel_gqual")
       ||match("rel_less")
       ||match("rel_lqual")
       ||match("rel_equal")){
        insertASTNode(current, "rel_operator", currensymbol->level + 1, 1);//ETTOTOTOT

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parensymbol->parent;
        getNextToken();

        if(REL_TERM()){
            current = currensymbol->parent;
            return 1;
        }
        current = currensymbol->parent;
        return 0;
    }
    current = currensymbol->parent;
    return 0;
}//REL_TERM()

int REL_TERM(){
    insertASTNode(current, "REL_TERM", currensymbol->level + 1, 1);

    if(ARITH_EXPR()){
        current = currensymbol->parent;
        return 1;
    }
    current = currensymbol->parent;
    return 0;
}//REL_TERM

int ARITH_EXPR(){
    isArith = 0;
    insertASTNode(current, "ARITH_EXPR", currensymbol->level + 1, 1);

    if(TERM()) isAcceptedExpr = 1;

    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"cParen"))
            break;
        /*if(!strcmp(chkexp,"log_not")
           ||!strcmp(chkexp,"log_and")
           ||!strcmp(chkexp,"log_or")
           ||!strcmp(chkexp,"rel_equal")
           ||!strcmp(chkexp,"rel_great")
           ||!strcmp(chkexp,"rel_qqual")
           ||!strcmp(chkexp,"rel_less")
           ||!strcmp(chkexp,"rel_lqual")
           ||!strcmp(chkexp,"rel_not")){
               isArith = 0;
               break;
        }*/ /*
        if(!strcmp(chkexp,"math_add")
           ||!strcmp(chkexp,"math_sub")
           ||!strcmp(chkexp,"math_mul")
           ||!strcmp(chkexp,"math_div")
           ||!strcmp(chkexp,"math_mod")){
                isArith = 1;
                break;
        }
        if(!strcmp(chkexp,"term_sym")
           ||!strcmp(chkexp,"oCurl")
           ||!strcmp(chkexp,"cCurl")){
                break;
       }
    }

    if(isArith == 0){
        isArith = 0;
        current = current -> parent;
        return (isAcceptedExpr == 1)? 1: 0;
    }
    else isArith = 0;

    if(isAcceptedExpr) isAcceptedExpr = 0;

    if(OPERATOR()){
        current = currensymbol->parent;
        return 1;
    }

    current = currensymbol->parent;//eto pala
    return 0;
}//ARITH_OPERATOR()

int TERM(){
    insertASTNode(current, "TERM", currensymbol->level + 1, 1);

    if(FACTOR()){
        if(OPERATOR1()){
            current = currensymbol->parent;
            return 1;
        }
        current = currensymbol->parent;
        return 1;
    }

    current = currensymbol->parent;
    return 0;
}//TERM()

int OPERATOR(){
    if(match("math_add")
       ||match("math_sub")){
       insertASTNode(current, "OPERATOR", currensymbol->level + 1, 1);

       insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
       current = currensymbol->parensymbol->parent;
       getNextToken();

       if(TERM()){
            return 1;
       }
       return 0;
    }

    return 0;
}

int OPERATOR1(){
    if(match("math_mul")
       ||match("math_div")
       ||match("math_mod")){
        insertASTNode(current, "OPERATOR1", currensymbol->level + 1, 1);

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parensymbol->parent;
        getNextToken();

        if(FACTOR()){
            current = currensymbol->parent;
            return 1;
        }

        current = currensymbol->parent;
        return 0;
   }

   return 0;
}

int FACTOR(){
    insertASTNode(current, "FACTOR", currensymbol->level + 1, 1);

    if(CONST()){
        current = currensymbol->parent;
        return 1;
    }
    else if(UNARY_OP()){
        if(match("current")){

            insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
            current = currensymbol->parent;
        }
        else error();
        current = currensymbol->parent;
        return 1;
    }
    else if(match("current")){

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parent;
        getNextToken();
        if(UNARY_OP());
        current = currensymbol->parent;
        return 1;
    }
    else if(match("oParen")){
        y = 1;

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parent;
        getNextToken();

        if(EXPR());

        if(match("cParen")){
            y = 0;
            insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
            current = currensymbol->parent;
            getNextToken();
        }
        else{
            y = 0;
            error();
        }
        current = currensymbol->parent;
        return 1;
    }
    current = currensymbol->parent;
    return 0;
}
//mamaya na

int UNARY_OP(){
    if(match("incre")
       ||match("decre")){
        insertASTNode(current, "UNARY_OP", currensymbol->level + 1, 1);

        insertASTNode(current, token, lexeme, "", currensymbol->level + 1, 0);
        current = currensymbol->parensymbol->parent;
        getNextToken();
        return 1;
   }
    return 0;
}


void enqueuecurr(){
        curr = (struct identifiers*) malloc(sizeof(struct identifiers));
        curr -> next = curr -> prev = NULL;

        if (head == NULL){
            head = tail = curr;
            strcpy(curr->value, "NULL");
            strcpy(curr->name, symbol->lex);
            strcpy(curr->type, currType);
        }//if
        else{
            tail -> next = curr;
            curr -> prev = tail;
            tail = curr;
            strcpy(curr->value, "NULL");
            strcpy(curr->name, symbol->lex);
            strcpy(curr->type, currType);
        }//else
}

void getNext(){
    t = (symbol->next != NULL)? symbol->next : t;
}

int isDeclared(){
    if(head!=NULL){
        for(x = head; x!=NULL; x = x->next){
            if(!strcmp(symbol->lex, x->name)){
                currIdentifier = x;
                return 1;
            }
        }
    }
    sprintf(m, "Variable '%s' Undeclared.", symbol->lex);
    return 0;
}

int isRepeated(){
    if(head!=NULL){
        for(z = head; z!=NULL; z = z->next){
            if(!strcmp(symbol->lex, z->name)){
                sprintf(m, "Multiple Declaration of Variable '%s'.", symbol->lex);
                return 1;
            }
        }
    }
    return 0;
}

int isNULL(){
    if(x!=NULL)
        if(strcmp(x->value,"NULL")) return 1;
    sprintf(m, "Variable '%s' has no value.", symbol->lex);
    return 0;
}

int typeCheck(char type[], char typeVal[]){
    if(!strcmp(type, "dt_int")&&!strcmp(typeVal, "val_int")) return 1;
        else if(!strcmp(type, "dt_int")&&!strcmp(typeVal, "dt_int")) return 1;
    else if(!strcmp(type, "dt_float")&&!strcmp(typeVal, "val_float")) return 1;
    else if(!strcmp(type, "dt_float")&&!strcmp(typeVal, "val_int")) return 1;
        else if(!strcmp(type, "dt_float")&&!strcmp(typeVal, "dt_float")) return 1;
    else if(!strcmp(type, "dt_string")&&!strcmp(typeVal, "val_str")) return 1;
        else if(!strcmp(type, "dt_string")&&!strcmp(typeVal, "dt_string")) return 1;
    else if(!strcmp(type, "dt_char")&&!strcmp(typeVal, "val_char")) return 1;
        else if(!strcmp(type, "dt_char")&&!strcmp(typeVal, "dt_char")) return 1;
    else if(!strcmp(type, "dt_bool")&&!strcmp(typeVal, "bl_true")) return 1;
    else if(!strcmp(type, "dt_bool")&&!strcmp(typeVal, "bl_false")) return 1;
        else if(!strcmp(type, "dt_bool")&&!strcmp(typeVal, "dt_bool")) return 1;
    else{
        sprintf(m, "Incompatible types.");
        return 0;
    }
}

void getcurrType(char tempName[]){
    if(head!=NULL){
        for(currType = head; currType!=NULL; currType = currType->next){
            if(!strcmp(n->name, tempName)){
                strcpy(tempcurr, n->type);
                return;
            }
        }
    }
    return;
}

void sError(){
    sprintf(errorMsg, "<line %d> -- %s!\n", symbol->lineNum, m);
    Message(errorMsg);
    errorCountCount++;
}//error()

void getSym(){
    SymbolTable = fopen("SymbolTable.sns","r+");
    while(fscanf(SymbolTable, "%[^\t] %s %d %d\n",lexeme, token, &state, &line) != EOF){
        if(!strcmp(token,"com_block")||!strcmp(token,"com_single")||!strcmp(lexeme,"LEXEME")||!strcmp(lexeme,"STATE")) continue;
        curr = (struct sym*) malloc(sizeof(struct sym));
        curr -> next = curr -> prev = NULL;
        if (head == NULL){
            head = tail = curr;
            strcpy(curr->lex, lexeme);
            strcpy(curr->tok, token);
            curr->acceptState = state;
            curr->lineNum = line;
        }//if
        else{
            tail -> next = curr;
            curr -> prev = tail;
            tail = curr;
            strcpy(curr->lex, lexeme);
            strcpy(curr->tok, token);
            curr->acceptState = state;
            curr->lineNum = line;
        }//else
    }
    fclose(SymbolTable);
}//getSym()


*/
void insertASPT(struct tree *tempPT){

    APtree *node1;
    node1 = newASTNode(tempPT->data, "", "", tempPT->level);


    if(tempParent!=NULL){
        node1->parent = tempParent;
        tempParent->children[tempParent->childCount] = node1;
        tempParent->childCount++;
    }
    else root = node1;
}

for(){
}

Tree* newASTNode(char* token, char* value, char* type int level) {
	AST *node;
	int i;
    node = (struct tree*) malloc(sizeof(struct tree));
    for(i = 0; i < 100; i++){
        node->children[i] = NULL;
    }
    node->childCount = 0;
    node->level = level;
    node->parent = NULL;
    strcpy(node->value, value);
    strcpy(node->token, token);
    strcpy(node->type, type);

	return node;
}


