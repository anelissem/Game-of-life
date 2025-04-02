#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
void push(GEN **stacktop,LIST *generation){
    //creating a newnode for the stack, the list containing the cells that have changed their state
    GEN *NewNode=(GEN*)malloc(sizeof(GEN));
    NewNode->generation=generation;
    NewNode->next=*stacktop;
    *stacktop=NewNode;
}
int isEmpty(GEN *stacktop){
    if(stacktop==NULL)
        return 1;
    else
        return 0;
}
LIST* pop(GEN **stacktop){
    if(isEmpty(*stacktop))
        return NULL;
    LIST *aux=(*stacktop)->generation;
    GEN *temp=*stacktop;
    (*stacktop)=(*stacktop)->next;
    free(temp);
    return aux; 
}
void deleteStack(GEN **stacktop){
    GEN *temp;
    while(isEmpty(*stacktop)!=1){
        temp=*stacktop;
        *stacktop=(*stacktop)->next;
        free(temp);
    }
    free(*stacktop);
}
void AddAtBeginning(LIST **head,int l,int c){
    LIST *NewNode=(LIST*)malloc(sizeof(LIST));
    if(NewNode==NULL)
        printf("Error at allocating memory for the new node of the list");
    NewNode->l=l;
    NewNode->c=c;
    NewNode->next=*head;
    *head=NewNode;
}
void AddAtEnd(LIST **head,int l,int c){
    LIST *NewNode=(LIST*)malloc(sizeof(LIST));
    if(NewNode==NULL)
        printf("Error at allocating memory for the new node of the list");
    NewNode->l=l;
    NewNode->c=c;
    NewNode->next=NULL;
    LIST *aux;
    aux=*head;
    while(aux->next!=NULL){
        aux=aux->next;
    }
    aux->next=NewNode;
}

void DeleteList(LIST **head){
    LIST *aux;
    while((*head)!=NULL){
        aux=*head;
        *head=(*head)->next;
        free(aux);
    }
    free(*head);
}

void print_list(LIST *head,const char* argv[],int K){
    FILE *f;
    if ((f = fopen(argv[2], "a+")) == NULL)
    {
      printf("Error at opening the output file\n");
      return;
    }
    fprintf(f,"%d ",K);
    while(head->next!=NULL){
        fprintf(f,"%d %d ",head->l,head->c);
        head=head->next;
    }
    fprintf(f,"%d %d\n",head->l,head->c);
    fclose(f);
}