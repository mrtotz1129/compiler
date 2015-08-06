#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include "SNSINTERPRETER.h"

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


typedef struct tree{
    char name[50];
    int childCount;
    int level, lineNum;
    char value[50], type[20];
    struct tree
     *children[100], *parent, *sibling;
}APTree;

struct identifiers{
    int isUsed;
    struct tree *nodeIdent;
    struct identifiers *next, *prev;
}*head, *currentIdent, *currentIdentAssign, *temp;

struct stack{
    struct tree *node;
    struct stack *next, *prev;
}*top, *curr;

struct mainStack{
    struct tree *node;
    struct stack *next, *prev;
}*top2, *curr2, *temp2, *symbol, *symbol2;

struct tree *root, *aTraverse;
struct mainStack *tempSym, *tempSym2, *tempSym3, *tempSym4, *tempSym5, *whileSym, *forSym, *doSym, *loopTemp, *loopTemp2, *condTemp, *switchTemp;

void iPush(struct tree *temp);
void iPop();
void iPush2(struct tree *temp);
void displayDFS();
int isEmpty();
void DFS();
void next();
void display();

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
void iSTEPSIZE();

int Continue = 0, isThereExpr = 0, executeCondition = 0, executeElse = 0, conditionExecuted = 0, executeDefault = 1, iOneSmt = 0;
char *switchValue;

void SNSINTERPRETER(struct tree *tempRoot, struct identifiers *tempHead){
    head = tempHead;
    iOneSmt = 0;
    top = curr = NULL;
    top2 = curr2 = temp2 = symbol = symbol2 = currentIdent = currentIdentAssign = temp = condTemp = switchTemp = NULL;
    tempSym = tempSym2 = tempSym3 = tempSym4 = tempSym5 = whileSym = doSym = loopTemp = loopTemp2 = forSym = NULL;
    root = tempRoot;
    DFS();

    iSNS();
    display();
    //displayDFS();
}


void iSNS(){
    if(match("SNS")) next();
    if(match("meth_main")) next();
    if(match("main")) next();
    if(match("start")){
        next();
        next();
    }

    iSMT();

    if(symbol!=NULL)
        if(match("finish")){
            next();
        }
}

void iSMT(){
    if(match("SMT")) next();
    if(iOneSmt){
        iOneSmt = 0;
        if(match("DEC_SMT")) iDEC_SMT();
        else if(match("ASSIGN_SMT")) iASSIGN_SMT();
        else if(match("OUTPUT_SMT")) iOUTPUT_SMT();
        else if(match("INPUT_SMT")) iINPUT_SMT();
        else if(match("LOOP_SMT")) iLOOP_SMT();
        else if(match("COND_SMT")) iCOND_SMT();
        else if(match("SWITCH_SMT")) iSWITCH_SMT();
        else if(match("st_cont")||match("st_break")){
            return;
        }
        else return;
        return;
    }
    else{
        if(match("DEC_SMT")) iDEC_SMT();
        else if(match("ASSIGN_SMT")) iASSIGN_SMT();
        else if(match("OUTPUT_SMT")) iOUTPUT_SMT();
        else if(match("INPUT_SMT")) iINPUT_SMT();
        else if(match("LOOP_SMT")) iLOOP_SMT();
        else if(match("COND_SMT")) iCOND_SMT();
        else if(match("SWITCH_SMT")) iSWITCH_SMT();
        else if(match("st_cont")||match("st_break")){
            return;
        }
        else return;
    }

    if(!strcmp("xFINISH",symbol->node->name)) return;
        else iSMT();

}
void iSWITCH_SMT(){
    char* val = malloc(100);
    switchValue = malloc(100);

    if(match("SWITCH_SMT")) next();
    if(match("cond_swt")){
        next();
        next();
    }
    if(match("oParen")){
        next();
        next();
    }

    strcpy(val, iEXPR());
    sprintf(switchValue, "%s", val);

    if(match("cParen")){
        next();
        next();
    }
    if(match("oCurl")){
        next();
        next();
    }

    if(match("OPTION")) iOPTION();

    if(match("cCurl")){
        next();
        next();
    }

    executeDefault = 1;
}//SWITCH

void iOPTION(){
    char* val = malloc(100);

    if(match("OPTION")) next();
    if(match("cond_opt")){
        next();
        next();
    }

    strcpy(val, iEXPR());
    char *tempStr = malloc(70);

    if(val[0] == '\'' || val[0] == '"'){
        sprintf(tempStr, "%s", val);
        tempStr++[strlen(tempStr)-1] = 0;
        sprintf(val, "%s", tempStr);
        sprintf(val, "%s", tempStr);
    }

    if(strcmp(switchValue, val)){
        for(switchTemp = symbol; switchTemp != NULL; switchTemp = switchTemp->next){
            if(!strcmp(switchTemp->node->name, "OPTION")
               ||!strcmp(switchTemp->node->name, "cond_def")){
                symbol = switchTemp;
                goto gotoNextOption;
            }
        }
    }
    if(match("colon")){
        next();
        next();
    }

    iSMT();

    executeDefault = 0;

    if(match("st_break")
       ||match("st_cont")){
        if(match("st_break")){
            for(switchTemp = symbol; switchTemp != NULL; switchTemp = switchTemp->prev){
                if(switchTemp->node == symbol->node->parent->parent){
                    symbol = switchTemp;
                    break;
                }
            }
            for(switchTemp = symbol; switchTemp != NULL ; switchTemp = switchTemp->next){
                if(switchTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = switchTemp;
                    break;
                }
            }
            return;
        }
        next();
        next();
    }

    gotoNextOption:

    if(match("OPTION")) iOPTION();
    else{
        if(match("cond_def")){
            next();
            next();
        }
        if(match("colon")){
            next();
            next();
        }

        iSMT();

        if(match("st_break")
           ||match("st_cont")){
            next();
            next();
        }
        if(match("term_sym")){
            next();
            next();
        }
    }
}

void iASSIGN_SMT(){
    char *finalValue = malloc(70);
    float num;
    int num2;

    if(match("ASSIGN_SMT")) next();
    if(match("UNARY_OP")){
        next();
        if(match("incre")){
            next();
            next();
            if(match("ident")){
                next();
                getCurrentID(symbol->node->name);
                num = (float)atof(currentIdent->nodeIdent->value);
                num++;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }
                else sprintf(finalValue, "%f", num);
                sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
                sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
                next();
            }
        }
        else if(match("decre")){
            next();
            next();
            if(match("ident")){
                next();
                getCurrentID(symbol->node->name);
                num = (float)atof(currentIdent->nodeIdent->value);
                num--;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    num2;
                    sprintf(finalValue, "%d", num2);
                }
                else sprintf(finalValue, "%f", num);
                sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
                sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
                next();
            }
        }
    }//unary_operator
    else if(match("ident")){
        next();
        getCurrentID(symbol->node->name);
        currentIdentAssign = currentIdent;
        next();
        if(unary()){
            if(match("++")){
                next();
                num = (float)atof(currentIdent->nodeIdent->value);
                num++;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }else sprintf(finalValue, "%f", num);
            }
            else if(match("--")){
                next();
                num = (float)atof(currentIdent->nodeIdent->value);
                num--;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }else sprintf(finalValue, "%f", num);
            }
            sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
            sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
        }
        else if(match("op_assign")){
            next();
            if(match("=")) next();

            strcpy(finalValue, iEXPR());

            char *tempStr = malloc(70);

            if(!strcmp(currentIdentAssign->nodeIdent->type, "int")){
                sprintf(currentIdentAssign->nodeIdent->value, "%d", atoi(finalValue));
                sprintf(currentIdentAssign->nodeIdent->parent->value, "%d", atoi(finalValue));
            }
            else if(!strcmp(currentIdentAssign->nodeIdent->type, "string") || !strcmp(currentIdentAssign->nodeIdent->type, "char")){
                sprintf(tempStr, "%s", finalValue);
                tempStr++[strlen(tempStr)-1] = 0;

                sprintf(currentIdentAssign->nodeIdent->value, "%s", tempStr);
                sprintf(currentIdentAssign->nodeIdent->parent->value, "%s", tempStr);
            }
            else{
                sprintf(currentIdentAssign->nodeIdent->value, "%s", finalValue);
                sprintf(currentIdentAssign->nodeIdent->parent->value, "%s", finalValue);
            }

            /*symbol2 = currentNodeIdent;
            set(finalValue2);
            getPrev();
            set(finalValue2);
            symbol2 = NULL;*/
            //printf("%s\n",tempID->nodeIdent->value);
        }
    }

    if(match("term_sym")){
        next();
        if(match(";")) next();
    }
}

void iSTEPSIZE(){
    char *finalValue = malloc(70);
    float num;
    int num2;

    if(match("STEPSIZE")) next();
    if(match("UNARY_OP")){
        next();
        if(match("incre")){
            next();
            next();
            if(match("ident")){
                next();
                getCurrentID(symbol->node->name);
                num = (float)atof(currentIdent->nodeIdent->value);
                num++;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }
                else sprintf(finalValue, "%f", num);
                sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
                sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
                next();
            }
        }
        else if(match("decre")){
            next();
            next();
            if(match("ident")){
                next();
                getCurrentID(symbol->node->name);
                num = (float)atof(currentIdent->nodeIdent->value);
                num--;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    num2;
                    sprintf(finalValue, "%d", num2);
                }
                else sprintf(finalValue, "%f", num);
                sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
                sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
                next();
            }
        }
    }//unary_operator
    else if(match("ident")){
        next();
        getCurrentID(symbol->node->name);
        currentIdentAssign = currentIdent;
        next();
        if(unary()){
            if(match("++")){
                next();
                num = (float)atof(currentIdent->nodeIdent->value);
                num++;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }else sprintf(finalValue, "%f", num);
            }
            else if(match("--")){
                next();
                num = (float)atof(currentIdent->nodeIdent->value);
                num--;
                if(fmod(num, 1) == 0){
                    num2 = (int)(num);
                    sprintf(finalValue, "%d", num2);
                }else sprintf(finalValue, "%f", num);
            }
            sprintf(currentIdent->nodeIdent->value,"%s",finalValue);
            sprintf(currentIdent->nodeIdent->parent->value,"%s",finalValue);
        }
    }
    else if(match("op_assign")){
        next();
        if(match("=")) next();

        strcpy(finalValue, iEXPR());

        sprintf(currentIdentAssign->nodeIdent->value, "%s", finalValue);
        sprintf(currentIdentAssign->nodeIdent->parent->value, "%s", finalValue);
        /*symbol2 = currentNodeIdent;
        set(finalValue2);
        getPrev();
        set(finalValue2);
        symbol2 = NULL;*/
        //printf("%s\n",tempID->nodeIdent->value);
    }
}

void iLOOP_SMT(){
    if(match("LOOP_SMT")) next();
    if(match("WHILE_SMT")){
        iWHILE_SMT();
    }
    else if(match("FOR_SMT")){
        iFOR_SMT();
    }
    else if(match("DO_SMT")){
        doSym = symbol->next;
        iDO_SMT();
    }
}

void iCOND_SMT(){
    char* val = malloc(100);
    int i, check = 0;

    if(match("COND_SMT")) next();
    if(match("cond_if")){
        next();
        next();
    }
    if(match("oParen")){
        next();
        next();
    }

    strcpy(val, expr());
    executeCondition = isTrueCondition(val);

    if(!executeCondition){
        executeElse = 1;

       setSymbolToParent();

       for(i = 0; i < symbol->node->childCount; i ++){
            if(!strcmp(symbol->node->children[i]->name, "cCurl")||!strcmp(symbol->node->children[i]->name, "cond_else")){
                for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                    if(loopTemp->node == symbol->node->children[i]){
                        symbol = loopTemp;
                        check = 1;
                        break;
                    }
                }
                break;
            }
        }
        if(!check){
            check = 0;
            for(condTemp = symbol; condTemp != NULL ; condTemp = condTemp->next){
                if(condTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = condTemp;
                    break;
                }
            }
        }
        if(!strcmp(symbol->node->name, "SMT")){
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    break;
                }
            }

            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    break;
                }
            }
            next();
            next();
        }
        else if(!strcmp(symbol->node->name, "cond_else")){
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[i]){
                    symbol = loopTemp;
                    break;
                }
            }
        }

        goto exitIfSmt;
    }

    if(match("cParen")){
        next();
        next();
    }

    if(match("cond_then")){
        next();
        next();
    }

     if(match("oCurl")){
        next();
        next();

        iSMT();
    }
    else{
        iOneSmt = 1;
        iSMT();

    }



    if(strcmp(symbol->node->name, "cCurl")){
        symbol = symbol->prev->prev;
        while(strcmp(symbol->node->name, "COND_SMT"))
            setSymbolToParent();
    }
    else{
        if(symbol->prev->prev->node->parent != symbol->node->parent){
            symbol = symbol->prev->prev;
            while(strcmp(symbol->node->name, "COND_SMT"))
                setSymbolToParent();
        }
        else setSymbolToParent();
    }

    digDeeper:

    for(condTemp = symbol; condTemp != NULL ; condTemp = condTemp->next){
        if(condTemp->node == symbol->node->children[symbol->node->childCount-1]){
            symbol = condTemp;
            break;
        }
    }

    if(!strcmp(symbol->node->name, "SMT")){
        digOutput:
        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }

        if(match("OUTPUT_SMT")) goto digOutput;
    }

    if(match("COND_SMT")) goto digDeeper;

    next();
    next();

    executeCondition = executeElse = 0;

    return;

    exitIfSmt:

    if(match("cCurl")){
        next();
        next();
    }

    if(match("cond_else")){
        next();
        next();

        if(!match("COND_SMT")){
            if(!executeElse){
                for(condTemp = symbol; condTemp != NULL; condTemp = condTemp->prev){
                    if(condTemp->node == symbol->node->parent){
                        symbol = condTemp;
                        break;
                    }
                }

                for(condTemp = symbol; condTemp != NULL ; condTemp = condTemp->next){
                    if(condTemp->node == symbol->node->children[symbol->node->childCount-1]){
                        symbol = condTemp;
                        next();
                        next();
                        break;
                    }
                }
                return;
            }
        }

        if(match("COND_SMT")){
            iCOND_SMT();
        }
        else  if(match("oCurl")){
            next();
            next();
            executeElse = 0;

            iSMT();
        }
        else{
            executeElse = 0;
            iOneSmt = 1;

            iSMT();

        }
    }
}

void iFOR_SMT(){
    char* val = malloc(100);
    int Continue = 0, i;

    if(match("FOR_SMT")) next();
    if(match("lp_for")){
        next();
        next();
    }

    if(match("oParen")){
        next();
        next();
    }

    if(match("term_sym")){
        next();
        next();
    }
    else if(match("DEC_SMT")) iDEC_SMT();
    else if(match("ASSIGN_SMT")) iASSIGN_SMT();

    if(match("EXPR")){
        isThereExpr = 1;
        continueEvaluationForLoop:

        strcpy(val, iEXPR());
        Continue = isTrueCondition(val);
    }
    else{
        isThereExpr = 0;
        Continue = 1;
    }

    if(!Continue){
        setSymbolToParent();

        if(!strcmp(symbol->node->children[symbol->node->childCount-1]->name, "cCurl")){
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    break;
                }
            }
        }
        else{
           for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(!strcmp(loopTemp->node->name, "SMT")){
                    symbol = loopTemp;
                    next();
                    break;
                }
            }
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    next();
                    next();
                    break;
                }
            }
        }

        goto closeForLoop;
    }
    else if(Continue){
        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(!strcmp(loopTemp->node->name, "cParen")){
                symbol = loopTemp;
                break;
            }
        }

        goto continueExecuteForLoop;
    }

    continueStepSize:

    if(strcmp(symbol->node->name, "cCurl")){
        symbol = symbol->prev->prev;
        while(strcmp(symbol->node->name, "FOR_SMT"))
            setSymbolToParent();
    }
    else{
        if(symbol->prev->prev->node->parent != symbol->node->parent){
            symbol = symbol->prev->prev;
            while(strcmp(symbol->node->name, "FOR_SMT"))
                setSymbolToParent();
        }
        else setSymbolToParent();
    }

    for(i = 0; i < symbol->node->childCount; i ++){
        if(!strcmp(symbol->node->children[i]->name, "STEPSIZE")){
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[i]){
                    symbol = loopTemp;
                    break;
                }
            }
            break;
        }
    }

    if(!strcmp(symbol->node->name, "STEPSIZE"))
        iSTEPSIZE();

    if(isThereExpr){
        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->prev){
            if(!strcmp(loopTemp->node->name, "EXPR")){
                symbol = loopTemp;
                break;
            }
        }
        goto continueEvaluationForLoop;
    }

    continueExecuteForLoop:

    if(match("cParen")){
        next();
        next();
    }
    if(match("oCurl")){
        next();
        next();

        iSMT();
    }
    else{
        iOneSmt = 1;
        iSMT();
    }

    if(match("st_break")){
        setSymbolToParent();
        setSymbolToParent();
        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }
        goto closeForLoop;
    }
    else if(match("st_cont")){
        setSymbolToParent();
        setSymbolToParent();

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }
    }

    //temp2 = symbol;
    goto continueStepSize;

    closeForLoop:

    if(match("cCurl")){
        next();
        next();
    }
}

void iDO_SMT(){
    char* val = malloc(100);
    int Continue = 0;

    if(match("DO_SMT")) next();
    if(match("lp_do")){
        next();
        next();
    }
    if(match("oCurl")){
        next();
        next();

        iSMT();
    }
    else{
        iOneSmt = 1;
        iSMT();
    }

    if(match("st_break")){
        setSymbolToParent();
        setSymbolToParent();

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }
        Continue = 1;
        goto closeDoLoop;
    }
    else if(match("st_cont")){
        setSymbolToParent();
        setSymbolToParent();

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(!strcmp(loopTemp->node->name, "EXPR")){
                symbol = loopTemp;
                break;
            }
        }

        goto executeContinue;
    }

    if(match("cCurl")){
        next();
        next();
    }

    if(match("lp_while")){
        next();
        next();
    }
    if(match("oParen")){
        next();
        next();
    }

    executeContinue:

    strcpy(val, expr());
    Continue = isTrueCondition(val);

    if(match("cParen")){
        next();
        next();
    }

    //temp2 = symbol;

    if(!Continue){

        setSymbolToParent();

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                break;
            }
        }
        Continue = 1;
        goto closeDoLoop;
    }
    else if(Continue){
        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->prev){
            if(loopTemp->node == symbol->node->parent->parent){
                symbol = loopTemp;
                break;
            }
        }

        iLOOP_SMT();
    }

    closeDoLoop:

    if(match("term_sym")){
        next();
        next();
    }

}

void iWHILE_SMT(){
    char* val = malloc(100);

    if(match("WHILE_SMT")) next();
    if(match("lp_while")){
        next();
        next();
    }
    if(match("oParen")){
        next();
        next();
    }

    strcpy(val, iEXPR());
    Continue = isTrueCondition(val);

    if(!Continue){
        for(loopTemp = symbol; loopTemp != NULL; loopTemp = loopTemp->prev){
            if(loopTemp->node == symbol->node->parent){
                symbol = loopTemp;
                break;
            }
        }

        if(!strcmp(symbol->node->children[symbol->node->childCount-1]->name, "cCurl")){
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    next();
                    next();
                    break;
                }
            }
        }
        else{
           for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(!strcmp(loopTemp->node->name, "SMT")){
                    symbol = loopTemp;
                    next();
                    break;
                }
            }
            for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
                if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                    symbol = loopTemp;
                    next();
                    next();
                    break;
                }
            }
        }

        Continue = 1;
        return;
    }

    if(match("cParen")){
        next();
        next();
    }
    if(match("oCurl")){
        next();
        next();

        iSMT();
    }
    else{
        iOneSmt = 1;
        iSMT();
    }


    if(match("st_break")){
        setSymbolToParent();
        setSymbolToParent();

        for(loopTemp = symbol; loopTemp != NULL ; loopTemp = loopTemp->next){
            if(loopTemp->node == symbol->node->children[symbol->node->childCount-1]){
                symbol = loopTemp;
                next();
                next();
                break;
            }
        }

        Continue = 1;
        return;
    }
    else if(match("st_cont")){
        setSymbolToParent();
        setSymbolToParent();
        iLOOP_SMT();
    }
    else if(Continue){
        if(strcmp(symbol->node->name, "cCurl")){
            symbol = symbol->prev->prev;
            while(strcmp(symbol->node->name, "WHILE_SMT"))
                setSymbolToParent();
        }

        else{
            if(symbol->prev->prev->node->parent != symbol->node->parent){
                symbol = symbol->prev->prev;
                while(strcmp(symbol->node->name, "WHILE_SMT"))
                setSymbolToParent();
            }
            else setSymbolToParent();
        }
        iLOOP_SMT();
    }
}

void iINPUT_SMT(){
    if(match("INPUT_SMT")) next();
    if(match("st_read")){
        next();
        next();
    }
    if(match("ident")){
        next();
        getCurrentID(symbol->node->name);
        next();
    }
    if(match("comma")){
        next();
        next();
    }

    if(iTYPE_SPEC()){
        if(match("%%i")){
            int value;
            scanf("%d", &value);
            sprintf(currentIdent->nodeIdent->value, "%d", value);
        }
        else if(match("%%f")){
            float value;
            scanf("%f", &value);
            sprintf(currentIdent->nodeIdent->value, "%f", value);
        }
        else if(match("%%s")){
            char *value = malloc(100);
            fflush(stdin);
            gets(value);
            sprintf(currentIdent->nodeIdent->value, "%s", value);
        }
        else if(match("%%c")){
            char value;
            scanf("%c", &value);
            sprintf(currentIdent->nodeIdent->value, "%c", value);
        }
        else if(match("%%b")){

        }
        next();
    }

    if(match("term_sym")){
        next();
        next();
    }
}

void iOUTPUT_SMT(){
    char *tempChar = malloc(5);

    if(match("OUTPUT_SMT")) next();
    if(match("st_print")){
        next();
        next();
    }
    if(match("ident")){
        next();
        getCurrentID(symbol->node->name);
        if(!strcmp(currentIdent->nodeIdent->type, "string")){
            int i = 0;
            for(i = 0; i < strlen(currentIdent->nodeIdent->value); i++){
                if(currentIdent->nodeIdent->value[i] == '#'){
                    i++;
                    switch(symbol->node->value[i]){
                        case 'n': OutputMessage("\n"); break;
                        case 't': OutputMessage("\t"); break;
                        case '#': sprintf(tempChar, "#"); break;
                    }
                }
                else{
                    sprintf(tempChar,"%c",currentIdent->nodeIdent->value[i]);
                    OutputMessage(tempChar);
                }
            }
        }
        else OutputMessage(currentIdent->nodeIdent->value);
        next();
    }
    else if(match("val_str")){
        int i = 0;
        char *tempStr = malloc(150);
        next();
        sprintf(tempStr, "%s", symbol->node->value);
        tempStr++[strlen(tempStr)-1] = 0;
        for(i = 0; i < strlen(tempStr); i++){
            if(tempStr[i] == '#'){
                i++;
                switch(tempStr[i]){
                    case 'n': sprintf(tempChar,"\n"); break;
                    case 't': sprintf(tempChar,"\t"); break;
                    case '#': sprintf(tempChar, "#"); break;
                }
                OutputMessage(tempChar);
            }
            else{
                sprintf(tempChar,"%c",tempStr[i]);
                OutputMessage(tempChar);
            }
        }
        next();
    }

    if(match("ccat_out")){
        next();
        next();
        iOUTPUT_SMT();
    }
    else if(match("term_sym")){
        next();
        next();
    }
}

void iDEC_SMT(){
    char *finalValue = malloc(70);
    if(match("DEC_SMT")) next();
    if(match("DATA_TYPE")) next();
    if(iDATA_TYPE()){
        next();
        next();
    }
    if(match("ident")){
        next();
        getCurrentID(symbol->node->name);
        next();
    }
    if(match("term_sym")){
        next();
        if(match(";")) next();
    }
    else if(match("op_assign")){
        next();
        if(match("=")){
            next();
            strcpy(finalValue, iEXPR());
            char *tempStr = malloc(70);

            if(!strcmp(currentIdent->nodeIdent->type, "int")){
                sprintf(currentIdent->nodeIdent->value, "%d", atoi(finalValue));
                sprintf(currentIdent->nodeIdent->parent->value, "%d", atoi(finalValue));
            }
            else if(!strcmp(currentIdent->nodeIdent->type, "string") || !strcmp(currentIdent->nodeIdent->type, "char")){
                sprintf(tempStr, "%s", finalValue);
                tempStr++[strlen(tempStr)-1] = 0;
                sprintf(currentIdent->nodeIdent->value, "%s", tempStr);
                sprintf(currentIdent->nodeIdent->parent->value, "%s", tempStr);
            }
            else{
                sprintf(currentIdent->nodeIdent->value, "%s", finalValue);
                sprintf(currentIdent->nodeIdent->parent->value, "%s", finalValue);
            }
        }
    }

    if(match("comma")){
        next();
        next();
        iDEC_SMT();
    }
    else if(match("term_sym")){
        next();
        if(match(";")) next();
    }
}

int iDATA_TYPE(){
    if(match("dt_bool")
       ||match("dt_char")
       ||match("dt_float")
       ||match("dt_int")
       ||match("dt_string")){
        return 1;
    }
    return 0;
}//iDATA_TYPE()


int iTYPE_SPEC(){
    if(match("type_int")
       ||match("type_float")
       ||match("type_char")
       ||match("type_str")
       ||match("type_bool")){
        next();
        return 1;
    }
    return 0;
}

char* iEXPR(){
    char* val = malloc(100);
    char temp[50];

    if(match("EXPR")){
        next();
        val = iLOG_EXPR();
        storeValue(val);
        backtrack();
        storeValue(val);
    }

    symbol2 = NULL;
    //printf("%s\t===\n",symbol->node->name);
    return val;
}//EXPR()

char* iLOG_EXPR(){
    char* val = malloc(100);
    int logcheck = 0;
    float num;

    if(match("LOG_EXPR")){
        next();
        if(match("log_not")){
            next();
            next();
            logcheck = 1;
        }
        val = iLOG_TERM();
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
        storeValue(val);
        backtrack();
    }


    if(match("LOG_OPERATOR")){
        tempSym4 = symbol2;
        val = iLOG_OPR(val);
        symbol2 = tempSym4;
    }
    /*
    if(match("log_not")){
        current = current->parent;
    */
    return val;
}//LOG_EXPR()

char* iLOG_TERM(){
    char* val = malloc(100);

    if(match("LOG_TERM")){
        next();
        val = iREL_EXPR();
        storeValue(val);
        backtrack();
    }

    return val;
}//LOG_TERM()

char* iLOG_OPR(char *val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    int logcheck = 0;
    float num;

    if(match("LOG_OPERATOR")){
        next();
        if(match("log_and")){
            next();
            next();
            if(match("log_not")){
                next();
                next();
                logcheck = 1;
            }

            tempVal = iLOG_TERM();

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

            storeValue(tempVal);

            if((!strcmp(val, "true") && !strcmp(tempVal, "true"))
                ||(atof(val)!=0 && atof(tempVal)!=0)
                ||(atof(val)!=0 && !strcmp(tempVal, "true"))
                ||(!strcmp(val, "true") && atof(tempVal)!=0)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("log_or")){
            next();
            next();
            if(match("log_not")){
                next();
                next();
                logcheck = 1;
            }

            tempVal = iLOG_TERM();

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

            storeValue(tempVal);
            if((!strcmp(val, "true")||!strcmp(tempVal, "true"))
                ||(atof(val)!=0 || atof(tempVal)!=0)
                ||(atof(val)!=0 || !strcmp(tempVal, "true"))
                ||(!strcmp(val, "true") || atof(tempVal)!=0)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        if(match("LOG_OPERATOR")) val2 = iLOG_OPR(val2);
    }
    return val2;
}

char* iREL_EXPR(){
    char* val = malloc(100);

    if(match("REL_EXPR")){
        next();
        val = iREL_TERM();
        storeValue(val);
        backtrack();
    }

    if(match("REL_OPERATOR")){
        tempSym3 = symbol2;
        val = iREL_OPR(val);
        symbol2 = tempSym3;
    }
    return val;
}

char* iREL_OPR(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);

    if(match("REL_OPERATOR")){
        next();
        if(match("rel_not")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(!strcmp(val, tempVal)){
                strcpy(val2, "false");
            }
            else strcpy(val2, "true");
        }
        else if(match("rel_equal")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(!strcmp(val, tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_great")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(atof(val)>atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_less")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(atof(val)<atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_lqual")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(atof(val)<=atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        else if(match("rel_gqual")){
            next();
            next();
            tempVal = iREL_TERM();
            storeValue(tempVal);
            if(atof(val)>=atof(tempVal)){
                strcpy(val2, "true");
            }
            else strcpy(val2, "false");
        }
        if(match("REL_OPERATOR")) iREL_OPR(val2);
    }

    return val2;
}

char* iREL_TERM(){
    char* val = malloc(100);

    if(match("REL_TERM")){
        next();
        val = iARITH_EXPR();
        storeValue(val);
        backtrack();
    }

    return val;
}//REL_TERM

char* iARITH_EXPR(){
    char* val = malloc(100);

    if(match("ARITH_EXPR")){
        next();
        val = iTERM();
        storeValue(val);
    }

    backtrack();
    storeValue(val);
//printf("%s\n",symbol2->node->name);
    if(match("OPERATOR")){
        tempSym = symbol2;
        val = iOPR(val);
        symbol2 = tempSym;
    }

    return val;
}//ARITH_OPERATOR()

char* iOPR(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    float num;
    int num2;

    if(match("OPERATOR")){
        next();
        if(match("math_add")){
            next();
            next();
            tempVal = iTERM();
            num = atof(val) + atof(tempVal);
            storeValue(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        else if(match("math_sub")){
            next();
            next();
            tempVal = iTERM();
            num = atof(val) - atof(tempVal);
            storeValue(tempVal);

            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        if(match("OPERATOR")) val2 = iOPR(val2);
    }

    return val2;
}

char* iTERM(){
    char* val = malloc(100);

    if(match("TERM")){
        next();
        val = iFACTOR();
        storeValue(val);
    }

    if(match("OPERATOR1")){
        tempSym2 = symbol2;
        val = iOPR1(val);
        symbol2 = tempSym2;
    }

    backtrack();
    return val;
}//TERM()

char* iOPR1(char* val){
    char* val2 = malloc(100);
    char* tempVal = malloc(100);
    float num;
    int num2;

    if(match("OPERATOR1")){
        next();

        if(match("math_mul")){
            next();
            next();
            tempVal = iFACTOR();
            num = atof(val) * atof(tempVal);

            storeValue(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        else if(match("math_div")){
            next();
            next();
            tempVal = iFACTOR();
            num = atof(val) / atof(tempVal);
            storeValue(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }
        else if(match("math_mod")){
            next();
            next();
            tempVal = iFACTOR();
            num = fmod((float)atof(val),(float)atof(tempVal));
            storeValue(tempVal);
            if(fmod(num, 1) == 0){
                num2 = (int)(num);
                sprintf(val2, "%d", num2);
            }
            else sprintf(val2, "%f", num);
        }

        if(match("OPERATOR1")) val2 = iOPR1(val2);
    }

    return val2;
}

char* iFACTOR(){
    char* val = malloc(100);
    char temp[20];
    char* tempVal = malloc(100);
    int num2;
    float num;

    if(match("FACTOR")){
        next();
        if(match("CONST")){
            symbol2 = symbol;
            next();
            if(iCONST()){
                strcpy(val, symbol->node->name);
                strcpy(temp,val);
                next();
                storeValue(val);
                backtrack();
            }
        }//iCONSTs

        else if(match("ident")){
            symbol2 = symbol->next;
            next();
            getCurrentID(symbol->node->name);
            next();
            if(iUNARY()){
                if(match("++")){
                    next();
                    num = (float)atof(currentIdent->nodeIdent->value);
                    num++;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }else sprintf(val, "%f", num);
                    sprintf(currentIdent->nodeIdent->value, "%s", val);
                    sprintf(currentIdent->nodeIdent->parent->value, "%s", val);
                }
                else if(match("--")){
                    next();
                    num = (float)atof(currentIdent->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }else sprintf(val, "%f", num);
                    sprintf(currentIdent->nodeIdent->value, "%s", val);
                    sprintf(currentIdent->nodeIdent->parent->value, "%s", val);
                }
            }
            else sprintf(val, "%s", currentIdent->nodeIdent->value);
            //strcpy(val,currentIdent->nodeIdent->valuegetCurrentID(symbol->node->name););
            strcpy(temp,val);
            //next();
            storeValue(val);
            backtrack();
            storeValue(val);
            backtrack();
        }//identifiers

        else if(match("oParen")){
            tempSym5 = symbol->prev;
            next();
            next();

            tempVal = expr();
            symbol2 = tempSym5;
            if(match("cParen")){
                next();
                next();

            }
            strcpy(val, tempVal);
        }//parenthesis

        else if(match("UNARY_OP")){
            tempSym5 = symbol->prev;
            next();
            if(match("incre")){
                next();
                next();
                if(match("ident")){
                    next();
                    num = (float)atof(currentIdent->nodeIdent->value);
                    num++;
                    getCurrentID(symbol->node->name);
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    symbol2 = tempSym5;
                    next();
                    storeValue(val);
                    backtrack();
                    storeValue(val);
                    backtrack();
                    sprintf(currentIdent->nodeIdent->value, "%s", val);
                    sprintf(currentIdent->nodeIdent->parent->value, "%s", val);
                }
            }
            else if(match("decre")){
                next();
                next();
                if(match("ident")){
                    next();
                    getCurrentID(symbol->node->name);
                    num = (float)atof(currentIdent->nodeIdent->value);
                    num--;
                    if(fmod(num, 1) == 0){
                        num2 = (int)(num);
                        num2;
                        sprintf(val, "%d", num2);
                    }
                    else sprintf(val, "%f", num);
                    symbol2 = tempSym5;
                    next();
                    storeValue(val);
                    backtrack();
                    storeValue(val);
                    backtrack();
                    sprintf(currentIdent->nodeIdent->value, "%s", val);
                    sprintf(currentIdent->nodeIdent->parent->value, "%s", val);
                }
            }
        }//unary_operator
    }
    return val;
}

int iUNARY(){
    if(match("UNARY_OP")) next();
    if(match("incre")||match("decre")){
        next();
        return 1;
   }
   return 0;
}

int iCONST(){
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
        next();
        return 1;
    }
    return 0;
}//iCONST()

int iMATCH(char temp[50]){
    if(!strcmp(symbol->node->name, temp)) return 1;
        return 0;
}

void DFS(){
    int i;
    aTraverse = root;
    iPush(aTraverse);
    while(isEmpty()){
        iPop();
        for(i = aTraverse->childCount; i >= 0 ; i--){
            if(aTraverse->children[i] !=NULL ){
                iPush(aTraverse->children[i]);
            }
        }
        if(top!=NULL) aTraverse = top->node;
    }
}

void iPop(){
    if(top == NULL) return;
    else{
        iPush2(top->node);
        top = (top->prev != NULL)? top->prev: NULL;
        if(top != NULL) top->next = NULL;
    }
    //printf("===========%s\n",front->node->data);
}


void iPush2(struct tree *temp2){
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


void iPush(struct tree *temp1){
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


void displayDFS(){
    char a[100];
    for(temp2 = top2; temp2!=NULL; temp2 = temp2->prev){
        sprintf(a, "<VALUE ( %s ) TYPE ( %s )> Line - %d * ( Level: %d ) -- < %s > Parent: %s \n", temp2->node->value, temp2->node->type, temp2->node->lineNum, temp2->node->level, temp2->node->name, temp2->node->parent->name);
        printf("%s\n",a);
    }
}

int isEmpty(){
    int size = 0;
    struct stack *temp;
    for(temp = top; temp!=NULL; temp = temp->prev){
        size++;
    }
    return size;
}

void next(){
    symbol = (symbol->next != NULL)? symbol->next: NULL;
    //strcpy(symName, symbol->node->name);
}

void storeValue(char* value){
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

void backtrack(){
    symbol2 = (symbol2->prev != NULL)? symbol2->prev: NULL;
}

int isTrueCondition(char *val){
    if(!strcmp(val, "true")
       ||atoi(val) != 0){
        return 1;
    }
    return 0;
}

void getCurrentID(char *name){
    for(temp = head; temp!=NULL; temp = temp->next){
        if(!strcmp(name, temp->nodeIdent->name)){
            currentIdent = temp;
            return;
        }
    }
    return;
}

void display(){
    for(temp = head; temp!=NULL; temp = temp->next)
    printf("IDENT: %s\t VALUE: %s\t TYPE: %s\n", temp->nodeIdent->name, temp->nodeIdent->value, temp->nodeIdent->type);
}

void setSymbolToParent(){
    for(loopTemp = symbol; loopTemp != NULL; loopTemp = loopTemp->prev){
        if(loopTemp->node == symbol->node->parent){
            symbol = loopTemp;
            break;
        }
    }
}
