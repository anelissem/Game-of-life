#ifndef STIVELIB
#define STIVELIB

struct list {
    int l, c;
    struct list *next;
};
typedef struct list LIST;

struct stack {
    LIST *generation;
    struct stack *next;
};
typedef struct stack GEN;

struct celula {
    char state;
    int neighbors;
};
typedef struct celula CEL;

struct arbore {
    LIST *cells;
    struct arbore *left, *right;
};
typedef struct arbore TREE;
struct graph{
    int V;//numarul de noduri din graf
    int val;//valoarea atribuita pentru nod in matricea de adiacenta
    int **adiacenta;//matricea de adiacenta
};
typedef struct graph G;
struct nod{
        int l,c;
        int val;
        int vizitat;
};
typedef struct nod NOD_GRAF;
void print(CEL **matrix, int N, int M, const char *argv[]);
void rules_stack(CEL **matrix, int N, int M, int K, GEN **stacktop, const char *argv[], int T);
void push(GEN **stacktop, LIST *head);
LIST *pop(GEN **stacktop);
int isEmpty(GEN *stacktop);
void printStack(GEN *stacktop);
void deleteStack(GEN **stacktop);
void AddAtBeginning(LIST **head, int l, int c);
void AddAtEnd(LIST **head, int l, int c);
void DeleteList(LIST **head);
void print_list(LIST *head, const char *argv[], int K);
GEN *create_stack_matrix(const char *argv[], CEL **matrix, int N, int M, int K, int T);
void make_first_generation(const char *argv[], GEN **stacktop, CEL **matrix, int N, int M, int K, int T);
TREE *createNode(LIST *head);
void change_rules(TREE **root, CEL **matrix,const char *argv[], int N, int M, int K, int T);
void delete_tree(TREE **root);
CEL **make_copy(CEL **matrix, int N, int M);
void create_list(CEL **prev, CEL **curr, LIST **head, int N, int M) ;
void rule_B(CEL **matrix, int N, int M);
void rule_A(CEL **matrix, int N, int M);
TREE *create_initial_tree(CEL **matrix, LIST *gen0, int N, int M);
void count_neighbors(CEL **matrix, int N, int M);
void deleteGraph(G *g);
NOD_GRAF *find_neighbor(CEL **matrice, int N, int M, int l, int c);
#endif