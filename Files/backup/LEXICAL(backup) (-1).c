#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<malloc.h>
#include<string.h>
#include"LEXICAL.h"

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
FILE *SourceCode, *SymbolTable, *LookUp;
int ln = 1, state = 0, str = 0, decim = 0, key2 = 0, key = 0;
int neg = 0;
char tempLexeme[50];

struct letter{
    char character;
    struct letter *next, *prev;
}*head, *tail, *curr, *x, *y;

struct token{
    int acceptState;
    char tokenName[20];
    struct token *next, *prev;
}*tHead, *tTail, *tCurr, *t;

int LEXICAL(char file[]);
void insertTokens();
void insertCharacters();
void checkingLexeme();
void Output(char token[]);
void lookUp();
int imptSym(char x);

int LEXICAL(char file[]){
    ln = 1, state = 0, str = 0, decim = 0, key2 = 0, key = 0, neg = 0;;
    head = tail = curr = NULL;
    tCurr = tTail = tHead = NULL;
    fclose(SymbolTable);
    remove("SymbolTable.sns");
    insertTokens();

    remove("SymbolTable.sns");
    SymbolTable = fopen("SymbolTable.sns","w+");
    fprintf(SymbolTable, "LEXEME\t\t\t TOKEN\t\t\t STATE\t\t\t\t LINE\n");
    fclose(SymbolTable);

    insertCharacters();
    return 0;
}

void checkingCharacters(){
    x = head;

    while(x!=NULL){
        key2 = 0;
        if(x->character == '\n'||x->character == ' '||x->character == '\t'){
            ln = x->character == '\n'?ln+1:ln;
        }
        else if(x->character == '"'){

            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 907; str = 1;
            fprintf(SymbolTable,"%c",x->character);

            for(x = x ->next; x!=NULL; x = x->next){
                if(x->character=='\t');
                else if(x->character == '\n'){
                    ln++; str = 0;
                    fclose(SymbolTable);
                    lookUp();
                    break;
                }
                else if(x->character=='"'){
                    fprintf(SymbolTable,"%c",x->character);
                    state = 502; str = 0;
                    fclose(SymbolTable);
                    lookUp();
                    break;
                }//======================================================================================================bug
                else fprintf(SymbolTable,"%c",x->character);

            }
            if(str == 1){
                fclose(SymbolTable);
                lookUp();
            }
        }//string

        else if(x->character=='/'&&x->next->character=='/'){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 600;

            for(; x!=NULL;x = x->next){
                if(x->character=='\t');
                else if(x->character=='\n'){
                    break;
                }
                else fprintf(SymbolTable, "%c",x->character);
            }
            fclose(SymbolTable);
            lookUp();
        }//singleline

        else if(x->character=='/'&&x->next->character=='*'){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 601;

            for(; x!=NULL;x = x->next){
                if(x->character=='\t');
                else if(x->character=='\n'){
                    ln++;
                }
                else if(x->character=='*'&&x->next->character=='/'){
                    fprintf(SymbolTable, "%c",x->character);
                    fprintf(SymbolTable, "%c",x->next->character);
                    x = x->next;
                    break;
                }
                else fprintf(SymbolTable, "%c",x->character);
            }
            fclose(SymbolTable);
            lookUp();
        }//block

        else if(x->character=='\''){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 908;
            fprintf(SymbolTable, "%c",x->character);

            for(x= x->next; x!=NULL; x = x->next){
                if(x->character=='\n'){
                    ln++;
                }
                if(x->character=='\''){
                    fprintf(SymbolTable, "%c",x->character);
                    state = x->prev->prev->character == '\''?503:908;
                    break;
                }
                else fprintf(SymbolTable, "%c",x->character);
            }
            fclose(SymbolTable);
            lookUp();
        }//char

        else if(!isalnum(x->character)&&!(x->character=='+'||x->character=='-'||x->character=='*'||x->character=='/'||x->character=='&'
                ||x->character=='!'||x->character=='>'||x->character=='<'||x->character=='='||x->character=='|'
                ||x->character=='%'||x->character=='('||x->character==')'||x->character=='{'||x->character=='\''
                ||x->character=='}'||x->character==':'||x->character==';'||x->character==','||x->character=='_'||x->character=='.')){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 904;
            fprintf(SymbolTable, "%c",x->character);
            for(x = x->next;x!=NULL;x=x->next){
                if(x->character=='\n'||x->character==' '){
                    ln = x->character == '\n'?ln+1:ln;
                    break;
                }
                fprintf(SymbolTable, "%c",x->character);
            }
            fclose(SymbolTable);
            lookUp();
        }//invalid sym

        else if(isdigit(x->character)||x->character=='.'){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 500;
            decim = x->character == '.'?decim+1:decim;
            fprintf(SymbolTable, "%c",x->character);
            for(x = x->next;x!=NULL;x=x->next){
                if(x->character=='\n'||x->character==' '||x->character=='\t'){
                    ln = x->character == '\n'?ln+1:ln;
                    break;
                }
                else if(x->character=='+'||x->character=='-'||x->character=='*'||x->character=='/'||x->character=='&'
                ||x->character=='!'||x->character=='>'||x->character=='<'||x->character=='='||x->character=='|'
                ||x->character=='%'||x->character=='('||x->character==')'||x->character=='{'||x->character=='\''
                ||x->character=='}'||x->character==':'||x->character==';'||x->character==','){
                    x=x->prev;
                    break;
                }
                else if(x->character=='.'){
                    decim++;
                }
                else if(!isdigit(x->character)&&!(x->character=='+'||x->character=='-'||x->character=='*'||x->character=='/'
                ||x->character=='!'||x->character=='>'||x->character=='<'||x->character=='='||x->character=='|'
                ||x->character=='%'||x->character=='('||x->character==')'||x->character=='{'||x->character=='_'
                ||x->character=='}'||x->character==':'||x->character==';'||x->character==','||x->character=='\''||x->character=='&')){
                    decim = 10;
                }
                fprintf(SymbolTable, "%c",x->character);
            }
            state = decim>1?905:(decim==1?501:500);
            decim = 0;
            fclose(SymbolTable);
            lookUp();
        }//numbers

        else if(isalpha(x->character)||x->character=='_'){
            SymbolTable = fopen("SymbolTable.sns","aw+");
            fprintf(SymbolTable, "%c",x->character);
            state = (x->character=='_')?900:250;

            switch(x->character){
                case 'b':{
                    state = 1; if(x!=tail)x=x->next;
                    if(x->character == 'o'){
                        state = 2;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'o'){
                            state = 3;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                 if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)&&imptSym(x->character)){
                                     state = 4;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                 }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'r'){
                        state = 5;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'e'){
                            state = 6;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 7;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'k'){
                                    if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 8;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//b

                case 'c':{
                    state = 9;  if(x!=tail)x=x->next;
                    if(x->character == 'h'){
                        state = 10;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'a'){
                            state = 11;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'r'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                     state = 12;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }
                    else
                    if(x->character == 'o'){
                        state = 13;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'n'){
                            state = 14;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 't'){
                                state = 15;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'i'){
                                    state = 16;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                    if(x->character == 'n'){
                                        state = 17;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                        if(x->character == 'u'){
                                            state = 18;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                            if(x->character == 'e'){
                                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                                    state = 19;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//c

                case 'd':{
                    state = 20;  if(x!=tail)x=x->next;
                    if(x->character == 'e'){
                        state = 21;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'f'){
                            state = 22;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 23;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'u'){
                                    state = 24;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                    if(x->character == 'l'){
                                        state = 25;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                        if(x->character == 't'){
                                            if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                                state = 26;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }
                    else
                    if(x->character == 'o'){
                        if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                           state = 27;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//d

                case 'e':{
                    state = 28;  if(x!=tail)x=x->next;
                    if(x->character == 'l'){
                        state = 29;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 's'){
                            state = 30;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'e'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 31;;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//e

                case 'f':{
                    state = 34;  if(x!=tail)x=x->next;
                    if(x->character == 'a'){
                        state = 35;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'l'){
                            state = 36;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 's'){
                                state = 37;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'e'){
                                    if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 38;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'l'){
                        state = 39;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'o'){
                            state = 40;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 41;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 't'){
                                    if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 42;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'o'){
                        state = 43;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'r'){
                            if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                state = 44;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//f

                case 'i':{
                    state = 45;  if(x!=tail)x=x->next;
                    if(x->character == 'f'){
                        if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                            state = 46;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'n'){
                        state = 47;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 't'){
                            if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                state = 48;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//i

                case 'm':{
                    state = 49;  if(x!=tail)x=x->next;
                    if(x->character == 'a'){
                        state = 50;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 51;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 52;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//m

                case 'n':{
                    state = 53;  if(x!=tail)x=x->next;
                    if(x->character == 'u'){
                        state = 54;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'l'){
                            state = 55;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 56;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//n

                case 'o':{
                    state = 57;  if(x!=tail)x=x->next;
                    if(x->character == 'p'){
                        state = 58;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 't'){
                            state = 59;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'i'){
                                state = 60;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'o'){
                                    state = 61;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                    if(x->character == 'n'){
                                        if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                           state = 62;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//o

                case 'p':{
                    state = 63;  if(x!=tail)x=x->next;
                    if(x->character == 'r'){
                        state = 64;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 65;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                state = 66;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 't'){
                                    if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 67;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//p

                case 'r':{
                    state = 68;  if(x!=tail)x=x->next;
                    if(x->character == 'e'){
                        state = 69;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'a'){
                            state = 70;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'd'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                     state = 71;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//r

                case 's':{
                    state = 72;  if(x!=tail)x=x->next;
                    if(x->character == 't'){
                        state = 73;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'r'){
                            state = 74;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'i'){
                                state = 75;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'n'){
                                    state = 76;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                    if(x->character == 'g'){
                                        if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                            state = 77;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'w'){
                        state = 78;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 79;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 't'){
                                state = 80;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'c'){
                                    state = 81;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                    if(x->character == 'h'){
                                        if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                            state = 82;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//s

                case 't':{
                    state = 83; if(x!=tail)x=x->next;
                    if(x->character == 'r'){
                        state = 84;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'u'){
                            state = 85;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'e'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 86;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else
                    if(x->character == 'h'){
                        state = 250;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'e'){
                            state = 250;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 800;fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//t

                case 'w':{
                    state = 87; if(x!=tail)x=x->next;
                    if(x->character == 'h'){
                        state = 88;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 89;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                state = 90;  fprintf(SymbolTable, "%c",x->character); x=x->next;
                                if(x->character == 'e'){
                                    if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 91; fprintf(SymbolTable, "%c",x->character);fclose(SymbolTable);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;if(imptSym(x->character)){fclose(SymbolTable); lookUp(); goto a;}};
                    break;
                }//w
                default:{
                    key2 = 1;
                    if(x!=tail)x=x->next;
                }
            }//switch

            for(;x!=NULL;x=x->next){
                if(x->character=='\n'||x->character==' '||x->character=='\t'){
                    if(!key||key2){fclose(SymbolTable);lookUp();key=0;key2=0;}
                    ln = x->character == '\n'?ln+1:ln;
                    break;
                }
                else if(x->character=='_'){
                    state = 250;
                    fprintf(SymbolTable, "%c",x->character);
                }
                else if(imptSym(x->character)){
                   if(!key||key2){fclose(SymbolTable);lookUp();key=0;key2=0;}
                    x=x->prev;
                    break;
                }
                else if(!isalnum(x->character)&&!imptSym(x->character)){
                    fprintf(SymbolTable, "%c",x->character);
                    state = 900;
                }
                else fprintf(SymbolTable, "%c",x->character);

                if(x==tail&&!key){fclose(SymbolTable);lookUp();key = 0;};
            }
            fclose(SymbolTable);

        }//ident

        else{
            a:
            SymbolTable = fopen("SymbolTable.sns","aw+");
            state = 903;

            switch(x->character)
            {
                case '+':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 101;
                    if(x!=tail&&x->next->character == '+'){
                        state = 805;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '-':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 102;
                    if(x!=tail&&x->next->character == '-'){
                        state = 810;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '*':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 103;
                    break;
                }
                case '/':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 104;
                    break;
                }
                case '%':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 105;
                    break;
                }
                case '&':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 106;
                    if(x!=tail&&x->next->character == '&'){
                        state = 107;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '|':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 108;
                    if(x!=tail&&x->next->character == '|'){
                        state = 109;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '!':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 110;
                    if(x!=tail&&x->next->character == '='){
                        state = 111;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '>':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 112;
                    if(x!=tail&&x->next->character == '='){
                        state = 113;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '<':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 114;
                    if(x!=tail&&x->next->character == '='){
                        state = 115;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '=':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 116;
                    if(x!=tail&&x->next->character == '='){
                        state = 117;
                        fprintf(SymbolTable, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '(':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 118;
                    decim = 0;
                    if(x->next->character=='-'||x->next->character=='+'){
                        neg = 1;
                        for(y = x->next; y->character!=')'&&y!=NULL ;y=y->next){

                            if(y->character=='\n'||y->character==' '||y->character=='\t'){
                                if(y->character=='\n')
                                    ln = y->character == '\n'?ln+1:ln;
                                break;
                            }
                            else if(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'||y->character=='&'
                            ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                            ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='\''
                            ||y->character=='}'||y->character==':'||y->character==';'||y->character==','){
                                fprintf(SymbolTable, "%c",y->character);
                                state = 900;
                            }
                            else if(y->character=='.'){
                                fprintf(SymbolTable, "%c",y->character);
                                decim++;
                            }
                            else if(!isdigit(y->character)&&!(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'
                            ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                            ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='_'
                            ||y->character=='}'||y->character==':'||y->character==';'||y->character==','||y->character=='\''||y->character=='&')){
                                decim = 10;
                            }
                            else fprintf(SymbolTable, "%c",y->character);

                        }
                    }
                    if(neg){
                        if(decim==0) state = 500;
                        else if(decim == 1) state = 501;
                        else state = 900;
                        decim = 0;
                        x = y;
                        fprintf(SymbolTable, "%c",x->character);
                    }

                    break;
                }
                case ')':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 119;
                    break;
                }
                case '{':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 120;
                    break;
                }
                case '}':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 121;
                    break;
                }
                case ';':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 122;
                    break;
                }
                case ':':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 123;
                    break;
                }
                case ',':{
                    fprintf(SymbolTable, "%c",x->character);
                    state = 124;
                    break;
                }
            }//switch

            fclose(SymbolTable);
            lookUp();
        }//symbols

        x = (x==NULL)?x:x->next;
    }
}

void insertCharacters(){
    char temp;
    SourceCode = fopen("SourceCode.sns","rw+");

    while(fscanf(SourceCode,"%c",&temp) != EOF){
        curr = (struct letter*) malloc(sizeof(struct letter));
        curr -> next = curr -> prev = NULL;

        if (head == NULL){
            head = tail = curr;
            curr->character = temp;
        }//if
        else{
            tail -> next = curr;
            curr -> prev = tail;
            tail = curr;
            curr->character = temp;
        }//else
    }//while

    checkingCharacters();

    fclose(SourceCode);
    tail -> next = NULL;
}

int imptSym(char x){
    if(x=='+'||x=='-'||x=='*'||x=='/'
    ||x=='!'||x=='>'||x=='<'||x=='='||x=='|'
    ||x=='%'||x=='('||x==')'||x=='{'||x=='&'
    ||x=='}'||x==':'||x==';'||x==','||x=='\''||x=='"')
        return 1;
    return 0;
}

void Output(char token[]){
    SymbolTable = fopen("SymbolTable.sns","aw+");

    fprintf(SymbolTable, "\t\t\t %s\t\t %d\t\t\t %d\n", token, state, ln);

    fclose(SymbolTable);
}


void lookUp(){
    for(t = tHead; t != NULL; t = t -> next){
        if(state == t->acceptState)
            Output(t->tokenName);
    }
}

void insertTokens(){
    LookUp = fopen("LookUpTable.sns","r+");
    int aState;
    char token[20];

    while(fscanf(LookUp,"%s %d",&token,&aState) != EOF){
        tCurr = (struct token*) malloc(sizeof(struct token));
        tCurr -> next = tCurr -> prev = NULL;
        if (tHead == NULL){
            tHead = tTail = tCurr;
            tCurr -> acceptState = aState;
            strcpy(tCurr->tokenName, token);
        }//if
        else{
            tTail -> next = tCurr;
            tCurr -> prev = tTail;
            tTail = tCurr;
            tCurr -> acceptState = aState;
            strcpy(tCurr -> tokenName, token);
        }//else
    }//while

    fclose(LookUp);
    tTail -> next = NULL;
}//insertToken()



