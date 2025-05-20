#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void push(GEN **stacktop, LIST *head)
{
    if (stacktop == NULL)
        return;
    GEN *NewNode = (GEN *)malloc(sizeof(GEN));
    if (NewNode == NULL)
    {
        printf("Error allocating memory for stack node\n");
        return;
    }
    NewNode->generation = head;
    NewNode->next = *stacktop;
    *stacktop = NewNode;
}
LIST *pop(GEN **stacktop)
{
    if (stacktop == NULL || *stacktop == NULL)
    {
        return NULL;
    }
    GEN *temp = *stacktop;
    LIST *generation = temp->generation;
    *stacktop = (*stacktop)->next;
    free(temp);
    return generation;
}

void deleteStack(GEN **stacktop)
{
    if (stacktop == NULL)
        return;
    while (*stacktop != NULL)
    {
        GEN *temp = *stacktop;
        *stacktop = (*stacktop)->next;
        if (temp->generation != NULL)
        {
            DeleteList(&(temp->generation));
        }
        free(temp);
    }
}

void AddAtEnd(LIST **head, int l, int c)
{
    if (head == NULL)
        return;
    LIST *NewNode = (LIST *)malloc(sizeof(LIST));
    if (NewNode == NULL)
    {
        printf("Error allocating memory for list node\n");
        return;
    }
    NewNode->l = l;
    NewNode->c = c;
    NewNode->next = NULL;
    if (*head == NULL)
    {
        *head = NewNode;
        return;
    }
    LIST *aux = *head;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    aux->next = NewNode;
}

void DeleteList(LIST **head)
{
    if (head == NULL)
        return;
    while (*head != NULL)
    {
        LIST *aux = *head;
        *head = (*head)->next;
        free(aux);
    }
}

void print(CEL **matrix, int N, int M, const char *argv[])
{
    FILE *f = fopen(argv[2], "a+");
    if (f == NULL)
    {
        printf("Error opening output file ");
        return;
    }
    for (int i = 0; i < N; i++)
    {
        if (matrix[i] == NULL)
        {
            fclose(f);
            return;
        }
        for (int j = 0; j < M; j++)
        {
            fprintf(f, "%c", matrix[i][j].state);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);
}

void print_list(LIST *head, const char *argv[], int K)
{
    if (head == NULL || argv == NULL)
        return;

    FILE *f = fopen(argv[2], "a+");
    if (f == NULL)
    {
        printf("Error opening output file\n");
        return;
    }

    fprintf(f, "%d ", K);
    LIST *current = head;
    while (current != NULL)
    {
        fprintf(f, "%d %d", current->l, current->c);
        current = current->next;
        if (current != NULL)
            fprintf(f, " ");
    }
    fprintf(f, "\n");
    fclose(f);
}

GEN *create_stack_matrix(const char *argv[], CEL **matrix, int N, int M, int K, int T)
{
    if (argv == NULL || matrix == NULL)
        return NULL;

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Error reopening input file\n");
        return NULL;
    }

    char linie[256];
    if (fgets(linie, sizeof(linie), f) == NULL ||
        fgets(linie, sizeof(linie), f) == NULL)
    {
        fclose(f);
        return NULL;
    }

    for (int j = 0; j < N; j++)
    {
        if (fgets(linie, sizeof(linie), f) == NULL)
        {
            fclose(f);
            return NULL;
        }
        for (int q = 0; q < M && q < (int)strlen(linie); q++)
        {
            matrix[j][q].state = linie[q];
            matrix[j][q].neighbors = 0;
        }
    }
    if (fgets(linie, sizeof(linie), f) == NULL)
    {
        fclose(f);
        return NULL;
    }
    GEN *stacktop = NULL;
    while (fgets(linie, sizeof(linie), f) != NULL)
    {
        LIST *head = NULL;
        const char *t = strtok(linie, " ");
        int pair[2], count = 0;

        while (t != NULL && count < 2)
        {
            pair[count++] = atoi(t); // ce face fct asta
            if (count == 2)
            {
                if (pair[0] >= 0 && pair[0] < N && pair[1] >= 0 && pair[1] < M)
                {
                    AddAtEnd(&head, pair[0], pair[1]);
                }
                count = 0;
            }
            t = strtok(NULL, " ");
        }

        if (head != NULL)
        {
            push(&stacktop, head);
        }
    }

    fclose(f);
    return stacktop;
}

void make_first_generation(const char *argv[], GEN **stacktop, CEL **matrix, int N, int M, int K, int T)
{
    if (stacktop == NULL || matrix == NULL)
        return;

    while (*stacktop != NULL)
    {
        LIST *head = pop(stacktop);
        LIST *current = head;
        while (current != NULL)
        {
            int l = current->l;
            int c = current->c;
            if (l >= 0 && l < N && c >= 0 && c < M)
            {
                if (matrix[l][c].state == '+')
                {
                    matrix[l][c].state = 'X';
                }
                else
                {
                    matrix[l][c].state = '+';
                }
            }
            current = current->next;
        }
        DeleteList(&head);
    }
}

void rules_stack(CEL **matrix, int N, int M, int K, GEN **stacktop, const char *argv[], int T)
{
    if (T == 1)
    {
        print(matrix, N, M, argv);
    }
    int change_whole = 0;
    while (change_whole < K)
    {
        count_neighbors(matrix, N, M);
        LIST *head = NULL;
        int changes = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (matrix[i][j].state == 'X')
                {
                    if (matrix[i][j].neighbors < 2 || matrix[i][j].neighbors > 3)
                    {
                        AddAtEnd(&head, i, j);
                        changes++;
                    }
                }
                else if (matrix[i][j].state == '+')
                {
                    if (matrix[i][j].neighbors == 3)
                    {
                        AddAtEnd(&head, i, j);
                        changes++;
                    }
                }
            }
        }

        if (changes > 0)
        {
            change_whole++;
            LIST *current = head;
            while (current != NULL)
            {
                int l = current->l;
                int c = current->c;
                if (l >= 0 && l < N && c >= 0 && c < M)
                {
                    matrix[l][c].state = (matrix[l][c].state == 'X') ? '+' : 'X';
                }
                current = current->next;
            }

            if (T == 1)
            {
                print(matrix, N, M, argv);
            }
            else if (T == 2)
            {
                print_list(head, argv, change_whole);
            }
            push(stacktop, head);
        }
        else
        {
            DeleteList(&head);
            break;
        }
    }
}
TREE *createNode(LIST *head)
{
    TREE *newNode = (TREE *)malloc(sizeof(TREE));
    if (newNode == NULL)
    {
        printf("Error at allocating memory for a node of  tree");
        return NULL;
    }
    newNode->cells = head;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void rule_A(CEL **matrix, int N, int M)
{
    if (matrix == NULL)
        return;
    /// numar numarul de vecini pentru fiecare celula
    count_neighbors(matrix, N, M);
    /// aplic regula A
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j].state == 'X')
            {
                if (matrix[i][j].neighbors < 2 || matrix[i][j].neighbors > 3)
                {
                    matrix[i][j].state = '+';
                }
            }
            else if (matrix[i][j].state == '+')
            {
                if (matrix[i][j].neighbors == 3)
                {
                    matrix[i][j].state = 'X';
                }
            }
        }
    }
}
void rule_B(CEL **matrix, int N, int M)
{
    if (matrix == NULL)
        return;
    /// numar numarul de vecini pentru fiecare celula
    count_neighbors(matrix, N, M);
    /// aplic regula B
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j].state == '+')
            {
                if (matrix[i][j].neighbors == 2)
                {
                    matrix[i][j].state = 'X';
                }
            }
        }
    }
}
TREE *create_initial_tree(CEL **matrix, LIST *gen0, int N, int M)
{
    if (*matrix == NULL)
        return NULL;
    TREE *node = createNode(gen0);
    if (node == NULL)
    {
        DeleteList(&gen0);
    }
    return node;
}
void create_list(CEL **prev, CEL **curr, LIST **head, int N, int M)
{
    if (prev == NULL || curr == NULL || head == NULL)
        return;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (prev[i][j].state != curr[i][j].state)
            {
                AddAtEnd(head, i, j);
            }
        }
    }
}

CEL **make_copy(CEL **matrix, int N, int M)
{
    if (matrix == NULL)
        return NULL;
    CEL **matrixB = (CEL **)malloc(N * sizeof(CEL *));
    if (matrixB == NULL)
    {
        printf("Error at the memory allocation for the rows of the matrix\n");
        return NULL;
    }
    for (int i = 0; i < N; i++)
    {
        matrixB[i] = (CEL *)malloc(M * sizeof(CEL));
        if (matrixB[i] == NULL)
        {
            printf("Error at the memory allocation for the rows of the matrix\n");
            for (int j = 0; j < i; j++)
            {
                free(matrixB[j]);
            }
            free(matrixB);
            return NULL;
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrixB[i][j].state = matrix[i][j].state;
            matrixB[i][j].neighbors = matrix[i][j].neighbors;
        }
    }
    return matrixB;
}

void delete_tree(TREE **root)
{
    if (*root == NULL)
    {
        return;
    }
    delete_tree(&((*root)->left));
    delete_tree(&((*root)->right));
    if ((*root)->cells != NULL)
    {
        DeleteList(&((*root)->cells));
    }
    free(*root);
    *root = NULL;
}
void count_neighbors(CEL **matrix, int N, int M)
{
    if (matrix == NULL)
        return;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j].neighbors = 0;

            // Check all 8 possible neighbors
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (di == 0 && dj == 0)
                        continue; // Skip self

                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < N && nj >= 0 && nj < M)
                    {
                        if (matrix[ni][nj].state == 'X')
                        {
                            matrix[i][j].neighbors++;
                        }
                    }
                }
            }
        }
    }
}

void change_rules(TREE **root, CEL **matrix, const char *argv[], int N, int M, int K, int T)
{
    if ((*root) == NULL || matrix == NULL || K <= 0)
    {
        return;
    }
    LIST *listaA = NULL, *listaB = NULL;
    CEL **newmatrixA = make_copy(matrix, N, M);
    CEL **newmatrixB = make_copy(matrix, N, M);

    if (newmatrixA == NULL || newmatrixB == NULL)
    {
        if (newmatrixA)
        {
            for (int i = 0; i < N; i++)
                free(newmatrixA[i]);
            free(newmatrixA);
        }
        if (newmatrixB)
        {
            for (int i = 0; i < N; i++)
                free(newmatrixB[i]);
            free(newmatrixB);
        }
        return;
    }

    rule_A(newmatrixA, N, M);
    rule_B(newmatrixB, N, M);
    create_list(matrix, newmatrixA, &listaA, N, M);
    create_list(matrix, newmatrixB, &listaB, N, M);

    (*root)->left = createNode(listaB);
    if( T == 3 )
        print(newmatrixB, N, M, argv);
    change_rules(&((*root)->left), newmatrixB, argv, N, M, K - 1, T);

    (*root)->right = createNode(listaA);
     if( T == 3 )
        print(newmatrixA, N, M, argv);
    change_rules(&((*root)->right), newmatrixA, argv, N, M, K - 1, T);

    for (int i = 0; i < N; i++)
    {
        free(newmatrixA[i]);
        free(newmatrixB[i]);
    }
    free(newmatrixA);
    free(newmatrixB);
}
void deleteGraph(G *g){
    if(g == NULL)
        return;
    for(int i = 0 ; i < g->V; i++){
        free(g->adiacenta[i]);
    }
    free(g->adiacenta);
    free(g);
    g=NULL;
}
/*NOD_GRAF *find_neighbor(CEL **matrice,NOD_GRAF *visited, int N, int M, int l, int c){
    if(matrice == NULL)
        return NULL;
    NOD_GRAF *vecini=(NOD_GRAF *)malloc(8*sizeof(NOD_GRAF));
    if(vecini == NULL)
    {
        printf("Error at the memory allocation for the neighbors\n");
        return NULL;
    }
    if(l-1 >= 0 && c-1 >= 0 && matrice[l-1][c-1].state == 'X'){
        vecini[0].l=l-1;
        vecini[0].c=c-1;
        
    }
    else if(l-1 >= 0 && c+1 <= M && matrice[l-1][c+1].state == 'X'){
        vecini[1].l=l-1;
        vecini[1].c=c+1;
    }
    else if(l+1 <= N && c-1 >= 0 && matrice[l+1][c-1].state == 'X'){
        vecini[2].l=l+1;
        vecini[2].c=c-1;
    }
    else if(l+1 <= N && c+1 <= M && matrice[l+1][c+1].state == 'X'){
        vecini[3].l=l+1;
        vecini[3].c=c+1;
    }
    else if(l-1 >= 0 && matrice[l-1][c].state == 'X'){
        vecini[4].l=l-1;
        vecini[4].c=c;
    }
    else if(l+1 <= N && matrice[l+1][c].state == 'X'){
        vecini[5].l=l+1;
        vecini[5].c=c;
    }
    else if(c-1 >= 0 && matrice[l][c-1].state == 'X'){
        vecini[6].l=l;
        vecini[6].c=c-1;
    }
    else if(c+1 <= M && matrice[l][c+1].state == 'X'){
        vecini[7].l=l;
        vecini[7].c=c+1;
    }
    return vecini;
}*/