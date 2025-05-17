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

int isEmpty(GEN *stacktop)
{
    return (stacktop == NULL);
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

void AddAtBeginning(LIST **head, int l, int c)
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
    NewNode->next = *head;
    *head = NewNode;
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

    // Read matrix
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

    // Skip empty line
    if (fgets(linie, sizeof(linie), f) == NULL)
    {
        fclose(f);
        return NULL;
    }

    GEN *stacktop = NULL;
    while (fgets(linie, sizeof(linie), f) != NULL)
    {
        LIST *head = NULL;
        char *t = strtok(linie, " ");
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
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                matrix[i][j].neighbors = 0;
            }
        }
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (matrix[i][j].state == 'X')
                {
                    for (int di = -1; di <= 1; di++)
                    {
                        for (int dj = -1; dj <= 1; dj++)
                        {
                            if (di == 0 && dj == 0)
                                continue;

                            int ni = i + di;
                            int nj = j + dj;

                            if (ni >= 0 && ni < N && nj >= 0 && nj < M)
                            {
                                matrix[ni][nj].neighbors++;
                            }
                        }
                    }
                }
            }
        }

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
void count_neighbors(CEL **matrix, int N, int M)
{
    ///reinitializez numarul de vecini pentru fiecare celula
    if (matrix == NULL)
        return;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j].neighbors = 0;
        }
    }
    for(int i = 0 ; i < N; i++){
        for(int j = 0 ; j < M ; j ++){
            if(matrix[i][j-1].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i][j+1].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i-1][j].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i+1][j].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i-1][j-1].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i-1][j+1].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i+1][j-1].state == 'X'){
                matrix[i][j].neighbors++;
            }
            if(matrix[i+1][j+1].state == 'X'){
                matrix[i][j].neighbors++;
            }
        }
    }
}
void rule_A(CEL **matrix, int N, int M){
    if(matrix == NULL)
        return;
    ///numar numarul de vecini pentru fiecare celula
    count_neighbors(matrix, N, M);
    ///aplic regula B
    for(int i = 0 ; i < N; i++){
        for(int j = 0 ; j < M ; j ++){
            if(matrix[i][j].state == 'X'){
                if(matrix[i][j].neighbors < 2 || matrix[i][j].neighbors > 3){
                    matrix[i][j].state = '+';
                }
            }
            else if(matrix[i][j].state == '+'){
                if(matrix[i][j].neighbors == 3){
                    matrix[i][j].state = 'X';
                }
            }
        }
    }
}
void rule_B(CEL **matrix, int N, int M){
    if(matrix == NULL)
        return;
    ///numar numarul de vecini pentru fiecare celula
    count_neighbors(matrix, N, M);
    ///aplic regula A
    for(int i = 0 ; i < N; i++){
        for(int j = 0 ; j < M ; j ++){
            if(matrix[i][j].state == 'X'){
                if(matrix[i][j].neighbors < 2 || matrix[i][j].neighbors > 3){
                    matrix[i][j].state = '+';
                }
            }
            else if(matrix[i][j].state == '+'){
                if(matrix[i][j].neighbors == 3){
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

void change_rules(TREE **root, CEL **matrix, CEL **matrixB, const char *argv[], int N, int M, int K, int T,int adancime)
{
    if(*root == NULL || matrix == NULL || matrixB == NULL || adancime >= K)
    {
        return;
    }
    // initializez numarul de vecini pentru ambele matrice
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j].neighbors = 0;
            matrixB[i][j].neighbors = 0;
        }
    }
    // ma ocup prima data de nodul stang
    // numar numarul de vecini pentru fiecare celula din matricea B
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrixB[i][j].state == 'X')
            {
                for (int di = -1; di <= 1; di++)
                {
                    for (int dj = -1; dj <= 1; dj++)
                    {
                        if (di == 0 && dj == 0)
                            continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < N && nj >= 0 && nj < M)
                        {
                            matrixB[ni][nj].neighbors++;
                        }
                    }
                }
            }
        }
    }
    // creez lista ce se salveaza in fiecare nod stang
    LIST *headB = NULL;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrixB[i][j].state == '+' && matrixB[i][j].neighbors == 2)
            {
                AddAtEnd(&headB, i, j);
                matrixB[i][j].state = 'X';
            }
        }
    }
    // ma duc pe nodul urmator pe subarborele stang
    if (headB != NULL)
    { // daca lista nu e goala
        (*root)->left = createNode(headB);
        ///print(matrixB, N, M, argv);
        if ((*root)->left)
        {
            LIST *current = (*root)->left->cells;
            while (current != NULL)
            {
                matrix[current->l][current->c].state = (matrix[current->l][current->c].state == 'X') ? '+' : 'X';
                current = current->next;
            }
            // print(matrixB, N, M, argv);
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    matrix[i][j].state = matrixB[i][j].state;
                }
            }
            
            change_rules(&((*root)->left), matrix, matrixB, argv, N, M, K , T,adancime+1);
        }
    }
    // calculez numarul de vecini pentru matrica din copilul drept
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j].state == 'X')
            {
                for (int di = -1; di <= 1; di++)
                {
                    for (int dj = -1; dj <= 1; dj++)
                    {
                        if (di == 0 && dj == 0)
                            continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < N && nj >= 0 && nj < M)
                        {
                            matrix[ni][nj].neighbors++;
                        }
                    }
                }
            }
        }
    }
    // fac lista cu celulele de se modifica
    LIST *head = NULL;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j].state == 'X')
            {
                if (matrix[i][j].neighbors < 2 || matrix[i][j].neighbors > 3)
                {
                    AddAtEnd(&head, i, j);
                    matrix[i][j].state = '+';
                }
            }
            else if (matrix[i][j].state == '+')
            {
                if (matrix[i][j].neighbors == 3)
                {
                    AddAtEnd(&head, i, j);
                    matrix[i][j].state = 'X';
                }
            }
        }
    }
    // daca am celule care se modifica adaug lista cu ele in nodul drept
    if (head != NULL)
    {
        (*root)->right = createNode(head);
        if ((*root)->right)
        {
            ///print(matrix, N, M, argv);
            LIST *current = (*root)->right->cells;
            while (current != NULL)
            {
                matrix[current->l][current->c].state = (matrix[current->l][current->c].state == 'X') ? '+' : 'X';
                current = current->next;
            }
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    matrixB[i][j].state = matrix[i][j].state;
                }
            }
            change_rules(&((*root)->right), matrix, matrixB, argv, N, M, K, T,adancime+1);
        }
    }
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
/*void preorder(TREE *root, CEL **matrix, int N, int M, const char *argv[])
{
    if (root == NULL)
    {
        return;
    }
    if (root->cells != NULL)
    {
        LIST *current = root->cells;
        while (current != NULL)
        {
            matrix[current->l][current->c].state = (matrix[current->l][current->c].state == 'X') ? '+' : 'X';
            current = current->next;
        }
    }
    print(matrix, N, M, argv);
    preorder(root->left, matrix, N, M, argv);
    preorder(root->right, matrix, N, M, argv);
}*/