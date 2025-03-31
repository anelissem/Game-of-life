#ifndef STIVELIB
#define STIVELIB

struct list{
    int l,c;
    struct lista *next;
};
typedef struct lista LIST;
struct stack{
    LIST *generation;
    struct stack *next;
};
typedef struct stack GEN;

void push(GEN **stacktop,LIST *generatie);
int isEmpty(GEN *stacktop);
LIST* pop(GEN **stacktop);
void deleteStack(GEN **stacktop);

#endif