#ifndef STIVELIB
#define STIVELIB

#include <stdbool.h>
//structura pentru lista de celule
struct list {
    int l, c;
    struct list *next;
};
typedef struct list LIST;
//structura pentru stiva de generatii
struct stack {
    LIST *generation;
    struct stack *next;
};
typedef struct stack GEN;
//structura pentru celula din matrice
struct celula {
    char state;//starea celulei, vie sau moarta
    int neighbors;//numarul de vecini
};
typedef struct celula CEL;
//structura pentru arborele de generatii
struct arbore {
    LIST *cells;
    struct arbore *left, *right;
};
typedef struct arbore TREE;
//structura pentru graf
struct graph{
    int V;//numarul de noduri din graf
    int **adiacenta;//matricea de adiacenta
};
typedef struct graph G;
//structura pentru un nod din graf
struct nod{
        int l,c;
};
typedef struct nod NOD_GRAF;
//structura pentru un drum in graf
struct cale {
    int lungime;
    int *path;
};
typedef struct cale HPath;
//structura pentru informatii despre un drum
typedef struct {
    int* path;       // Indecșii nodurilor din lanț
    int length;      // Lungimea lanțului
    bool is_valid;   // Dacă lanțul este valid
} PathInfo;
//structura cu informatii despre un nod din graf
typedef struct {
    int node;
    int degree;
} NodeInfo;
//structura 
typedef struct {
    int* nodes;
    int count;
} Component;
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
void create_list(CEL **inainte, CEL **dupa, LIST **head, int N, int M) ;
void rule_B(CEL **matrix, int N, int M);
void rule_A(CEL **matrix, int N, int M);
TREE *create_initial_tree(CEL **matrix, LIST *gen0, int N, int M);
void count_neighbors(CEL **matrix, int N, int M);
void deleteGraph(G *g);
void printadiacenta(int **matrix, int N, int M, const char *argv[]);
void task4(TREE* root, CEL** matrix, int N, int M, const char* argv[]) ;
void find_path(G *g, NOD_GRAF *noduri, HPath *maxPath, NOD_GRAF *current, int *visited, int lungimea_curenta, int nod);
void buildGraph(G *g, NOD_GRAF *nodes, CEL **matrix, int N, int M);
void findComponents(G *g, Component **components, int *count);
///void dfsComponent(G *g, int node, bool *visited, int *list, int *size);
void hamiltonianInComponent(G *g, NOD_GRAF *nodes, Component comp, HPath *best);
void backtrackHamiltonian(G *g, NOD_GRAF *nodes, Component comp, int *visited, int *path, int depth, HPath *best, HPath *current);
int cmpNodes(const void *a, const void *b);
void dfs_longest(G *g, int node, int *visited, NOD_GRAF *nodes, int depth,HPath *best, int *path, int stop_len);
int bfs_farthest_node(G *g, int start, int *dist_out, int *parent_out);
void change_state(CEL **matrix, LIST *head);
void componente_conexe(G *g, int nod, int *visited, int *vector, int *nr);
void held_karp(G *g, NOD_GRAF *nodes, Component comp, HPath *best);
void dfs_bitmask(G *g, int current, int mask, int depth, Component comp,  HPath *best, int *path, NOD_GRAF *nodes, int stop_len) ;
void same_path(HPath *cale, NOD_GRAF *nodes);
int compare_paths(int *a, int *b, int len, NOD_GRAF *nodes);
#endif