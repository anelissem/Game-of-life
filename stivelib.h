#ifndef STIVELIB
#define STIVELIB

struct list{
    int l,c;
    struct list *next;
};
typedef struct list LIST;
struct stack{
    LIST *generation;
    struct stack *next;
};
typedef struct stack GEN;

void push(GEN **stacktop,LIST *generatie);
int isEmpty(GEN *stacktop);
LIST* pop(GEN **stacktop);
void deleteStack(GEN **stacktop);
void AddAtBeginning(LIST **head,int l,int c);
void AddAtEnd(LIST **head,int l,int c);
void DeleteList(LIST **head);
void print_list(LIST *head,const char* argv[],int K);

#endif