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
FILE *SourceCode, *Symbol, *LookUp;
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
}*t, *tHead;

void insertCharacters();
void checkingLexeme();
void Output(char token[]);
void lookUp();
int imptSym(char x);

int LEXICAL(char file[], struct token *tokenHead){
//int main(char file[], struct token *tokenHead){

    ln = 1;
    state = str = decim = key2 =  key =  neg = 0;

    head = tail = curr = NULL;
    tHead = tokenHead;

    fclose(Symbol);
    remove("SymbolTable.sns");
    Symbol = fopen("SymbolTable.sns","w+");
    fprintf(Symbol, "LEXEME\t\t\t TOKEN\t\t\t STATE\t\t\t\t LINE\n");
    fclose(Symbol);

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

            Symbol = fopen("SymbolTable.sns","aw+");
            state = 907; str = 1;
            fprintf(Symbol,"%c",x->character);

            for(x = x ->next; x!=NULL; x = x->next){
                if(x->character=='\t');
                else if(x->character == '\n'){
                    ln++; str = 0;
                    fclose(Symbol);
                    lookUp();
                    break;
                }
                else if(x->character=='"'){
                    fprintf(Symbol,"%c",x->character);
                    state = 502; str = 0;
                    fclose(Symbol);
                    lookUp();
                    break;
                }//======================================================================================================bug
                else fprintf(Symbol,"%c",x->character);

            }
            if(str == 1){
                fclose(Symbol);
                lookUp();
            }
        }//string

        else if(x->character=='/'&&x->next->character=='/'){
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 600;

            for(; x!=NULL;x = x->next){
                if(x->character=='\t');
                else if(x->character=='\n'){
                    break;
                }
                else fprintf(Symbol, "%c",x->character);
            }
            fclose(Symbol);
            lookUp();
        }//singleline

        else if(x->character=='/'&&x->next->character=='*'){
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 601;

            for(; x!=NULL;x = x->next){
                if(x->character=='\t');
                else if(x->character=='\n'){
                    ln++;
                }
                else if(x->character=='*'&&x->next->character=='/'){
                    fprintf(Symbol, "%c",x->character);
                    fprintf(Symbol, "%c",x->next->character);
                    x = x->next;
                    break;
                }
                else fprintf(Symbol, "%c",x->character);
            }
            fclose(Symbol);
            lookUp();
        }//block

        else if(x->character=='\''){
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 908;
            fprintf(Symbol, "%c",x->character);

            for(x= x->next; x!=NULL; x = x->next){
                if(x->character=='\n'){
                    ln++;
                }
                if(x->character=='\''){
                    fprintf(Symbol, "%c",x->character);
                    state = x->prev->prev->character == '\''?503:908;
                    break;
                }
                else fprintf(Symbol, "%c",x->character);
            }
            fclose(Symbol);
            lookUp();
        }//char

        else if(!isalnum(x->character)&&!(x->character=='+'||x->character=='-'||x->character=='*'||x->character=='/'||x->character=='&'
                ||x->character=='!'||x->character=='>'||x->character=='<'||x->character=='='||x->character=='|'
                ||x->character=='%'||x->character=='('||x->character==')'||x->character=='{'||x->character=='\''
                ||x->character=='}'||x->character==':'||x->character==';'||x->character==','||x->character=='_'||x->character=='.')){
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 904;
            fprintf(Symbol, "%c",x->character);
            for(x = x->next;x!=NULL;x=x->next){
                if(x->character=='\n'||x->character==' '){
                    ln = x->character == '\n'?ln+1:ln;
                    break;
                }
                fprintf(Symbol, "%c",x->character);
            }
            fclose(Symbol);
            lookUp();
        }//invalid sym

        else if(isdigit(x->character)||x->character=='.'){
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 500;
            decim = x->character == '.'?decim+1:decim;
            fprintf(Symbol, "%c",x->character);
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
                fprintf(Symbol, "%c",x->character);
            }
            state = decim>1?905:(decim==1?501:500);
            decim = 0;
            fclose(Symbol);
            lookUp();
        }//numbers

        else if(isalpha(x->character)||x->character=='_'){
            Symbol = fopen("SymbolTable.sns","aw+");
            fprintf(Symbol, "%c",x->character);
            state = (x->character=='_')?900:250;

            switch(x->character){
                case 'b':{
                    state = 1; if(x!=tail)x=x->next;
                    if(x->character == 'o'){
                        state = 2;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'o'){
                            state = 3;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                 if(x==tail||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)&&imptSym(x->character)){
                                     state = 4;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                 }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'r'){
                        state = 5;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'e'){
                            state = 6;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 7;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'k'){
                                    if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 8;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//b

                case 'c':{
                    state = 9;  if(x!=tail)x=x->next;
                    if(x->character == 'h'){
                        state = 10;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'a'){
                            state = 11;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'r'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                     state = 12;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }
                    else
                    if(x->character == 'o'){
                        state = 13;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'n'){
                            state = 14;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 't'){
                                state = 15;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'i'){
                                    state = 16;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'n'){
                                        state = 17;  fprintf(Symbol, "%c",x->character); x=x->next;
                                        if(x->character == 'u'){
                                            state = 18;  fprintf(Symbol, "%c",x->character); x=x->next;
                                            if(x->character == 'e'){
                                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                                    state = 19;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//c

                case 'd':{
                    state = 20;  if(x!=tail)x=x->next;
                    if(x->character == 'e'){
                        state = 21;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'f'){
                            state = 22;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 23;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'u'){
                                    state = 24;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'l'){
                                        state = 25;  fprintf(Symbol, "%c",x->character); x=x->next;
                                        if(x->character == 't'){
                                            if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                                state = 26;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }
                    else
                    if(x->character == 'o'){
                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                           state = 27;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//d

                case 'e':{
                    state = 28;  if(x!=tail)x=x->next;
                    if(x->character == 'l'){
                        state = 29;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 's'){
                            state = 30;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'e'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 31;;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//e

                case 'f':{
                    state = 34;  if(x!=tail)x=x->next;
                    if(x->character == 'a'){
                        state = 35;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'l'){
                            state = 36;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 's'){
                                state = 37;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'e'){
                                    if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 38;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'l'){
                        state = 39;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'o'){
                            state = 40;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'a'){
                                state = 41;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 't'){
                                    if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 42;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'o'){
                        state = 43;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'r'){
                            if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                state = 44;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//f

                case 'i':{
                    state = 45;  if(x!=tail)x=x->next;
                    if(x->character == 'f'){
                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                            state = 46;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'n'){
                        state = 47;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 't'){
                            if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                state = 48;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//i

                case 'm':{
                    state = 49;  if(x!=tail)x=x->next;
                    if(x->character == 'a'){
                        state = 50;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 51;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 52;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//m

                case 'n':{
                    state = 53;  if(x!=tail)x=x->next;
                    if(x->character == 'u'){
                        state = 54;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'l'){
                            state = 55;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 56;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//n

                case 'o':{
                    state = 57;  if(x!=tail)x=x->next;
                    if(x->character == 'p'){
                        state = 58;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 't'){
                            state = 59;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'i'){
                                state = 60;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'o'){
                                    state = 61;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'n'){
                                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                           state = 62;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//o

                case 'p':{
                    state = 63;  if(x!=tail)x=x->next;
                    if(x->character == 'r'){
                        state = 64;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 65;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                state = 66;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 't'){
                                    if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 67;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//p

                case 'r':{
                    state = 68;  if(x!=tail)x=x->next;
                    if(x->character == 'e'){
                        state = 69;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'a'){
                            state = 70;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'd'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                     state = 71;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//r

                case 's':{
                    state = 72;  if(x!=tail)x=x->next;
                    if(x->character == 't'){
                        state = 73;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'r'){
                            state = 74;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'i'){
                                state = 75;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'n'){
                                    state = 76;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'g'){
                                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                            state = 77;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'w'){
                        state = 78;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 79;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 't'){
                                state = 80;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'c'){
                                    state = 81;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'h'){
                                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                            state = 82;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//s

                case 't':{
                    state = 83; if(x!=tail)x=x->next;
                    if(x->character == 'r'){
                        state = 84;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'u'){
                            state = 85;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'e'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 86;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'h'){
                        state = 250;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'e'){
                            state = 250;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'n'){
                                if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                    state = 800;fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//t

                case 'x':{
                    state = 1994; if(x!=tail)x=x->next;
                    if(x->character == 'S'){
                        state = 1995;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'T'){
                            state = 1996;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'A'){
                                state = 1997;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'R'){
                                    state = 1998;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'T'){
                                        if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                            state = 2000; fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else
                    if(x->character == 'F'){
                        state = 2001;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'I'){
                            state = 2002;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'N'){
                                state = 2003;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'I'){
                                    state = 2004;  fprintf(Symbol, "%c",x->character); x=x->next;
                                    if(x->character == 'S'){
                                        state = 2005;  fprintf(Symbol, "%c",x->character); x=x->next;
                                        if(x->character == 'H'){
                                            if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                                state = 2007; fprintf(Symbol, "%c",x->character); fclose(Symbol);lookUp(); key=1;
                                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//x

                case 'w':{
                    state = 87; if(x!=tail)x=x->next;
                    if(x->character == 'h'){
                        state = 88;  fprintf(Symbol, "%c",x->character); x=x->next;
                        if(x->character == 'i'){
                            state = 89;  fprintf(Symbol, "%c",x->character); x=x->next;
                            if(x->character == 'l'){
                                state = 90;  fprintf(Symbol, "%c",x->character); x=x->next;
                                if(x->character == 'e'){
                                    if(x==tail||x->next->character=='\t'||x->next->character=='\n'||x->next->character==' '||imptSym(x->next->character)){
                                        state = 91; fprintf(Symbol, "%c",x->character);fclose(Symbol);lookUp(); key=1;
                                    }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                                }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                            }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                        }else {key2 = 1; state = 250;   if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    }else {key2 = 1; state = 250;if(imptSym(x->character)){fclose(Symbol); lookUp(); goto a;}};
                    break;
                }//w
                default:{
                    key2 = 1;
                    if(x!=tail)x=x->next;
                }
            }//switch

            for(;x!=NULL;x=x->next){
                if(x->character=='\n'||x->character==' '||x->character=='\t'){
                    if(!key||key2){fclose(Symbol);lookUp();key=0;key2=0;}
                    ln = x->character == '\n'?ln+1:ln;
                    break;
                }
                else if(x->character=='_'){
                    state = 250;
                    fprintf(Symbol, "%c",x->character);
                }
                else if(imptSym(x->character)){
                   if(!key||key2){fclose(Symbol);lookUp();key=0;key2=0;}
                    x=x->prev;
                    break;
                }
                else if(!isalnum(x->character)&&!imptSym(x->character)){
                    fprintf(Symbol, "%c",x->character);
                    state = 900;
                }
                else fprintf(Symbol, "%c",x->character);

                if(x==tail&&!key){fclose(Symbol);lookUp();key = 0;};
            }
            fclose(Symbol);

        }//ident

        else{
            a:
            Symbol = fopen("SymbolTable.sns","aw+");
            state = 903;

            switch(x->character)
            {
                case '+':{
                    decim = 0;
                    neg=0;
                    fprintf(Symbol, "%c",x->character);
                    state = 101;
                    if(x!=tail&&x->next->character == '+'){
                        state = 805;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }
                    else if((x->prev->prev->character==' '
                        && !(isalnum(x->prev->prev->prev->character)))
                        ||(!(isdigit(x->prev->prev->character)))){

                        if(isdigit(x->next->character) && !(isalnum(x->prev->character))){
                            for(y = x->next; y->character!=','&&y->character!=' '&& y->character!=';' ;y=y->next){
                                if(y->character=='\n'||y->character==' '||y->character=='\t'){
                                    if(y->character=='\n')
                                        ln = y->character == '\n'?ln+1:ln;
                                    break;
                                }
                                else if(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'||y->character=='&'
                                ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                                ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='\''
                                ||y->character=='}'||y->character==':'||y->character==';'||y->character==','){
                                    break;
                                }
                                else if(y->character=='.'){
                                    fprintf(Symbol, "%c",y->character);
                                    decim++;
                                    neg=1;
                                }
                                else if(!isdigit(y->character)&&!(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'
                                ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                                ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='_'
                                ||y->character=='}'||y->character==':'||y->character==';'||y->character==','||y->character=='\''||y->character=='&')){
                                    decim = 10;
                                    neg=1;
                                }
                                else{
                                    neg=1;
                                    fprintf(Symbol, "%c",y->character);
                                }
                            }
                        }
                        if(neg){
                        if(decim==0) state = 500;
                        else if(decim == 1) state = 501;
                        else state = 900;
                        decim = 0;
                        x = y->prev;
                        }
                    }

                    break;
                }
                case '-':{
                    decim = 0;
                    neg=0;
                    fprintf(Symbol, "%c",x->character);
                    state = 102;
                    if(x!=tail&&x->next->character == '-'){
                        state = 810;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }
                    else if((x->prev->prev->character==' '
                        && !(isalnum(x->prev->prev->prev->character)))
                        ||(!(isdigit(x->prev->prev->character)))){

                        if(isdigit(x->next->character) && !(isalnum(x->prev->character))){
                            for(y = x->next; y->character!=','&&y->character!=' '&& y->character!=';' ;y=y->next){
                                if(y->character=='\n'||y->character==' '||y->character=='\t'){
                                    if(y->character=='\n')
                                        ln = y->character == '\n'?ln+1:ln;
                                    break;
                                }
                                else if(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'||y->character=='&'
                                ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                                ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='\''
                                ||y->character=='}'||y->character==':'||y->character==';'||y->character==','){
                                    break;
                                }
                                else if(y->character=='.'){
                                    fprintf(Symbol, "%c",y->character);
                                    decim++;
                                    neg=1;
                                }
                                else if(!isdigit(y->character)&&!(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'
                                ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                                ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='_'
                                ||y->character=='}'||y->character==':'||y->character==';'||y->character==','||y->character=='\''||y->character=='&')){
                                    decim = 10;
                                    neg=1;
                                }
                                else{
                                    neg=1;
                                    fprintf(Symbol, "%c",y->character);
                                }
                            }
                        }
                        if(neg){
                        if(decim==0) state = 500;
                        else if(decim == 1) state = 501;
                        else state = 900;
                        decim = 0;
                        x = y->prev;
                        }
                    }

                    break;
                }
                case '*':{
                    fprintf(Symbol, "%c",x->character);
                    state = 103;
                    break;
                }
                case '/':{
                    fprintf(Symbol, "%c",x->character);
                    state = 104;
                    break;
                }
                case '%':{
                    fprintf(Symbol, "%c",x->character);
                    state = 105;
                    if(x!=tail&&x->next->character == 'i'){
                        state = 720;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }else
                    if(x!=tail&&x->next->character == 'f'){
                        state = 721;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }else
                    if(x!=tail&&x->next->character == 's'){
                        state = 722;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }else
                    if(x!=tail&&x->next->character == 'c'){
                        state = 723;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }else
                    if(x!=tail&&x->next->character == 'b'){
                        state = 724;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                        break;
                    }
                    break;
                }

                case '&':{
                    fprintf(Symbol, "%c",x->character);
                    state = 106;
                    if(x!=tail&&x->next->character == '&'){
                        state = 107;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '|':{
                    fprintf(Symbol, "%c",x->character);
                    state = 108;
                    if(x!=tail&&x->next->character == '|'){
                        state = 109;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '!':{
                    fprintf(Symbol, "%c",x->character);
                    state = 110;
                    if(x!=tail&&x->next->character == '='){
                        state = 111;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '>':{
                    fprintf(Symbol, "%c",x->character);
                    state = 112;
                    if(x!=tail&&x->next->character == '='){
                        state = 113;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '<':{
                    fprintf(Symbol, "%c",x->character);
                    state = 114;
                    if(x!=tail&&x->next->character == '='){
                        state = 115;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '=':{
                    fprintf(Symbol, "%c",x->character);
                    state = 116;
                    if(x!=tail&&x->next->character == '='){
                        state = 117;
                        fprintf(Symbol, "%c",x->next->character);
                        x = x->next;
                    }
                    break;
                }
                case '(':{
                    fprintf(Symbol, "%c",x->character);
                    state = 118;
                    decim = 0;
                    neg=0;
                    if(x->next->character==' '||x->next->character=='-'||x->next->character=='+'){//printf("pass0---\n");
                       if(x->next->character==' ') x = x->next;
                        for(y = x->next; y->character!=')'&&y!=NULL;y=y->next){
                            if(y->character=='\n'||y->character==' '||y->character=='\t'){//printf("pass1---\n");
                                if(y->character=='\n')
                                    ln = y->character == '\n'?ln+1:ln;
                                break;
                            }
                            else if(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'||y->character=='&'
                            ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                            ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='\''
                            ||y->character=='}'||y->character==':'||y->character==';'||y->character==','){//printf("pass2---\n");

                                if(y->next->character=='+'||y->next->character=='-'||y->next->character=='*'||y->next->character=='/'||y->next->character=='&'
                            ||y->next->character=='!'||y->next->character=='>'||y->next->character=='<'||y->next->character=='='||y->next->character=='|'
                            ||y->next->character=='%'||y->next->character=='('||y->next->character==')'||y->next->character=='{'||y->next->character=='\''
                            ||y->next->character=='}'||y->next->character==':'||y->next->character==';'||y->next->character==',')
                                fprintf(Symbol,"%c",y->prev->prev->prev->character);
                                else if(y->character=='+'||y->character=='-'&& isdigit(y->next->character)){
                                    if(y->next->character!=' ')
                                        fprintf(Symbol, "%c",y->character);
                                    else
                                        fprintf(Symbol,"%c",y->prev->prev->character);
                                    //printf("invo1--\n");
                                }
                                else fprintf(Symbol,"%c",y->prev->prev->character);
                                neg=0;
                                state = 118;
                                //state = 900;

                            }
                            else if(y->character=='.'){//printf("pass3---\n");
                                fprintf(Symbol, "%c",y->character);
                                decim++;neg=1;
                            }
                            else if(!isdigit(y->character)&&!(y->character=='+'||y->character=='-'||y->character=='*'||y->character=='/'
                            ||y->character=='!'||y->character=='>'||y->character=='<'||y->character=='='||y->character=='|'
                            ||y->character=='%'||y->character=='('||y->character==')'||y->character=='{'||y->character=='_'
                            ||y->character=='}'||y->character==':'||y->character==';'||y->character==','||y->character=='\''||y->character=='&')){
                                decim = 10;//printf("pass4---\n");
                                neg=1;
                            }
                            else{ neg=1;fprintf(Symbol, "%c",y->character); //printf("pass5---\n");
                            }

                        }
                    }
                  if(neg){//printf("pass6---\n");
                        if(decim==0) state = 500;
                        else if(decim == 1) state = 501;
                        else state = 900;
                        decim = 0;
                        x = y;
                        fprintf(Symbol, "%c",x->character);
                    }
                    break;
                }
                case ')':{
                    fprintf(Symbol, "%c",x->character);
                    state = 119;
                    break;
                }
                case '{':{
                    fprintf(Symbol, "%c",x->character);
                    state = 120;
                    break;
                }
                case '}':{
                    fprintf(Symbol, "%c",x->character);
                    state = 121;
                    break;
                }
                case ';':{
                    fprintf(Symbol, "%c",x->character);
                    state = 122;
                    break;
                }
                case ':':{
                    fprintf(Symbol, "%c",x->character);
                    state = 123;
                    break;
                }
                case ',':{
                    fprintf(Symbol, "%c",x->character);
                    state = 124;
                    break;
                }
            }//switch

            fclose(Symbol);
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
    Symbol = fopen("SymbolTable.sns","aw+");

    fprintf(Symbol, "\t\t\t %s\t\t %d\t\t\t %d\n", token, state, ln);

    fclose(Symbol);
}


void lookUp(){
    for(t = tHead; t != NULL; t = t -> next){
        if(state == t->acceptState)
            Output(t->tokenName);
    }
}




