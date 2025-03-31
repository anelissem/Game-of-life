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