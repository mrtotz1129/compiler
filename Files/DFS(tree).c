#include <stdio.h>
#include <conio.h>

typedef struct tree{
    char data[50];
    int level;
    int childCount;
    struct tree *children[100], *parent;
}Tree;

typedef struct stack{
    struct tree *node;
    struct stack *up, *down;
}stack;

typedef struct stack2{
    struct tree *node;
    struct stack *up, *down;
}stack2;

struct stack *top, *curr;
struct stack2 *top2, *curr2;
struct tree *current, *root, *traverse;

void push(struct tree *temp);
void pop();

void push2(struct tree *temp);
void pop2();

void insertNode(struct tree *tempParent, char n[], int level, int position);
void displaystack();
int stackSize();
void DFSearch();
Tree* newNode(char* token, int level);

void main(){
    current = root = traverse= NULL;
    top = curr = NULL;
    top2 = curr2 = NULL;
    insertNode(root, "E", 0, 1);
    insertNode(root, "E", 1, 1);
    insertNode(root, "+", 1, 1);
    insertNode(root, "T", 1, 1);
    insertNode(root->children[0], "T",2 ,1);
    insertNode(root->children[0]->children[0], "F",2 ,1);
    insertNode(root->children[0]->children[0]->children[0], "NUM2",3 ,1);
    insertNode(root->children[2], "T",3, 1);
    insertNode(root->children[2], "*",3, 1);
    insertNode(root->children[2], "F",3, 1);
    insertNode(root->children[2]->children[0], "F",3, 1);
    insertNode(root->children[2]->children[0]->children[0], "NUM3",3, 1);
    //insertNode(root->children[2]->children[2], "",3, 1);
    insertNode(root->children[2]->children[2], "NUM4",3, 1);
    DFSearch();
    displaystack();
    getch();
}

void DFSearch(){
    int i;
    traverse = root;
    push(traverse);
    while(stackSize()){
        if(!traverse->childCount) pop();
        else{
            for(i = traverse->childCount; i >= 0 ; i--){
                if(traverse->children[i] !=NULL ){
                    push(traverse->children[i]);
                    traverse->childCount--;
                }
            }
        }
        if(top!=NULL) traverse = top->node;
    }
}

void push(struct tree *temp1){
    curr = (struct stack*)malloc(sizeof(struct stack));
    curr->up = curr->down = NULL;
    if(top == NULL){
        top = curr;
        curr->node = temp1;
    }
    else{
        top->up = curr;
        curr->down = top;
        top = curr;
        curr->node = temp1;
    }
    top->up = NULL;
}

void pop(){
    if(top == NULL) return;
    else{
        push2(top->node);
        //printf("destack: <%s>\n",front->node->data);
        top = top->down != NULL? top->down: NULL;
        if(top != NULL) top->up = NULL;
    }
    //printf("===========%s\n",front->node->data);
}


void push2(struct tree *temp2){
    curr2 = (struct stack2*)malloc(sizeof(struct stack2));
    curr2->up = curr2->down = NULL;
    if(top2 == NULL){
        top2 = curr2;
        curr2->node = temp2;
    }
    else{
        top2->up = curr2;
        curr2->down = top2;
        top2 = curr2;
        curr2->node = temp2;
    }
    top2->up = NULL;
}


void displaystack(){
    stack2 *temp2;
    printf("\n");
    if(temp2 == NULL);
    else
    for(temp2 = top2; temp2!=NULL; temp2 = temp2->down){

        printf("* <%s - level %d> = parent: %s\n",temp2->node->data, temp2->node->level, temp2->node->parent);

    }
}

int stackSize(){
    int size = 0;
    struct stack *temp;
    for(temp = top; temp!=NULL; temp = temp->down){
        //printf("%s\n", temp->node->data);
        size++;
    }
    return size;
}

void insertNode(struct tree *tempParent, char n[], int level, int position){
    Tree *node1;
    node1 = newNode(n, level);

    if(tempParent!=NULL){
        node1->parent = tempParent;
        tempParent->children[tempParent->childCount] = node1;
        tempParent->childCount++;
    }
    else root = node1;
}


Tree* newNode(char* token, int level) {
	Tree *node;
	int i;
    node = (struct tree*) malloc(sizeof(struct tree));
    for(i = 0; i < 100; i++){
        node->children[i] = NULL;
    }
    node->childCount = 0;
    node->level = level;
    node->parent = NULL;
    strcpy(node->data, token);

	return node;
}
