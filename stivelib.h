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
struct celula
{
  char state;
  int neighbors;
};
typedef struct celula CEL;
void print(CEL **matrix, int N, int M, const char *argv[]);
void rules_stack(CEL **matrix, int N, int M, int K,GEN **stacktop, const char *argv[],int T);
void push(GEN **stacktop,LIST *head);
LIST *pop(GEN **stacktop);
int isEmpty(GEN *stacktop);
void printStack(GEN *stacktop);
void deleteStack(GEN **stacktop);
void AddAtBeginning(LIST **head,int l,int c);
void AddAtEnd(LIST **head,int l,int c);
void DeleteList(LIST **head);
void print_list(LIST *head,const char* argv[],int K);
GEN *create_stack_matrix(const char *argv[],CEL **matrix,int N,int M,int K,int T);
void make_first_generation(const char *argv[],GEN **stacktop,CEL **matrix,int N,int M,int K,int T);
#endif