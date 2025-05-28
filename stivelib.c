#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
/// functia push pentru stiva
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
/// functia pop pentru stiva 
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
//functia de stergere a stivei
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
// functia ce adauga la sfarsitul unei liste
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
//functia ce sterge lista
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
//functia ce printeaza o matrice tip game of life
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
//functia prin care printez o matrice de adiacenta pentru graf pentru a verifica crearea corecta a matricilor de adiacenta din task 4
void printadiacenta(int **matrix, int N, int M, const char *argv[])
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
            fprintf(f, "%d", matrix[i][j]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);
}
//functia ce printeaza componentele unei liste
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
//functia prin care creez o stiva pe baza citirii dintr-un fisier pentru a face bonusul
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

    for (int i = 0; i < N; i++)
    {
        if (fgets(linie, sizeof(linie), f) == NULL)
        {
            fclose(f);
            return NULL;
        }
        for (int j = 0; j < M && j < (int)strlen(linie); j++)
        {
            matrix[i][j].state = linie[j];
            matrix[i][j].neighbors = 0;
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
        int pair[2];
        int count = 0;
        //citesc cate doua numere
        while (t != NULL && count < 2)
        {
            pair[count++] = atoi(t); 
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
//functie ce creeaza prima generatie a matricei pe baza stivei obtinute din fisierul de citire, bonusul
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
//functia ce rezolva atat primul task cat si al doilea task
void rules_stack(CEL **matrix, int N, int M, int K, GEN **stacktop, const char *argv[], int T)
{
    //prima data printez prima generatie
    if (T == 1)
    {
        print(matrix, N, M, argv);
    }
    //numar schimbarile din matrice, o schimbare e considerata dupa ce am parcurs intreaga generatie curenta, astfel tin evidenta la ce generatie ma aflu
    int change_whole = 0;
    while (change_whole < K)
    {
        //apelez functia ce imi numara vecinii unei celule
        count_neighbors(matrix, N, M);
        LIST *head = NULL;
        int changes = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                //aplic prima regula a jocului si salvez schimbarea in lista pentru generatia actuala
                //adaug la sfarsit de lista pentru a nu modifica ordinea in care se fac schimbarile
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
        //daca cel putin o celula si a modificat starea, asta inseamna ca voi obtine o noua generatie
        if (changes > 0)
        {
            change_whole++;
            LIST *current = head;
            //aplic schimbarile de stare a celulelor pe matricea curenta
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
            //daca am task ul 1 de rezolvat printez matricea obtinuta
            //daca am task ul 2 de rezolvat printez lista cu coordonatele celulelor ce si-au schimbat starea
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
            //daca nu si-a modificat nicio celula starea, atunci nu mai am cum sa obtin alta generatie deci ies din bucla
            DeleteList(&head);
            break;
        }
    }
}
//functia ce imi creeaza un nod de arbore ce are salvat ca valoare lista cu celulele ce si au modificat starea fata de generatia anterioara
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
//functia prin care aplic prima regula a jocului asupra unei generatii
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
//functia prin care aplic a doua regula a jocului asupra unei generatii
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
//functia prin care creez radacina pentru arbore, in radacina e salvata lista ce cuprinde coordonatele celulelor vii din generatia 0
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
//functia prin care obtin lista ce contine coordonatele celulelor ce si-au modificat starea fata de generatia anterioara
void create_list(CEL **inainte, CEL **dupa, LIST **head, int N, int M)
{
    if (inainte == NULL || dupa == NULL || head == NULL)
        return;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (inainte[i][j].state != dupa[i][j].state)
            {
                AddAtEnd(head, i, j);
            }
        }
    }
}
// functie ce face o copie a matricei pentru a nu modifica matricea initiala atunci cand apelez recursiv in preorder in arbore
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
//functia ce sterge arborele 
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
//functia ce numara vecinii unei celule, o celula are maxim 8 vecini
void count_neighbors(CEL **matrix, int N, int M)
{
    if (matrix == NULL)
        return;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j].neighbors = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (di == 0 && dj == 0)
                        continue; //nu numar celula in sine

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
//functia prin care creez arborele ce retine in fiecare nod lista cu coordonatele celulelor ce si-au schimbat starea fata de generatia anterioara, este o functie tip preorder, recursiva ce aplica regula A asupara subarborelui drept in care se mentine regula initiala a jocului si regula B asupara subarborelui stang in care se mentine regula noua a jocului
void change_rules(TREE **root, CEL **matrix, const char *argv[], int N, int M, int K, int T)
{
    if ((*root) == NULL || matrix == NULL || K <= 0)
    {
        return;
    }
    //initializez cele doua liste in care voi salva individual schimbarile celulelor
    LIST *listaA = NULL, *listaB = NULL;
    //fac copii asupra matricei initiale pentru a putea implementa corect regulile fara a se suprapune
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
    //aplic regulile asupra celor doua copii ale matricei
    rule_A(newmatrixA, N, M);
    rule_B(newmatrixB, N, M);
    //creeaza listele cu coordonatele celulelor ce si-au schimbat starea fata de generatia anterioara
    create_list(matrix, newmatrixA, &listaA, N, M);
    create_list(matrix, newmatrixB, &listaB, N, M);
    //fac nodul din stanga
    (*root)->left = createNode(listaB);
    //daca am task ul 3 de rezolvat, adica sa printez matricea dupa aplicarea regulii B, atunci o fac
    if (T == 3)
        print(newmatrixB, N, M, argv);
    //apelez recursiv functia pentru a continua sa creez arborele
    change_rules(&((*root)->left), newmatrixB, argv, N, M, K - 1, T);
    //fac nodul din dreapta
    (*root)->right = createNode(listaA);
    //daca am task ul 3 de rezolvat, adica sa printez matricea dupa aplicarea regulii A, atunci o fac
    if (T == 3)
        print(newmatrixA, N, M, argv);
    //apelez recursiv functia pentru a continua sa creez arborele
    change_rules(&((*root)->right), newmatrixA, argv, N, M, K - 1, T);
    // eliberez memoria alocata pentru listele cu coordonatele celulelor ce si-au schimbat starea
    for (int i = 0; i < N; i++)
    {
        free(newmatrixA[i]);
        free(newmatrixB[i]);
    }
    free(newmatrixA);
    free(newmatrixB);
}
//functia ce imi sterge graful
void deleteGraph(G *g)
{
    if (g == NULL)
        return;
    for (int i = 0; i < g->V; i++)
    {
        free(g->adiacenta[i]);
    }
    free(g->adiacenta);
    free(g);
    g = NULL;
}
/*
//aici am o incercare de backtracking pentru a gasi cel mai lung lant dintr-un graf, imi crapa la timpul de executie
void find_path(G *g, NOD_GRAF *noduri, HPath *maxPath, NOD_GRAF *current, int *visited, int lungimea_curenta, int nod)
{
    /// marchez nodul curent ca vizitat
    visited[nod] = 1;
    /// il pun in lantul actual
    current[lungimea_curenta] = noduri[nod];
    /// actualizez lungimea lantului
    lungimea_curenta++;
    /// daca lungimea lantului curent este mai mare decat lungimea maxima
    /// atunci actualizez lungimea maxima si salvez lantul curent
    if (lungimea_curenta > maxPath->lungime)
    {
        maxPath->lungime = lungimea_curenta;
        if (maxPath->path != NULL)
        {
            free(maxPath->path);
        }
        /// copiez lantul curent in lantul maxim
        maxPath->path = (NOD_GRAF *)malloc(lungimea_curenta * sizeof(NOD_GRAF));
        if (maxPath->path)
        {
            for (int i = 0; i < lungimea_curenta; i++)
            {
                maxPath->path[i] = current[i];
            }
        }
    }
    /// ma duc la toti vecinii nodului curent
    for (int i = 0; i < g->V; i++)
    {
        if (g->adiacenta[nod][i] && !visited[i])
        {
            find_path(g, noduri, maxPath, current, visited, lungimea_curenta, i);
        }
    }
    visited[nod] = 0;
}
void task4(TREE *root, CEL **matrix, int N, int M, const char *argv[])
{
    FILE *f = fopen(argv[2], "a+");
    if (f == NULL)
    {
        printf("Error opening output file\n");
        return;
    }

    if (root == NULL)
        return;
    // eu primesc radacina arborelui pentru fiecare apel
    // fac graful pentru generatia actuala
    G *g = (G *)malloc(sizeof(G));
    if (g == NULL)
    {
        printf("Error at allocatting memory for the graph\n");
        return;
    }
    /// determin generatia actuala din lista stocata in nodul arborelui
    /// la primul apel al acestei functii matricea va fi formata doatr din celule moarte, intrucat in radacina sunt salvate coordonatele celuleor vii din generatia 0
    LIST *head = root->cells;
    while (head != NULL)
    {
        int l = head->l;
        int c = head->c;
        if (l >= 0 && l < N && c >= 0 && c < M)
        {
            if (matrix[l][c].state == '+')
                matrix[l][c].state = 'X'; // actualizez matricea cu celulele vii
            else if (matrix[l][c].state == 'X')
                matrix[l][c].state = '+'; // actualizez matricea cu celulele moarte
        }
        head = head->next;
    }
    DeleteList(&head);
    /// daca lista e goala matricea nu se modifica
    /// aflu numarul de noduri din graf numarand celulele vii din matricea actualizata
    g->V = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j].state == 'X')
            {
                g->V++;
            }
        }
    }
    if (g->V == 0)
    {
        fprintf(f, "-1\n");
        fclose(f);
        return;
    }
    else
    {
        // fac matricea de adiacenta
        g->adiacenta = (int **)malloc(g->V * sizeof(int *));
        if (g->adiacenta == NULL)
        {
            printf("Error at the memory allocation for the adjacency matrix\n");
            deleteGraph(g);
            return;
        }
        for (int i = 0; i < g->V; i++)
        {
            g->adiacenta[i] = (int *)calloc(g->V, sizeof(int));
            if (g->adiacenta[i] == NULL)
            {
                printf("Error at the memory allocation for the adjacency matrix\n");
                for (int j = 0; j < i; j++)
                {
                    free(g->adiacenta[j]);
                }
                free(g->adiacenta);
                deleteGraph(g);
                return;
            }
        }
        /// in vectorul visited o sa retin nodurile grafului
        NOD_GRAF *visited = (NOD_GRAF *)calloc(g->V, sizeof(NOD_GRAF));
        if (visited == NULL)
        {
            printf("Error at the memory allocation for the visited vector\n");
            for (int i = 0; i < N; i++)
            {
                free(g->adiacenta[i]);
            }
            free(g->adiacenta);
            free(g);
            delete_tree(&root);
            for (int i = 0; i < N; i++)
            {
                free(matrix[i]);
            }
            free(matrix);
            return;
        }
        int index = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (matrix[i][j].state == 'X')
                {
                    visited[index].l = i;
                    visited[index].c = j;
                    // visited[index].vizitat = 0;
                    index++;
                }
            }
        }
        for (int i = 0; i < g->V; i++)
        {
            for (int j = 0; j < g->V; j++)
            {
                if (i == j)
                {
                    continue; // Nu există muchie de la un nod la el însuși
                }
                // Verific dacă cele două celule sunt vecine (inclusiv pe diagonală)
                int dx = abs(visited[i].l - visited[j].l);
                int dy = abs(visited[i].c - visited[j].c);
                if (dx <= 1 && dy <= 1)
                {
                    g->adiacenta[i][j] = 1; // Marchez adiacența
                }
            }
        }
        HPath maxPath = {0, NULL};
        NOD_GRAF *current = (NOD_GRAF *)malloc(g->V * sizeof(NOD_GRAF));
        int *vizitate = (int *)calloc(g->V, sizeof(int));

        if (current == NULL || vizitate == NULL)
        {
            printf("Error allocating memory\n");
        }
        else
        {
            for (int i = 0; i < g->V; i++)
            {
                memset(vizitate, 0, g->V * sizeof(int));
                find_path(g, visited, &maxPath, current, vizitate, 0, i);
            }
            if (maxPath.lungime > 0)
            {
                fprintf(f, "%d\n", maxPath.lungime - 1);
                for (int i = 0; i < maxPath.lungime; i++)
                {
                    fprintf(f, "(%d,%d)", maxPath.path[i].l, maxPath.path[i].c);
                    if (i < maxPath.lungime - 1)
                        fprintf(f, " ");
                }
                fprintf(f, "\n");
            }
            else
            {
                fprintf(f, "-1\n");
            }
        }
        if (current)
            free(current);
        if (vizitate)
            free(vizitate);
        if (maxPath.path)
            free(maxPath.path);
        free(visited);
    }
    CEL **matriceA = make_copy(matrix, N, M);
    if (matriceA == NULL)
    {
        printf("Error at the memory allocation for the adjacency matrix\n");
        deleteGraph(g);
        return;
    }
    CEL **matriceB = make_copy(matrix, N, M);
    task4(root->left, matriceB, N, M, argv);
    task4(root->right, matriceA, N, M, argv);
    for (int i = 0; i < N; i++)
    {
        free(matriceA[i]);
        free(matriceB[i]);
    }
    free(matriceA);
    free(matriceB);
*/
//am definit o functie de comparare pentru nodurile globale, pentru a le sorta lexicografic
//folosesc aceasta functie pentru a sorta nodurile inainte de a le folosi in algoritmul Held-Karp
//am definit global pentru ca o voi folosi in mai multe functii
NOD_GRAF *global_nodes;
//functia ce o folosesc ca criteriu de comparare in qsort pentru a-mi sorta nodurile dupa coordonatele lor, astfel incat sa obtin lantul hamiltonian care trebuie
int compare_nods(const void *a, const void *b)
{
    int index_a = *(const int *)a;
    int index_b = *(const int *)b;
    if (global_nodes[index_a].l != global_nodes[index_b].l)
        return global_nodes[index_a].l - global_nodes[index_b].l;
    return global_nodes[index_a].c - global_nodes[index_b].c;
}
//functia ce compara doua cai de lungimi egale pentru a obtine pe cea mai buna atunci cand vreau sa aflu cel mai lung lant hamiltonian
//o folosesc in qsort pentru a sorta lanturile hamiltoniene gasite
int compare_paths(int *a, int *b, int lenght, NOD_GRAF *nodes)
{
    for (int i = 0; i < lenght; ++i)
    {
        if (nodes[a[i]].l != nodes[b[i]].l)
            return nodes[a[i]].l - nodes[b[i]].l;
        if (nodes[a[i]].c != nodes[b[i]].c)
            return nodes[a[i]].c - nodes[b[i]].c;
    }
    return 0;
}
//luand mai multe cazuri pentru aflarea celui mai lung lant hamiltonian, luandu-ma dup anumarul de noduri ale grafului si facand algoritmi diferiti pentru fiecare caz, obtin lanturi ce iau nodurile in ordine diferita, functia imi face ca toate lanturile sa aibe nodurile in aceiasi ordine
void same_path(HPath *cale, NOD_GRAF *nodes)
{
    //in lant_inversat voi salva lantul cu nodurile in ordinea corecta
    //adica inversez ordinea nodurilor
    int *lant_inversat = malloc(cale->lungime * sizeof(int));
    for (int i = 0; i < cale->lungime; ++i)
        lant_inversat[i] = cale->path[cale->lungime - 1 - i];
    //vad daca lantul respecta ordinea care trebuie a nodurilor
    if (compare_paths(lant_inversat, cale->path, cale->lungime, nodes) < 0)
    {
        //inlocuiesc lantul initial cu nodurile in ordinea gresita cu cel cu nodurile in ordinea corecta
        //folosesc memcpy pentru a copia elementele pentru a nu avea erori la copiere
        memcpy(cale->path, lant_inversat, cale->lungime * sizeof(int));
    }
    //eliberez memoria alocata pentru lantul inversat
    free(lant_inversat);
}
//functia ce imi aplica schimbarile asupra matricei, in functie de lista cu coordonatele celulelor ce si-au schimbat starea
void change_state(CEL **matrix, LIST *head)
{
    while (head)
    {
        int l = head->l, c = head->c;
        matrix[l][c].state = (matrix[l][c].state == 'X') ? '+' : 'X';
        head = head->next;
    }
}
// functie ce face un DFS pentru a gasi componentele conexe ale grafului
void componente_conexe(G *g, int nod, int *visited, int *vector, int *nr)
{
    visited[nod] = 1;
    vector[(*nr)++] = nod;
    for (int i = 0; i < g->V; ++i)
        if (g->adiacenta[nod][i] && !visited[i])
            componente_conexe(g, i, visited, vector, nr);
}
// functie ce gaseste componentele conexe ale grafului si le salveaza intr-un vector de componente conexe
void findComponents(G *g, Component **components, int *compNR)
{
    int *visited = calloc(g->V, sizeof(int));
    *components = NULL;
    *compNR = 0;

    for (int i = 0; i < g->V; ++i)
    {
        // daca nodul nu a fost vizitat, inseamna ca face parte dintr-o componenta conexa noua
        if (!visited[i])
        {
            int *vector = malloc(g->V * sizeof(int));
            int nr = 0;
            componente_conexe(g, i, visited, vector, &nr);
            // aloc memorie pentru o noua componenta
            // si o adaug in vectorul de componente
            *components = realloc(*components, (*compNR + 1) * sizeof(Component));
            (*components)[*compNR].nodes = vector;
            (*components)[*compNR].count = nr;
            (*compNR)++;
        }
    }
    free(visited);
}
// functie ce gaseste cel mai lung lant hamiltonian dintr-un graf folosind DFS
//o folosesc pentru a gasi cel mai lung lant hamiltonian in cazul in care graful are un numar mare de noduri, chiar daca este lent, insa imi gaseste exact lantul, peste 25 de noduri
void dfs_longest(G *g, int node, int *visited, NOD_GRAF *nodes, int depth, HPath *best, int *path, int stop_len)
{
    // daca am ajuns la lungimea maxima a lantului,adica numarul de noduri din graf, ies din functie
    if (best->lungime == stop_len)
        return;
    // marchez nodul curent ca vizitat si il adaug in lantul curent
    visited[node] = 1;
    // adaug nodul curent in lantul curent
    path[depth] = node;
// verific daca lungimea lantului curent este mai mare decat lungimea maxima a lantului gasit pana acum
    // sau daca lungimea este egala si lantul curent are nodurile intr-o ordine mai buna dupa coordonate
    if (depth + 1 > best->lungime || (depth + 1 == best->lungime && compare_paths(path, best->path, depth + 1, nodes) < 0))
    {
        // actualizez lungimea maxima a lantului si salvez lantul curent
        best->lungime = depth + 1;
        memcpy(best->path, path, (depth + 1) * sizeof(int));
    }
    // daca am ajuns la lungimea maxima a lantului, ies din functie
    //ma duc la toti vecinii nodului curent
    for (int i = 0; i < g->V; ++i)
        if (!visited[i] && g->adiacenta[node][i])
        {
            dfs_longest(g, i, visited, nodes, depth + 1, best, path, stop_len);
            if (best->lungime == stop_len)
                break;
        }
    //marchez nodul curent ca nevizitat pentru a putea fi folosit in alte lanturi pentru a gasi toate lanturile hamiltoniene posibile
    visited[node] = 0;
}
// functie ce gaseste cel mai lung lant hamiltonian folosind bitmasking si DFS
//o folosesc pentru grafuri cu numar mediu de noduri, 22-25, pentru a reduce timpul de executie
void dfs_bitmask(G *g, int current, int mask, int depth, Component comp,HPath *best, int *path, NOD_GRAF *nodes, int stop_len)
{
    //adaug nodul curent la lantul partial
    path[depth] = comp.nodes[current];
    //actualizez cel mai bun drum gasit
    if (depth + 1 > best->lungime ||
        (depth + 1 == best->lungime && compare_paths(path, best->path, depth + 1, nodes) < 0))
    {
        best->lungime = depth + 1;
        memcpy(best->path, path, (depth + 1) * sizeof(int));
    }
    //daca am adaugat toate nodurile la lantul partial, ies din functie
    if (best->lungime == stop_len)
        return;
    //ca sa nu mai folosesc si un vector de vizitate, folosesc bitmasking pentru a marca nodurile vizitate
    //astfel fac operatiile mai rapide si salvez memorie facand operatii pe biti
    for (int next = 0; next < comp.count; ++next)
        if (!(mask & (1 << next)) && g->adiacenta[comp.nodes[current]][comp.nodes[next]])
            dfs_bitmask(g, next, mask | (1 << next), depth + 1, comp, best, path, nodes, stop_len);
}
//folosesc algoritmul Held-Karp pentru a gasi cel mai lung lant hamiltonian in cazul in care graful are <= 22, este mai rapid decat DFS si reduce timpul de executie daca graful este mic
void held_karp(G *g, NOD_GRAF *nodes, Component comp, HPath *best)
{
    int n = comp.count;
    int **dp = malloc((1 << n) * sizeof(int *));
    int **prev = malloc((1 << n) * sizeof(int *));
    for (int i = 0; i < (1 << n); ++i)
    {
        dp[i] = calloc(n, sizeof(int));
        prev[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j)
            dp[i][j] = -1, prev[i][j] = -1;
    }
    for (int i = 0; i < n; ++i)
        dp[1 << i][i] = 1;

    for (int mask = 1; mask < (1 << n); ++mask)
    {
        for (int u = 0; u < n; ++u)
        {
            if (!(mask & (1 << u)) || dp[mask][u] == -1)
                continue;
            for (int v = 0; v < n; ++v)
            {
                if ((mask & (1 << v)) == 0 &&
                    g->adiacenta[comp.nodes[u]][comp.nodes[v]])
                {
                    int new_mask = mask | (1 << v);
                    if (dp[new_mask][v] < dp[mask][u] + 1)
                    {
                        dp[new_mask][v] = dp[mask][u] + 1;
                        prev[new_mask][v] = u;
                    }
                }
            }
        }
    }

    int max_len = 0, end = -1, mask = 0;
    for (int m = 0; m < (1 << n); ++m)
        for (int i = 0; i < n; ++i)
            if (dp[m][i] > max_len)
            {
                max_len = dp[m][i];
                end = i;
                mask = m;
            }

    best->lungime = max_len;
    best->path = malloc(max_len * sizeof(int));
    int curr = end;
    for (int i = max_len - 1; i >= 0; --i)
    {
        best->path[i] = comp.nodes[curr];
        int prev_node = prev[mask][curr];
        mask ^= (1 << curr);
        curr = prev_node;
    }
    for (int i = 0; i < (1 << n); ++i)
    {
        free(dp[i]);
        free(prev[i]);
    }
    free(dp);
    free(prev);
}

void task4(TREE *root, CEL **matrix, int N, int M, const char *argv[])
{
    if (!root)
        return;
    //actualizez matricea cu celulele vii din generatia 0
    change_state(matrix, root->cells);
    //deschid fisierul de output
    FILE *f = fopen(argv[2], "a");
    if (!f)
        return;
    //fac o matrice de indexare pentru a putea accesa rapid nodurile din graf atunci cand fac matricea de adiacenta
    int **index_matrix = malloc(N * sizeof(int *));
    //in V retin numarul de noduri din graf
    int V = 0;
    for (int i = 0; i < N; ++i)
    {
        index_matrix[i] = malloc(M * sizeof(int));
        for (int j = 0; j < M; ++j)
        {
            if (matrix[i][j].state == 'X')
                index_matrix[i][j] = V++;
            else
                index_matrix[i][j] = -1;
        }
    }
    if (V == 0)
    {
        fprintf(f, "-1\n");
        for (int i = 0; i < N; ++i)
            free(index_matrix[i]);
        free(index_matrix);
        fclose(f);
    }
    //creez graful
    G *g = malloc(sizeof(G));
    if(g==NULL){
        fprintf(f, "Error allocating memory for graph\n");
        for (int i = 0; i < N; ++i)
            free(index_matrix[i]);
        free(index_matrix);
        fclose(f);
        return;
    }
    g->V = V;
    //fac matricea de adiacenta
    g->adiacenta = malloc(V * sizeof(int *));
    for (int i = 0; i < V; ++i)
        g->adiacenta[i] = calloc(V, sizeof(int));
    //fac vectorul de noduri
    NOD_GRAF *nodes = malloc(V * sizeof(NOD_GRAF));
    int index = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (index_matrix[i][j] != -1)
            {
                nodes[index].l = i;
                nodes[index].c = j;
                index++;
            }
    //creez matricea de adiacenta a grafului
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (index_matrix[i][j] != -1)
                for (int d = 0; d < 8; ++d)
                {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < M && index_matrix[ni][nj] != -1)
                        g->adiacenta[index_matrix[i][j]][index_matrix[ni][nj]] = 1;
                }

    // Aflu toate componentele conexe ale grafului
    Component *componente = NULL;
    int compCount = 0;
    findComponents(g, &componente, &compCount);

    for (int i = 0; i < compCount; ++i)
    {
        Component comp = componente[i];
        if (comp.count == 0)
        {
            free(comp.nodes);
            continue;
        }

        // Sortează nodurile componentei 
        global_nodes = nodes;
        qsort(comp.nodes, comp.count, sizeof(int), compare_nods);
        //definesc cel mai bun drum pentru componenta curenta
        HPath best = {0, calloc(V, sizeof(int))};

        for (int j = 0; j < compCount; ++j)
        {
            Component comp = componente[j];
            if (comp.count == 0)
            {
                free(comp.nodes);
                continue;
            }
            //definesc cel mai bun drum din una din componentele conexe continute de graf
            HPath local_best = {0, malloc(comp.count * sizeof(int))};
            //daca graful e mic, are pana in 22 de noduri folosesc algoritmul Held-Karp
            if (comp.count <= 22)
            {
                held_karp(g, nodes, comp, &local_best);
                //verific daca lantul gasit are nodurile in ordinea corecta
                //daca nu are, il inversez pentru a avea nodurile in ordinea corecta
                same_path(&local_best, nodes);
            }
            else if (comp.count <= 25) //daca graful e mediu, are intre 22 si 25 de noduri, folosesc DFS cu bitmasking
            {
                int *temp_path = malloc(comp.count * sizeof(int));
                for (int q = 0; q< comp.count; ++q)
                {
                    dfs_bitmask(g, q, 1 << q, 0, comp, &local_best, temp_path, nodes, comp.count);
                    if (local_best.lungime == comp.count)
                        break;
                }
                free(temp_path);
                //verific daca lantul gasit are nodurile in ordinea corecta
                //daca nu are, il inversez pentru a avea nodurile in ordinea corecta
                same_path(&local_best, nodes);
            }
            else// daca graful e mare, are peste 25 de noduri, folosesc DFS pentru a gasi cel mai lung lant hamiltonian
            {
                int *visited = calloc(V, sizeof(int));
                int *path = malloc(comp.count * sizeof(int));
                for (int q = 0; q < comp.count; ++q)
                {
                    memset(visited, 0, V * sizeof(int));
                    dfs_longest(g, comp.nodes[q], visited, nodes, 0, &local_best, path, comp.count);
                    if (local_best.lungime == comp.count)
                        break;
                }
                free(path);
                free(visited);
                //verific daca lantul gasit are nodurile in ordinea corecta
                //daca nu are, il inversez pentru a avea nodurile in ordinea corecta
                same_path(&local_best, nodes);
            }

            if (local_best.lungime > best.lungime ||
                (local_best.lungime == best.lungime &&
                 compare_paths(local_best.path, best.path, best.lungime, nodes) < 0))
            {
                best.lungime = local_best.lungime;
                memcpy(best.path, local_best.path, best.lungime * sizeof(int));
            }

            free(local_best.path);
            free(comp.nodes);
        }
        //afisez cel mai lung lant hamiltonian gasit pentru graful
        if (best.lungime <= 1)
            fprintf(f, "-1\n");
        else
        {
            fprintf(f, "%d\n", best.lungime - 1);
            for (int i = 0; i < best.lungime; ++i)
            {
                NOD_GRAF n = nodes[best.path[i]];
                fprintf(f, "(%d,%d)%s", n.l, n.c, (i < best.lungime - 1) ? " " : "\n");
            }
        }

        free(best.path);
        free(componente);
        for (int i = 0; i < V; ++i)
            free(g->adiacenta[i]);
        free(g->adiacenta);
        free(g);
        free(nodes);

    cleanup:
        for (int i = 0; i < N; ++i)
            free(index_matrix[i]);
        free(index_matrix);
        fclose(f);

        if (root->left)
            task4(root->left, matrix, N, M, argv);
        if (root->right)
            task4(root->right, matrix, N, M, argv);

        change_state(matrix, root->cells);
    }
}