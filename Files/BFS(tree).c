#include <stdio.h>
#include <conio.h>

typedef struct tree{
    char data[50];
    int level;
    int childCount;
    struct tree *children[10], *parent;
}Tree;

typedef struct queue{
    struct tree *node;
    struct queue *next, *prev;
};

struct queue *back, *curr, *front;
struct tree *current, *root, *traverse;

void enqueue(struct tree *temp);
void dequeue();
void insertNode(struct tree *tempParent, char n[], int level, int position);
void displayQueue();
int queueSize();
void BFSearch();
Tree* newNode(char* token, int level);

void main(){
    current = root = traverse= NULL;
    front = back = curr = NULL;
    insertNode(root, "parent1", 0, 1);
    insertNode(root, "child_1", 1, 1);
    insertNode(root, "child_2", 1, 1);
    insertNode(root, "child_3", 1, 1);
    printf("%s\t%s\t%s\t%s\n",root->data,root->children[0]->data,root->children[1]->data,root->children[2]->data);
    insertNode(root->children[0], "son_1",2 ,1);
    insertNode(root->children[0], "son_2",2 ,1);
    printf("\t%s\t%s\n",root->children[0]->children[0]->data,root->children[0]->children[1]->data);
    insertNode(root->children[1], "daughter_1",3 ,1);
    insertNode(root->children[1], "daughter_2",3, 1);
    printf("\t\t%s\t%s\n",root->children[1]->children[0]->data,root->children[1]->children[1]->data);
    insertNode(root->children[2], "daughter_4",3 ,1);
    insertNode(root->children[2], "daughter_5",3, 1);
    printf("\t\t%s\t%s\n",root->children[2]->children[0]->data,root->children[5]->children[1]->data);
    BFSearch();
    getch();
}

void BFSearch(){
    int i;
    traverse = root;
    enqueue(traverse);
    while(queueSize()){
        for(i = 0; i < 10; i++){
            if(traverse->children[i] !=NULL ){
                enqueue(traverse->children[i]);
            }
        }
        printf("* <%s - %d>\n",traverse->data, traverse->level);
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
        //printf("dequeue: <%s>\n",front->node->data);
        front = front->next != NULL? front->next: NULL;
        if(front != NULL) front->prev = NULL;
    }
    //printf("===========%s\n",front->node->data);
}

void displayQueue(){
    struct queue *temp;
    for(temp = front; temp!=NULL; temp = temp->next){
        printf("%s\n", temp->node->data);
    }
}

int queueSize(){
    int size = 0;
    struct queue *temp;
    for(temp = front; temp!=NULL; temp = temp->next){
        //printf("%s\n", temp->node->data);
        size++;
    }
    return size;
}

void insertNode(struct tree *tempParent, char n[], int level, int position){

    current = newNode(n, level);

    if(tempParent!=NULL){
        current->parent = tempParent;
        tempParent->children[tempParent->childCount] = current;
        tempParent->childCount++;
    }
    else root = current;
}


Tree* newNode(char* token, int level) {
	Tree *node;
	int i;
    node = (struct tree*) malloc(sizeof(struct tree));
    for(i = 0; i < 10; i++){
        node->children[i] = NULL;
    }
    node->childCount = 0;
    node->level = level;
    node->parent = NULL;
    strcpy(node->data, token);

	return node;
}
