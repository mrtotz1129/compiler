#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"SYNTACTIC.h"

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

FILE *SymTable;
int fileline = 0, state, line, count = 0;
int  errorCount = 0, adLine = 0, chk = 0, adln, adst, x, y;
int logParen = 0, relParen = 0, arithParen = 0, templine = 0, k = 0;
char  lexeme[50], token[30], code[30], stmt[30], msg[30], errorMessage[100];
char  adLex[50], adTok[30], chkexp[30];


int setCode(char temp[]);
int isMatch(char temp[]);
int isAdMatch(char temp[]);
void insertNode(char tempData[], int i);
int SYNTACTIC();
void error();
void backTrack();

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

/*struct parseTree{
    char data[50];
    struct parseTree *parent;
    struct parseTree *child[10];
}*root, *currNode, *traverse, *temp;*/

int SYNTACTIC(){
    fileline = 0, count = 0, errorCount = 0, adLine = 0;
    //root = currNode = NULL;
    countLines();
    getNextToken();

    SNS();

    return errorCount;
}//main()

void countLines(){
    SymTable = fopen("SymbolTable.sns","r+");
    char temp[127];
    while(fgets(temp, 127, SymTable) != NULL) count++;
    count--;
    fclose(SymTable);
}

void advanced(){
    SymTable = fopen("SymbolTable.sns","r+");
    x = -1;
    adLine = fileline+2;
    while(fscanf(SymTable, "%[^\t] %s %d %d\n",adLex, adTok, &adst, &adln) != EOF){
        if( x < 1){
            x++;
            continue;
        }
        if(adLine == x+1) break;
        x++;
    }
    if(!strcmp(token,"com_block")||!strcmp(token,"com_single")) advanced();

    fclose(SymTable);
}//advanced()

void chkEXP(){
    SymTable = fopen("SymbolTable.sns","r+");
    x = -1;
    while(fscanf(SymTable, "%[^\t] %s %d %d\n",adLex, chkexp, &adst, &adln) != EOF){
        if( x < 1){
            x++;
            continue;
        }
        if(chk == x) break;
        x++;
    }
    if(!strcmp(token,"com_block")||!strcmp(token,"com_single")) advanced();

    fclose(SymTable);
}//advanced()

void getNextToken(){
    SymTable = fopen("SymbolTable.sns","r+");
    fileline++;
    x = -1;
    while(fscanf(SymTable, "%[^\t] %s %d %d\n",lexeme, token, &state, &line) != EOF){
        if( x < 1){
            x++;
            continue;
        }
        if(fileline == x) break;
        x++;
    }
    if(!strcmp(token,"com_block")||!strcmp(token,"com_single")) getNextToken();

    fclose(SymTable);
}//getNextToken()

void error(){
    errorCount++;
    sprintf(errorMessage, "<line %d> -- %s %s\n", line, msg, code);
    Message(errorMessage);
}//error()

int setCode(char temp[]){
    if(!strcmp(temp, "term_sym")||!strcmp(temp, "comma")||!strcmp(temp, "op_assign")||!strcmp(temp, "colon")){
        strcpy(code, "Symbol ';', ',', '=','&'");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "oParen")){
        strcpy(code, "Symbol '('");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "cParen")){
        strcpy(code, "Symbol ')'");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "oCurl")){
        strcpy(code, "Symbol '{'");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "cCurl")){
        strcpy(code, "Symbol '}'");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "ccat_out")){
        strcpy(code, "Symbol '&'");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp, "dt_bool")||!strcmp(temp, "dt_char")||!strcmp(temp, "dt_float")||!strcmp(temp, "dt_int")||!strcmp(temp, "dt_string")){
        strcpy(code, "Data Type.");
        strcpy(msg, "Expected: ");
    }
    else if(!strcmp(temp,"inval_ident")||
           !strcmp(temp,"inval_sym")||
           !strcmp(temp,"inval_num")||
           !strcmp(temp,"inval_str")||
           !strcmp(temp,"inval_char")){
        strcpy(msg,"Unexpected: ");
        strcpy(code,"Lexical Error");
    }
    else if(!strcmp(stmt,"output")){
        strcpy(code, "Parameters");
        strcpy(msg, "Too Few arguments: ");
    }
    else if(!strcmp(stmt,"assign")){
        strcpy(code, "Expression");
        strcpy(msg, "Expected: ");
    }
    else strcpy(code, temp);

    strcat(code, ".");

    return (isMatch(temp))?1:0;
}//setCode()

int isMatch(char temp[]){
    if(!strcmp(token, temp)){
        //printf("%s, ",token);
        return 1;
    } return 0;
}//isMatch()

int isAdMatch(char temp[]){
    if(!strcmp(adTok, temp)){
        //printf("%s, ",token);
        return 1;
    } return 0;
}//isAdMatch()


void SNS(){
    if(setCode("meth_main")) getNextToken(); else error();
    if(setCode("oCurl")) getNextToken(); else error();

    SMT();

    if(fileline==count){
        if(setCode("cCurl")) getNextToken();
    }else error();

}//SNS()

void SMT(){
    strcpy(stmt,"");
    if(DATA_TYPE())  DEC_SMT();
    else if(isMatch("st_read")) INPUT_SMT();
    else if(isMatch("st_print")) OUTPUT_SMT();
    else if(isMatch("ident")||isMatch("inval_ident")) ASSIGN_SMT();
    else if(isMatch("lp_while")||isMatch("lp_do")||isMatch("lp_for")) LOOP_SMT();
    else if(isMatch("cond_if")) COND_SMT();
    else if(isMatch("cond_swt")) SWITCH_SMT();
    else if(SYMBOLS()) error();
    else return;

    if(!isMatch("cCurl")) SMT();
}//SMT()

void DEC_SMT(){
    strcpy(stmt,"declaration");

    if(DATA_TYPE()) getNextToken();

    if(isMatch("inval_ident")||setCode("ident")){
        if(setCode("inval_ident")) error();
        getNextToken();

        if(setCode("term_sym")) getNextToken();
        else if(setCode("op_assign")){
            getNextToken();
            if(EXPR()){
                if(setCode("term_sym")) getNextToken();
                else if(setCode("comma")){
                    getNextToken();
                    DEC_SMT();
                }
                else error();
            }
            else if(CONST()){
                getNextToken();
                if(setCode("term_sym")) getNextToken();
                else if(setCode("comma")){
                    getNextToken();
                    DEC_SMT();
                }
                else error();
            }
            else if(setCode("ident")||isMatch("inval_ident")){
                if(setCode("inval_ident")) error();
                getNextToken();
            }
            else error();
        }
        else if(setCode("comma")){
            getNextToken();
            DEC_SMT();
        }
        else error();
    }else error();
    if(setCode("term_sym")) getNextToken();
}//DEC_SMT()

void INPUT_SMT(){
    strcpy(stmt,"input");
    if(isMatch("st_read"))  getNextToken();
    if(setCode("ident")) getNextToken(); else error();
    if(setCode("term_sym")) getNextToken(); else error();
}//INPUT_SMT()

void OUTPUT_SMT(){
    strcpy(stmt,"output");
    if(isMatch("st_print")) getNextToken();

    if(setCode("ident")) getNextToken();
    else if(setCode("val_str")) getNextToken();
    else error();
    if(setCode("ccat_out")){
        getNextToken();
        OUTPUT_SMT();
    }
    else if(setCode("term_sym")) getNextToken();
    else error();
}//OUTPUT_SMT()

void ASSIGN_SMT(){
    strcpy(stmt, "assign");
    if(setCode("ident")||isMatch("inval_ident")){
        if(setCode("inval_ident")) error();
        getNextToken();
        if(isMatch("incre")||isMatch("decre")){
            getNextToken();
            if(setCode("term_sym")){
                getNextToken();
            }else error();
        }
        else if(setCode("op_assign")){
            getNextToken();

            if(EXPR());
            else if(setCode("ident")){
                getNextToken();
            }
            else if(CONST()){
                getNextToken();
            }
            else error();
            if(setCode("term_sym")) getNextToken(); else error();
        }
    }
}//ASSIGN_SMT()

void LOOP_SMT(){
    if(isMatch("lp_while")) WHILE_SMT();
    else if(isMatch("lp_do")) DO_SMT();
    else if(isMatch("lp_for")) FOR_SMT();
}

void FOR_SMT(){
    if(isMatch("lp_for")) getNextToken();

    if(setCode("oParen")){
        getNextToken();
        if(DATA_TYPE()) getNextToken();
        ASSIGN_SMT();

        if(LOG_EXPR()); else (REL_EXPR());

        if(setCode("term_sym")) getNextToken(); else error();

        if(setCode("ident")){
            getNextToken();
            if(isMatch("incre")||isMatch("decre")){
                getNextToken();
            }
            else if(setCode("op_assign")){
                getNextToken();
                if(!ARITH_EXPR()) error();
            }
        }
        else if(isMatch("incre")||isMatch("decre")){
            getNextToken();
            if(setCode("ident")){
                getNextToken();
            }else error();
        }

        if(setCode("cParen")) getNextToken(); else error();
    }else error();

    if(setCode("oCurl")){
        getNextToken();
        SMT();
        if(setCode("cCurl")) getNextToken();
            else error();
    }
}

void DO_SMT(){
    if(isMatch("lp_do")) getNextToken();
    if(setCode("oCurl")){
        getNextToken();
        SMT();
        if(setCode("cCurl")){
            getNextToken();
        }else error();

        if(isMatch("lp_while")){
            getNextToken();
            if(setCode("oParen")){
                getNextToken();

                if(LOG_EXPR()); else (REL_EXPR());

                if(setCode("cParen")){
                    getNextToken();
                    if(setCode("term_sym")){
                        getNextToken();
                    }else error();
                }else error();
            }else error();
        }else error();
    }else error();
}

void WHILE_SMT(){
    if(isMatch("lp_while")) getNextToken();
    if(setCode("oParen")){
        getNextToken();

        if(LOG_EXPR()); else (REL_EXPR());

        if(setCode("cParen")){
            getNextToken();
        }else error();
    }else error();

    if(setCode("oCurl")){
        getNextToken();
        SMT();
        if(setCode("cCurl")){
            getNextToken();
        }else error();
    }
    else error();
}//WHILE

void COND_SMT(){
    if(isMatch("cond_if")) getNextToken();
    if(setCode("oParen")) getNextToken(); else error();
    if(LOG_EXPR()); else (REL_EXPR());
    if(setCode("cParen")) getNextToken(); else error();

    if(setCode("cond_then")) getNextToken();

    if(setCode("oCurl")) getNextToken(); else error();
    SMT();
    if(setCode("cCurl")) getNextToken(); else error();

    if(isMatch("cond_else")){
        getNextToken();

        if(isMatch("cond_if")) COND_SMT();
        else if(setCode("oCurl")){
            getNextToken();
            SMT();
            if(setCode("cCurl")){
                getNextToken();
            }else error();
        }else error();
    }
}//COND

void SWITCH_SMT(){
    if(setCode("cond_swt")) getNextToken();
    if(setCode("oParen")) getNextToken(); else error();
    if(setCode("inval_ident")||setCode("inval_num")||setCode("inval_sym")||setCode("inval_str")||setCode("inval_char")||
      setCode("ident")||setCode("val_int")||setCode("val_float")||setCode("val_char")||setCode("val_str")){
            if(isMatch("inval_ident")||isMatch("inval_num")||isMatch("inval_sym")||isMatch("inval_str")||isMatch("inval_char")) error();
            getNextToken();
    }else error();

    if(setCode("cParen")) getNextToken(); else error();
    if(setCode("oCurl")) getNextToken(); else error();
    if(isMatch("cond_opt")) OPTION();

    if(setCode("cCurl")) getNextToken(); else error();

}//SWITCH

void OPTION(){
    if(setCode("cond_opt")) getNextToken();
    if(setCode("inval_ident")||setCode("inval_num")||setCode("inval_sym")||setCode("inval_str")||setCode("inval_char")||
       setCode("ident")||setCode("val_int")||setCode("val_float")||setCode("val_char")||setCode("val_str")){
        if(isMatch("inval_ident")||isMatch("inval_num")||isMatch("inval_sym")||isMatch("inval_str")||isMatch("inval_char")) error();
        getNextToken();
    }else{
        strcpy(msg, "Expected: ");
        strcpy(code, "Ident");
        error();
    }
    if(setCode("colon")) getNextToken(); else error();
    if(setCode("oCurl")) getNextToken(); else error();
    SMT();
    if(setCode("st_break")||setCode("st_cont")) getNextToken();
    if(setCode("cCurl")) getNextToken(); else error();
    if(isMatch("cond_opt")) OPTION();
    else{
        if(setCode("cond_def")){
            getNextToken();
            if(setCode("colon")) getNextToken(); else error();
            if(setCode("oCurl")) getNextToken(); else error();
            SMT();
            if(setCode("cCurl")) getNextToken(); else error();
        }
    }
}//OPTION()

int DATA_TYPE(){
    if(isMatch("dt_bool")||isMatch("dt_char")||isMatch("dt_float")||isMatch("dt_int")||isMatch("dt_string")){
        return 1;
    }
    return 0;
}//DATA_TYPE()

int BOOL_VAL(){
    if(setCode("bl_true")||setCode("bl_false")){
        return 1;
    }return 0;
}//BOOL_VAL()

int CONST(){
    if(setCode("val_int")||setCode("val_float")||setCode("val_str")||
       setCode("val_char")||setCode("inval_sym")||setCode("inval_num")||
       setCode("inval_str")||setCode("inval_char")||BOOL_VAL()){
       if(!strcmp(token,"inval_sym")||
          !strcmp(token,"inval_num")||
          !strcmp(token,"inval_str")||
          !strcmp(token,"inval_char")){
          error();
        }
        return 1;
    }
    return 0;
}//CONST()

int EXPR(){
    if(LOG_EXPR()) return 1;
    else if (REL_EXPR()) return 1;
    else if (ARITH_EXPR()) return 1;
    return 0;
}//EXPR()

int LOG_EXPR(){
    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"log_and")||!strcmp(chkexp,"log_or")){
            y = 1; break;
        }
        if(!strcmp(chkexp,"term_sym")||!strcmp(chkexp,"oCurl")||!strcmp(chkexp,"cCurl")) break;
    }
    if(y==0) return 0;
    else y = 0;

    if(setCode("oParen")){
        templine = fileline;
        advanced();
        if(adMatch("oParen")){
            getNextToken();
            logParen = 1;
        }
        else if(!REL_EXPR()){
            fileline = templine;
            getNextToken();
            logParen = 1;
        }
        else{
            fileline = templine - 1;
            getNextToken();
        }
    }

    /*if(setCode("log_not")){
         getNextToken();
         if(LOG_TERM());
         if(isMatch("term_sym")) return 1;
         else {
            if(LOG_EXPR()) return 1;
                else if(REL_EXPR()) return 1;
         }
    }*/

    if(LOG_TERM()); else error();

    if(LOG_OPERATOR()){
        getNextToken();
        if(LOG_EXPR()||LOG_TERM()){
            if(setCode("cParen")&&logParen){
                getNextToken();
                logParen = 0;
            }
            else if(setCode("cParen")){
                getNextToken();
                if(!logParen){
                    strcpy(msg,"Unexpected: ");
                    strcpy(code,"')'");
                    logParen = 0;
                    error();
                }
                else if(logParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    logParen = 0;
                    error();
                }
            }
            else{
                if(logParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    logParen = 0;
                    error();
                }
            }
            return 1;
        }else return 0;
    }

    if(setCode("cParen")&&logParen){
        getNextToken();
        logParen = 0;
    }
    else if(setCode("cParen")){
        if(!logParen){
            strcpy(msg,"Unexpected: ");
            strcpy(code,")");
            logParen = 0;
            error();
        }
        else if(logParen){
            strcpy(msg,"Expected: ");
            strcpy(code,")");
            logParen = 0;
            error();
        }
    }
    else{
        if(logParen){
            strcpy(msg,"Expected: ");
            strcpy(code,"')'");
            logParen = 0;
            error();
        }
    }
    return 0;
}//LOG_EXPR()

int LOG_TERM(){
    if(REL_EXPR()){
        return 1;
    }
    else if(setCode("ident")||setCode("val_int")||setCode("val_float")||BOOL_VAL()){
        getNextToken();
        return 1;
    }
    return 0;
}//LOG_TERM()

int LOG_OPERATOR(){
    if(setCode("log_and")||setCode("log_or")){
        return 1;
    }return 0;
}//LOG_OPERATOR()

int REL_EXPR(){
    advanced();

    if(isAdMatch("log_and")||isAdMatch("log_or")||isAdMatch("term_sym")){
        return 0;
    }

    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"rel_not")||!strcmp(chkexp,"rel_great")||!strcmp(chkexp,"rel_gqual")||
           !strcmp(chkexp,"rel_less")||!strcmp(chkexp,"rel_lqual")||!strcmp(chkexp,"rel_equal")){
                y = 1; break;
        }
        if(!strcmp(chkexp,"term_sym")||!strcmp(chkexp,"oCurl")||!strcmp(chkexp,"cCurl")) break;
    }
    if(y==0) return 0;
    else y = 0;

    if(setCode("oParen")){
        templine = fileline;
        advanced();
        if(adMatch("oParen")){
            getNextToken();
            relParen = 1;
        }
        else if(!ARITH_EXPR()){
            fileline = templine;
            getNextToken();
            relParen = 1;
        }
        else{
            fileline = templine - 1;
            getNextToken();
        }
    }

    if(REL_TERM()); else error();

    if(REL_OPERATOR()){
        getNextToken();
        if(REL_TERM()){
            if(setCode("cParen")&&relParen){
                getNextToken();
                relParen = 0;
            }
            else if(setCode("cParen")){
                getNextToken();
                if(!relParen){
                    strcpy(msg,"Unexpected: ");
                    strcpy(code,"')'");
                    relParen = 0;
                    error();
                }
                else if(relParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    relParen = 0;
                    error();
                }
            }
            else{
                if(relParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    relParen = 0;
                    error();
                }
            }
            return 1;
        }else return 0;
    }

    if(setCode("cParen")&&relParen){
        getNextToken();
        relParen = 0;
    }
    else if(setCode("cParen")){
        getNextToken();
        if(!relParen){
            strcpy(msg,"Unexpected: ");
            strcpy(code,"')'");
            relParen = 0;
            error();
        }
        else if(relParen){
            strcpy(msg,"Expected: ");
            strcpy(code,"')'");
            relParen = 0;
            error();
        }
    }
    else{
        if(relParen){
            strcpy(msg,"Expected: ");
            strcpy(code,"')'");
            relParen = 0;
            error();
        }
    }
    return 0;
}

int REL_OPERATOR(){
    if(setCode("rel_not")||setCode("rel_great")||setCode("rel_gqual")||
       setCode("rel_less")||setCode("rel_lqual")||setCode("rel_equal")){
        return 1;
    }return 0;
}//REL_TERM()

int REL_TERM(){
    if(ARITH_EXPR()){
        return 1;
    }
    else if(setCode("ident")||setCode("val_int")||setCode("val_float")){
        getNextToken();
        return 1;
    }
    return 0;
}//REL_TERM

int ARITH_EXPR(){
    if(setCode("oParen")){
        getNextToken();
        arithParen = 1;
    }
    advanced();

    if(!(isAdMatch("math_add")||isAdMatch("math_sub")||isAdMatch("math_mul")||isAdMatch("math_div")||isAdMatch("math_mod"))){
         return 0;
    }


    if(setCode("incre")||setCode("decre")){
        getNextToken();
        if(setCode("ident")){
            getNextToken();
            if(!isMatch("term_sym")) ARITH_EXPR();
                else return 1;
        }
    }
    else if(setCode("ident")){
        getNextToken();
        if(setCode("incre")||setCode("decre")){
            getNextToken();
            if(!isMatch("term_sym")) ARITH_EXPR();
                else return 1;
        }
    }
    else if(TERM());
    else error();

    if(ARITH_OPERATOR()){
        getNextToken();
        if(TERM()){
            if(setCode("cParen")&&arithParen){
                getNextToken();
                arithParen = 0;
            }
            else if(setCode("cParen")){
                getNextToken();
                if(!arithParen){
                    strcpy(msg,"Unexpected: ");
                    strcpy(code,"')'");
                    arithParen = 0;
                    error();
                }
                else if(arithParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    arithParen = 0;
                    error();
                }
            }
            else{
                if(arithParen){
                    strcpy(msg,"Expected: ");
                    strcpy(code,"')'");
                    arithParen = 0;
                    error();
                }
            }
            return 1;
        }else return 0;
    }

    if(setCode("cParen")&&arithParen){
        getNextToken();
        arithParen = 0;
    }
    else if(setCode("cParen")){
        getNextToken();
        if(!arithParen){
            strcpy(msg,"Unexpected: ");
            strcpy(code,"')'");
            arithParen = 0;
            error();
        }
        else if(arithParen){
            strcpy(msg,"Expected: ");
            strcpy(code,"')'");
            arithParen = 0;
            error();
        }
    }
    else{
        if(arithParen){
            strcpy(msg,"Expected: ");
            strcpy(code,"')'");
            arithParen = 0;
            error();
        }
    }
    return 0;
}

int ARITH_OPERATOR(){
    if(setCode("math_add")||setCode("math_sub")||setCode("math_mul")||setCode("math_div")||setCode("math_mod")){
        return 1;
    }return 0;
}//ARITH_OPERATOR()

int TERM(){
    if(setCode("val_int")||setCode("val_float")||setCode("ident")){
        getNextToken();
        return 1;
    }return 0;
}

int SYMBOLS(){
    if(isMatch("term_sym")||isMatch("comma")||isMatch("oParen")||isMatch("cParen")||isMatch("oCurl")||isMatch("assign_op")
       ||LOG_OPERATOR()||REL_OPERATOR()||ARITH_OPERATOR()){
        strcpy(msg,"Unexpected: ");
        char temp[30];
        strcpy(temp,""); strcat(temp,"Symbol -- "); strcat(temp,token);
        strcpy(code,temp);
        getNextToken();
        return 1;
    }return 0;
}

/*void insertNode(char tempData[], int i){
    currNode = (struct parseTree*) malloc(sizeof(struct parseTree));
    currNode -> parent = NULL;
    for(k = 0; k < 10; k++) currNode->child[k] = NULL;
    for(traverse = root; traverse!=NULL;){
        if(!strcmp(traverse->child[k]->data, tempNode)){
            temp = traverse;
            break;
       }
        for(k = 0; k < 10; k++){
            if(traverse->child[k]!=NULL{
               if(!strcmp(traverse->child[k]->data, tempNode)){
                    temp = traverse;
                    break;
               }
            }
        }
    }

    if (root == NULL){
        root = currNode = temp;
        strcpy(currNode, tempData);
    }//if
    else{
        temp->child[i] = currNode;
        currNode->parent = temp;
        temp = currNode;
        strcpy(currNode, tempData);
    }//else
}

void backTrack(){
    traverse = traver->parent!=NULL?traverse->parent, traverse;
}*/









