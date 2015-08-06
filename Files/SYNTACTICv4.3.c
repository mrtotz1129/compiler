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

FILE *BFSParseTree, *SymTable;

typedef struct tree{
    char name[50];
    int childCount;
    int level, lineNum;
    char value[50], type[20];
    struct tree *children[100], *parent, *sibling;
}Tree;

typedef struct queue{
    struct tree *node;
    struct queue *next, *prev;
};

struct queue *back, *curr, *front;
struct tree *current, *root, *traverse, *tempCurr, *tempCurr1;

void enqueue(struct tree *temp);
void dequeue();
void insertNode(struct tree *tempParent, char* data, int level, int line, int swap);
void displayQueue();
int queueSize();
void BFSearch();
Tree* newNode(char* data, int level, int line);

int fileline = 0, state, line, count = 0, errorCount = 0, adLine = 0, chk = 0, adln, adst, x, y;
int isLog = 0, isRel = 0, isArith = 0, templine = 0, loop = 0, lvl = 0, tmplvl = 0;
char  lexeme[50], token[30], code[30], stmt[30], msg[30], errorMessage[100];
char  adLex[50], adTok[30], chkexp[30], PTmessage[80];
int isAcceptedExpr = 0;

int setCode(char temp[]);
int isMatch(char temp[]);
int isAdMatch(char temp[]);
Tree* SYNTACTIC();
void error();
void SNS();

void SMT();
void DEC_SMT();
void INPUT_SMT();
void OUTPUT_SMT();
void M_SMT();
void LOOP_SMT();
void WHILE_SMT();
void DO_SMT();
void FOR_SMT();
void COND_SMT();
void SWITCH_SMT();
void OPTION();
int DATA_TYPE();
int TYPE_SPEC();
int CONST();
int EXPR();
void STEPSIZE();

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

Tree* SYNTACTIC(){
    errorCount = 0;
    remove("ParseTree.sns");

    BFSParseTree = fopen("ParseTree.sns","aw+");

    current = root = traverse = NULL;
    front = back = curr = tempCurr = tempCurr1 = NULL;

    fileline = count = errorCount = adLine = lvl = tmplvl = y = 0;

    countLines();
    getNextToken();

    SNS();

    fclose(BFSParseTree);
    return root;
    //return errorCount;
}//main()

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
    else if(!strcmp(temp, "dt_bool")||
            !strcmp(temp, "dt_char")||
            !strcmp(temp, "dt_float")||
            !strcmp(temp, "dt_int")||
            !strcmp(temp, "dt_string")){
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
        return 1;
    } return 0;
}//isMatch()

void SNS(){
    insertNode(root, "SNS", 0, 0, 0);
    current = tempCurr = tempCurr1 = root;
    if(setCode("meth_main")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    } else error();

    if(setCode("start")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();

    insertNode(current, "SMT", current->level + 1, 0, 1);
    tempCurr = current;

    SMT();

    if(setCode("finish")){
        insertNode(root, token, root->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        BFSearch();

    }else error();
}//SNS()

void SMT(){
    strcpy(stmt,"");
    if(DATA_TYPE()){
        current = tempCurr;
        DEC_SMT();
    }
    else if(isMatch("st_read")){
        current = tempCurr;
        INPUT_SMT();
    }
    else if(isMatch("st_print")){
        current = tempCurr;
        OUTPUT_SMT();
    }
    else if(isMatch("ident")
            ||isMatch("inval_ident")
            ||isMatch("incre")
            ||isMatch("decre")){
        current = tempCurr;
        ASSIGN_SMT();
    }
    else if(isMatch("lp_while")
            ||isMatch("lp_do")
            ||isMatch("lp_for")){
        current = tempCurr;
        LOOP_SMT();
    }
    else if(isMatch("cond_if")){
        current = tempCurr;
        COND_SMT();
    }
    else if(isMatch("cond_else")){
            current = tempCurr;
            insertNode(current, "COND_SMT", current->level + 1, 0, 1);
            tempCurr = current;
            errorCount++;
            sprintf(errorMessage, "<line %d> -- misplaced cond_else\n", line);
            Message(errorMessage);

            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
            if(isMatch("cond_if")) COND_SMT();
            else if(setCode("oCurl")){
                    insertNode(current, token, current->level + 1, line, 1);
                    insertNode(current, lexeme, current->level + 1, line, 0);
                    current = current->parent;
                    getNextToken();


                    insertNode(current, "SMT", current->level + 1, 0, 1);
                    tempCurr = current;
                    SMT();
                    tempCurr = tempCurr->parent;
                    current = tempCurr;

                    if(setCode("cCurl")){
                    insertNode(current, token, current->level + 1, line, 1);
                    insertNode(current, lexeme, current->level + 1, line, 0);
                    current = current->parent;

                    getNextToken();
                }else error();
            }else error();

    }
    else if(isMatch("cond_swt")){
        current = tempCurr;
        SWITCH_SMT();
    }
    else if(SYMBOLS()) error();
    else return;

    if(!isMatch("finish")){
        SMT();
    }
}//SMT()

void DEC_SMT(){
    insertNode(current, "DEC_SMT", current->level + 1, 0, 1);
    tempCurr = current;
    strcpy(stmt,"declaration");
    if(DATA_TYPE()){
        insertNode(current, "DATA_TYPE", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent->parent;

        getNextToken();
    }

    if(isMatch("inval_ident")
       ||setCode("ident")){
        if(setCode("inval_ident")) error();
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();

        if(setCode("op_assign")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

            if(EXPR());
            else error();
        }
    }else error();

    current = tempCurr;

    if(setCode("comma")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
        DEC_SMT();
    }
    else if(setCode("term_sym")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else{
        tempCurr = current;
        error();
    }

    current = tempCurr->parent;
    tempCurr = current;
}//DEC_SMT()

void INPUT_SMT(){
    insertNode(current, "INPUT_SMT", current->level + 1, 0, 1);
    tempCurr = current;
    strcpy(stmt,"input");
    if(isMatch("st_read")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }else error();
    if(setCode("ident")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();

    if(setCode("comma")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();
    if(TYPE_SPEC()){
        if(isMatch("type_int")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else if(isMatch("type_float")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else if(isMatch("type_char")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else if(isMatch("type_str")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else if(isMatch("type_bool")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else error();
        getNextToken();
    }else error();

    current = tempCurr;
    if(setCode("term_sym")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();
     tempCurr = tempCurr->parent;
    //templvl = 1;
}//INPUT_SMT()

void OUTPUT_SMT(){
    insertNode(current, "OUTPUT_SMT",current->level + 1, 0, 1);
    tempCurr = current;

    strcpy(stmt,"output");
    if(isMatch("st_print")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }

    if(setCode("ident")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }
    else if(setCode("val_str")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }
    else error();

    if(setCode("ccat_out")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
        OUTPUT_SMT();
    }
    else if(setCode("term_sym")){
        current = tempCurr;
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else{
        error();
    }

    tempCurr = tempCurr->parent;
    current = tempCurr;
}//OUTPUT_SMT()

void ASSIGN_SMT(){
    insertNode(current, "ASSIGN_SMT", current->level + 1, 0, 1);
    tempCurr = current;

    strcpy(stmt, "assign");

    if(UNARY_OP()){
        if(setCode("ident")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            getNextToken();
            current = current->parent;
        }
        else error();
    }
    else if(setCode("ident")||isMatch("inval_ident")){
        if(setCode("inval_ident")) error();
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();

        if(UNARY_OP());
        else if(setCode("op_assign")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();

            if(EXPR());
            else error();

            current = tempCurr;
        }
    }

    if(setCode("term_sym")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else{
        current = tempCurr;
        error();
    }

    tempCurr = tempCurr->parent;
    current = tempCurr;
}//ASSIGN_SMT()

void STEPSIZE(){
    insertNode(current, "STEPSIZE", current->level + 1, 0, 1);
    tempCurr = current;

    strcpy(stmt, "assign");

    if(UNARY_OP()){
        if(setCode("ident")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            getNextToken();
            current = current->parent;
        }
        else error();
    }
    else if(setCode("ident")||isMatch("inval_ident")){
        if(setCode("inval_ident")) error();
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();

        if(UNARY_OP());
        else if(setCode("op_assign")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();

            if(EXPR());
            else error();

            current = tempCurr;
        }
    }

    tempCurr = tempCurr->parent;
    current = tempCurr;

}//STEPSIZE()

void LOOP_SMT(){
    if(isMatch("lp_while")){
        insertNode(current, "LOOP_SMT", current->level + 1, 0, 1);
        tempCurr = current;
        WHILE_SMT();
    }
    else if(isMatch("lp_do")){
        insertNode(current, "LOOP_SMT", current->level + 1, 0, 1);
        tempCurr = current;
        DO_SMT();
    }
    else if(isMatch("lp_for")){
        insertNode(current, "LOOP_SMT", current->level + 1, 0, 1);
         tempCurr = current;
        FOR_SMT();
    }
    tempCurr = tempCurr->parent;
    current = tempCurr;
}

void FOR_SMT(){
    insertNode(current, "FOR_SMT", current->level + 1, 0, 1);
    tempCurr = current;

    if(isMatch("lp_for")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }

    if(setCode("oParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();

        if(setCode("term_sym")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;


            getNextToken();
        }
        else if(DATA_TYPE()){
            DEC_SMT();
        }
        else if(setCode("ident")){
            ASSIGN_SMT();
        }
        else error();


        if(EXPR());
        else error();

        current = tempCurr;

        if(setCode("term_sym")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();
        }


        if(setCode("ident")){
            STEPSIZE();
        }

        if(setCode("cParen")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
        }else error();
    }else error();

    if(setCode("oCurl")){

        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();

        insertNode(current, "SMT", current->level + 1, 0, 1);
        tempCurr = current;

        SMT();

        tempCurr = tempCurr->parent;
        current = tempCurr;

        if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
        }
    }//else error();
    tempCurr = tempCurr->parent;
    current = tempCurr;
}//FOR

void DO_SMT(){
    insertNode(current, "DO_SMT", current->level + 1, 0, 1);
    tempCurr = current;
    if(isMatch("lp_do")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();
    if(setCode("oCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();

        insertNode(current, "SMT", current->level + 1, 0, 1);
        tempCurr = current;

        SMT();

        tempCurr = tempCurr->parent;
        current = tempCurr;

    if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
        }
    }else error();
    if(isMatch("lp_while")){
            loop = 1;
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
    }else error();
    if(setCode("oParen")){

            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
    }else error();
    if(EXPR());
   current = tempCurr;
    if(setCode("cParen")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

    }else error();

    if(setCode("term_sym")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
    }
    else error();
    tempCurr = tempCurr->parent;
    current = tempCurr;
}

void WHILE_SMT(){
    insertNode(current, "WHILE_SMT", current->level + 1, 0, 1);
    tempCurr = current;
    if(isMatch("lp_while")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    if(setCode("oParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();

        if(EXPR());
        else error();

        current = tempCurr;

        if(setCode("cParen")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();
        }else error();
    }else error();

    if(setCode("oCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();

        insertNode(current, "SMT", current->level + 1, 0, 1);
        tempCurr = current;

        SMT();

        tempCurr = tempCurr->parent;
        current = tempCurr;

        if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
        }else error();
    }
    else error();

    tempCurr = tempCurr->parent;
    current = tempCurr;
}//WHILE

void COND_SMT(){
     insertNode(current, "COND_SMT", current->level + 1, 0, 1);
     tempCurr = current;

    if(isMatch("cond_if")){

        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }
    if(setCode("oParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }else error();

    if(EXPR());
    else error();

    if(setCode("cParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;


        getNextToken();
    }else error();

    if(setCode("cond_then")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();
    }

    if(setCode("oCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;

        getNextToken();

        insertNode(current, "SMT", current->level + 1, 0, 1);
        tempCurr = current;
        SMT();
        tempCurr = tempCurr->parent;
        current = tempCurr;


        if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();
            strcpy(msg,"Expected Symbol ");
            strcpy(code, "'{'");
        }else error();
    }
    else if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;

            getNextToken();
            strcpy(msg,"Unexpected Symbol ");
            strcpy(code, "'}'");
            error();
    }else smt();//ano to?


    if(isMatch("cond_else")){

            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

            if(isMatch("cond_if")) COND_SMT();
            else if(setCode("oCurl")){
                    insertNode(current, token, current->level + 1, line, 1);
                    insertNode(current, lexeme, current->level + 1, line, 0);
                    current = current->parent;
                    getNextToken();


                    insertNode(current, "SMT", current->level + 1, 0, 1);
                    tempCurr = current;
                    SMT();
                    tempCurr = tempCurr->parent;
                    current = tempCurr;

                    if(setCode("cCurl")){
                    insertNode(current, token, current->level + 1, line, 1);
                    insertNode(current, lexeme, current->level + 1, line, 0);
                    current = current->parent;

                    getNextToken();
                }else error();
            }else error();

    }

    tempCurr = tempCurr->parent;
}//COND

void SWITCH_SMT(){
    insertNode(current, "SWITCH_SMT", current->level + 1, 0, 1);
    tempCurr = current;
    if(setCode("cond_swt")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    if(setCode("oParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();

    if(EXPR());
    else error();

    if(setCode("cParen")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    if(setCode("oCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    if(isMatch("cond_opt")) OPTION();

    if(setCode("cCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    tempCurr = tempCurr->parent;
}//SWITCH

void OPTION(){
    insertNode(current, "OPTION", current->level + 1, 0, 1);
    tempCurr = current;
    if(setCode("cond_opt")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }

    if(EXPR());
    else error();

    if(setCode("colon")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else error();
    if(setCode("oCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else error();

    insertNode(current, "SMT", current->level + 1, 0, 1);
    tempCurr = current;
    SMT();
    tempCurr = tempCurr->parent;
    current = tempCurr;

    if(setCode("st_break")
       ||setCode("st_cont")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    if(setCode("term_sym")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }else error();
    if(setCode("cCurl")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }
    else error();

    if(isMatch("cond_opt")) OPTION();
    else{
        if(setCode("cond_def")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }
        if(setCode("colon")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }else error();
        if(setCode("oCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }else error();

            insertNode(current, "SMT", current->level + 1, 0, 1);
            tempCurr = current;
            SMT();
            tempCurr = tempCurr->parent;
            current = tempCurr;

        if(setCode("st_break")
           ||setCode("st_cont")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }
        if(setCode("term_sym")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }else error();
        if(setCode("cCurl")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }else error();
    }
    tempCurr = tempCurr->parent;
}//OPTION()

int DATA_TYPE(){

    if(isMatch("dt_bool")
       ||isMatch("dt_char")
       ||isMatch("dt_float")
       ||isMatch("dt_int")
       ||isMatch("dt_string")){
        return 1;
    }
    return 0;
}//DATA_TYPE()

int TYPE_SPEC(){
    if(isMatch("type_int")
       ||isMatch("type_float")
       ||isMatch("type_char")
       ||isMatch("type_str")
       ||isMatch("type_bool")){
        return 1;
    }
    return 0;
}//TYPE_SPEC()

int BOOL_VAL(){
    if(setCode("bl_true")
       ||setCode("bl_false")){
        return 1;
    }return 0;
}//BOOL_VAL()

int CONST(){
    if(setCode("val_int")
       ||setCode("val_float")
       ||setCode("val_str")
       ||setCode("val_char")
       ||setCode("inval_sym")
       ||setCode("inval_num")
       ||setCode("inval_str")
       ||setCode("inval_char")
       ||BOOL_VAL()){
       if(!strcmp(token,"inval_sym")
          ||!strcmp(token,"inval_num")
          ||!strcmp(token,"inval_str")
          ||!strcmp(token,"inval_char")){
          error();
        }

        insertNode(current, "CONST", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);

        current = current->parent->parent;

        getNextToken();
        return 1;
    }
    return 0;
}//CONST()

int EXPR(){
    insertNode(current, "EXPR", current->level + 1, 0, 1);

    if(LOG_EXPR()){
        isAcceptedExpr = 0;
        current = current->parent;
        return 1;
    }
    isAcceptedExpr = 0;
    current = current->parent;
    return 0;
}//EXPR()

int LOG_EXPR(){
    isLog = 0;
    insertNode(current, "LOG_EXPR", current->level + 1, 0, 1);


    if(isMatch("log_not")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
    }

    if(LOG_TERM()) isAcceptedExpr = 1;


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
        current = current->parent;
        return (isAcceptedExpr == 1)? 1: 0;
    }
    else isLog = 0;

    if(isAcceptedExpr) isAcceptedExpr = 0;

    if(LOG_OPERATOR()){
       current = current->parent;//asdasdasdasdasdasdas
        return 1;
    }
    current = current->parent;
    return 0;
}//LOG_EXPR()

int LOG_TERM(){
    insertNode(current, "LOG_TERM", current->level + 1, 0, 1);

    if(REL_EXPR()){
        current =  current->parent;
        return 1;
    }

    current =  current->parent;
    return 0;
}//LOG_TERM()

int LOG_OPERATOR(){
    if(setCode("log_and")
       ||setCode("log_or")){
        insertNode(current, "LOG_OPERATOR", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent->parent;


        getNextToken();

        if(isMatch("log_not")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();

        }
        if(LOG_TERM()){
            current = current->parent;
            if(LOG_OPERATOR());
            return 1;
        }

        current = current->parent;
        return 0;
    }
    current = current->parent;
    return 0;
}//LOG_OPERATOR()

int REL_EXPR(){
    isRel = 0;
    insertNode(current, "REL_EXPR", current->level + 1, 0, 1);

    if(REL_TERM()) isAcceptedExpr = 1;

    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"cParen"))
            break;
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
            //current = current->parent;//asfasfasfasfas

        return (isAcceptedExpr == 1)? 1: 0;
    }
    else isRel = 0;

    if(isAcceptedExpr) isAcceptedExpr = 0;

    if(REL_OPERATOR()){
        current = current->parent;//isNeeded?
        return 1;
    }
    current = current->parent;//isNeeded?
    return 0;
}

int REL_OPERATOR(){
    if(setCode("rel_not")
       ||setCode("rel_great")
       ||setCode("rel_gqual")
       ||setCode("rel_less")
       ||setCode("rel_lqual")
       ||setCode("rel_equal")){
        insertNode(current, "REL_OPERATOR", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent->parent;
        getNextToken();

        if(REL_TERM()){
            current = current->parent;
            if(REL_OPERATOR());
            return 1;
        }
        current = current->parent;
        return 0;
    }
    //current = current->parent;
    return 0;
}//REL_OPERATOR()

int REL_TERM(){
    insertNode(current, "REL_TERM", current->level + 1, 0, 1);

    if(ARITH_EXPR()){
        current = current->parent;
        return 1;
    }
    current = current->parent;//isNeeded?
    return 0;
}//REL_TERM

int ARITH_EXPR(){
    isArith = 0;
    insertNode(current, "ARITH_EXPR", current->level + 1, 0, 1);

    if(TERM()) isAcceptedExpr = 1;

    chk = fileline-1;
    for(;;){
        chk++; chkEXP();
        if(!strcmp(chkexp,"cParen"))
            break;
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
        current = current->parent;
        return 1;
    }

    current = current->parent;//eto pala
    return 0;
}//ARITH_OPERATOR()

int TERM(){
    insertNode(current, "TERM", current->level + 1, 0, 1);

    if(FACTOR()){
        if(OPERATOR1()){
            current = current->parent;
            return 1;
        }
        current = current->parent;
        return 1;
    }

    current = current->parent;
    return 0;
}//TERM()

int OPERATOR(){
    if(setCode("math_add")
       ||setCode("math_sub")){
       insertNode(current, "OPERATOR", current->level + 1, 0, 1);
       insertNode(current, token, current->level + 1, line, 1);
       insertNode(current, lexeme, current->level + 1, line, 0);
       current = current->parent->parent;
       getNextToken();

       if(TERM()){
            if(!OPERATOR()) OPERATOR1();
            return 1;
       }
       return 0;
    }

    return 0;
}

int OPERATOR1(){
    if(setCode("math_mul")
       ||setCode("math_div")
       ||setCode("math_mod")){
        insertNode(current, "OPERATOR1", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent->parent;
        getNextToken();

        if(FACTOR()){
            current = current->parent;
            if(!OPERATOR1()) OPERATOR();
            return 1;
        }

        current = current->parent;
        return 0;
   }

   return 0;
}

int FACTOR(){
    insertNode(current, "FACTOR", current->level + 1, 0, 1);

    if(CONST()){
        current = current->parent;
        return 1;
    }
    else if(UNARY_OP()){
        if(setCode("ident")){
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
        }
        else error();
        current = current->parent;
        return 1;
    }
    else if(setCode("ident")){
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();
        if(UNARY_OP());
        current = current->parent;
        return 1;
    }
    else if(setCode("oParen")){
        y = 1;
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent;
        getNextToken();

        if(EXPR());

        if(setCode("cParen")){
            y = 0;
            insertNode(current, token, current->level + 1, line, 1);
            insertNode(current, lexeme, current->level + 1, line, 0);
            current = current->parent;
            getNextToken();
        }
        else{
            y = 0;
            error();
        }
        current = current->parent;
        return 1;
    }
    current = current->parent;
    return 0;
}
//mamaya na

int UNARY_OP(){
    if(setCode("incre")
       ||setCode("decre")){
        insertNode(current, "UNARY_OP", current->level + 1, 0, 1);
        insertNode(current, token, current->level + 1, line, 1);
        insertNode(current, lexeme, current->level + 1, line, 0);
        current = current->parent->parent;
        getNextToken();
        return 1;
   }
    return 0;
}

int SYMBOLS(){
    if(isMatch("term_sym")
       ||isMatch("comma")
       ||isMatch("oParen")
       ||isMatch("cParen")
       ||isMatch("oCurl")
       ||isMatch("assign_op")
       ||LOG_OPERATOR()
       ||REL_OPERATOR()
       ||OPERATOR()
       ||OPERATOR1()){
        strcpy(msg,"Unexpected: ");
        char temp[30];
        strcpy(temp,""); strcat(temp,"Symbol -- "); strcat(temp,token);
        strcpy(code,temp);
        getNextToken();
        return 1;
    }return 0;
}

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

int isAdMatch(char temp[]){
    if(!strcmp(adTok, temp)){
        //printf("%s\t%s\n",adTok,temp);
        return 1;
    }return 0;
}//isAdMatch()


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




void BFSearch(){
    int i;
    traverse = root;
    enqueue(traverse);
    while(queueSize()){
        for(i = 0; i < 100; i++){
            if(traverse->children[i] !=NULL ){
                enqueue(traverse->children[i]);
            }
        }

        sprintf(PTmessage, "Line - %d * ( Level: %d ) -- < %s > Parent: %s\n", traverse->lineNum, traverse->level, traverse->name, traverse->parent->name);
        fprintf(BFSParseTree, PTmessage);

        dequeue();
        if(queueSize() != 0) traverse = front->node;
    }
}

void enqueue(struct tree *temp1){
    curr = (struct queue*)malloc(sizeof(struct queue));
    curr->next = curr->prev = NULL;
    if (front == NULL){
        back = front = curr;
        curr->node = temp1;
    }
    else{
        back->next = curr;
        curr->prev = back;
        back = curr;
        curr->node = temp1;
    }
    back->next = NULL;
}

void dequeue(){
    if(front == NULL) return;
    else{
        front = front->next != NULL? front->next: NULL;
        if(front != NULL) front->prev = NULL;
    }
}

/*
void displayQueue(){
    struct queue *temp;
    for(temp = front; temp!=NULL; temp = temp->next){
        printf("%s\n", temp->node->data);
    }
}*/

int queueSize(){
    int size = 0;
    struct queue *temp;
    for(temp = front; temp!=NULL; temp = temp->next){
        //printf("%s\n", temp->node->data);
        size++;
    }
    return size;
}

void insertNode(struct tree *tempParent, char* data, int level, int line, int swap){
    Tree *node1;
    node1 = newNode(data, level, line);

    if(tempParent!=NULL){
        node1->parent = tempParent;
        tempParent->children[tempParent->childCount] = node1;
        if(tempParent->childCount != 0) tempParent->children[tempParent->childCount - 1]->sibling = node1;
        tempParent->childCount++;
    }
    else root = node1;

    if(swap == 1){
        current = node1;
    }
}


Tree* newNode(char* data, int level, int line) {
	Tree *node;
	int i;
    node = (struct tree*) malloc(sizeof(struct tree));
    for(i = 0; i < 100; i++){
        node->children[i] = NULL;
    }
    node->childCount = 0;
    node->level = level;
    node->lineNum = line;
    node->parent = NULL;
    node->sibling = NULL;
    strcpy(node->name, data);
    strcpy(node->value, "");
    strcpy(node->type, "");

	return node;
}









