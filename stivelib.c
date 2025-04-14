#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void push(GEN **stacktop, LIST *head)
{
    // creating a newnode for the stack, the list containing the cells that have changed their state
    GEN *NewNode = (GEN *)malloc(sizeof(GEN));
    if (NewNode == NULL){
        printf("Error at allocating memory for the new node of the stack");
        return;
    }
    NewNode->generation=head;
    NewNode->next = *stacktop;
    *stacktop = NewNode;
}
int isEmpty(GEN *stacktop)
{
    if (stacktop == NULL)
        return 1;
    else
        return 0;
}
LIST *pop(GEN **stacktop)
{
    if (*stacktop==NULL)
        return NULL;
    GEN *temp = *stacktop;
    LIST *generation = temp->generation;
    *stacktop = (*stacktop)->next;
    free(temp);
    return generation;
}
void deleteStack(GEN **stacktop)
{
    while (isEmpty(*stacktop) != 1)
    {
        GEN *temp = *stacktop;
        *stacktop = (*stacktop)->next;
        DeleteList(&(temp->generation));
        free(temp);
    }
}
void AddAtBeginning(LIST **head, int l, int c)
{
    LIST *NewNode = (LIST *)malloc(sizeof(LIST));
    if (NewNode == NULL){
        printf("Error at allocating memory for the new node of the list");
        return;
    }
    NewNode->l = l;
    NewNode->c = c;
    NewNode->next = *head;
    *head = NewNode;
}
void AddAtEnd(LIST **head, int l, int c)
{
    LIST *NewNode = (LIST *)malloc(sizeof(LIST));
    if (NewNode == NULL){
        printf("Error at allocating memory for the new node of the list");
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
    while ((*head) != NULL)
    {
        LIST *aux = *head;
        *head = (*head)->next;
        free(aux);
    }
}
void print(CEL **matrix, int N, int M, const char *argv[])
{
  FILE *f;
  if ((f = fopen(argv[2], "a+")) == NULL)
  {
    printf("Error at opening the output file\n");
    return;
  }
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M - 1; j++)
    {
      fprintf(f, "%c", matrix[i][j].state);
    }
    fprintf(f, "%c\n", matrix[i][j].state);
  }
  fprintf(f, "\n");
  fclose(f);
}

void print_list(LIST *head, const char *argv[], int K)
{
    FILE *f;
    if ((f = fopen(argv[2], "a+")) == NULL)
    {
        printf("Error at opening the output file\n");
        return;
    }
    fprintf(f, "%d ", K);
    while (head->next != NULL)
    {
        fprintf(f, "%d %d ", head->l, head->c);
        head = head->next;
    }
    fprintf(f, "%d %d", head->l, head->c);
    fprintf(f, "\n");
    fclose(f);
}
void rules_stack(CEL **matrix, int N, int M, int K,GEN **stacktop, const char *argv[], int T)
{
    if (T == 1)
        print(matrix, N, M, argv);
    int change_whole = 0;
    while (change_whole < K)
    {
        /// Reset neighbors count
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                matrix[i][j].neighbors = 0;
            }
        }
        /// Count neighbors
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (matrix[i][j].state == 'X')
                {
                    if (j - 1 >= 0)
                        matrix[i][j - 1].neighbors++;
                    if (j + 1 < M)
                        matrix[i][j + 1].neighbors++;
                    if (i - 1 >= 0 && j - 1 >= 0)
                        matrix[i - 1][j - 1].neighbors++;
                    if (i - 1 >= 0 && j + 1 < M)
                        matrix[i - 1][j + 1].neighbors++;
                    if (i - 1 >= 0)
                        matrix[i - 1][j].neighbors++;
                    if (i + 1 < N && j - 1 >= 0)
                        matrix[i + 1][j - 1].neighbors++;
                    if (i + 1 < N && j + 1 < M)
                        matrix[i + 1][j + 1].neighbors++;
                    if (i + 1 < N)
                        matrix[i + 1][j].neighbors++;
                }
            }
        }
        int changes = 0;
        /// creating the list representing the new positions of the cels that change
        LIST *head = NULL;
        int ok = 0; /// pt a verifica daca am bagat sau nu primul element in lista
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                if (matrix[i][j].state == 'X')
                {
                    if (matrix[i][j].neighbors < 2)
                    { /// subpopulare
                        if (ok == 0)
                        {
                            AddAtBeginning(&head, i, j);
                            ok = 1;
                        }
                        else
                            AddAtEnd(&head, i, j);
                        matrix[i][j].state = '+';
                        changes++;
                    }
                    else if (matrix[i][j].neighbors > 3)
                    { /// suprapopulare
                        if (ok == 0)
                        {
                            AddAtBeginning(&head, i, j);
                            ok = 1;
                        }
                        else
                            AddAtEnd(&head, i, j);
                        matrix[i][j].state = '+';
                        changes++;
                    }
                }
                else if (matrix[i][j].state == '+')
                {
                    if (matrix[i][j].neighbors == 3)
                    { /// reproducere
                        if (ok == 0)
                        {
                            AddAtBeginning(&head, i, j);
                            ok = 1;
                        }
                        else
                            AddAtEnd(&head, i, j);
                        matrix[i][j].state = 'X';
                        changes++;
                    }
                }
            }
        }
        if (changes != 0)
        {
            change_whole++;
            if (head != NULL)
            {

                push(stacktop, head);
                if (T == 1)
                    print(matrix, N, M, argv);
                else if (T == 2)
                    print_list(head, argv, change_whole);
            }
        }
        else
        {
            break;
        }
        DeleteList(&head);
    }
}
GEN *create_stack_matrix(const char *argv[],CEL **matrix,int N,int M,int K,int T){
    ///redeschid fisierul pentru a citi generatia K dupa ce i-am aflat dimensiunile
    FILE *f;
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        printf("Error at the opening of the input file");
        return NULL;
    }
    fscanf(f, "%d\n", &T);
    fscanf(f, "%d\n", &K);
    ///citesc matricea generata pentru generatia K
    for (int j = 0; j < N; j++)
        for (int q = 0; q < M; q++)
        {
            fscanf(f, " %c", &matrix[j][q].state);
            matrix[j][q].neighbors = 0;
        }
    char linie[256];
    GEN *stacktop = NULL;
    fgets(linie, sizeof(linie), f);
    fgets(linie, sizeof(linie), f);
    while (fgets(linie, sizeof(linie), f))
    {
        LIST *head = NULL;
        int lenght = strlen(linie);
        int pair = 1; // pentru verificarea daca am gasit si linia si coloana pentru element
        int i = 0;
        int l, c;
        for (i = 0; i < lenght && linie[i] != '\n';) {
            if (linie[i] != ' ') { 
                int number = 0;
                int digits = 0;
                while (i < lenght && linie[i] != ' ' && linie[i] != '\n' && digits < 3) {
                    number = number * 10 + (linie[i] - '0'); 
                    i++;
                    digits++;
                }
                if (pair == 1) {
                    l = number; 
                    pair++;
                } else {
                    c = number; 
                    if (head == NULL)
                        AddAtBeginning(&head, l, c);
                    else
                        AddAtEnd(&head, l, c);
                    pair = 1;
                }
            } else {
                i++; 
            }
        }
        push(&stacktop, head);
    }
    return stacktop;
}
void make_first_generation(const char *argv[],GEN **stacktop,CEL **matrix,int N,int M,int K,int T){
    while(*stacktop!=NULL){
        LIST *head=pop(stacktop);
        while(head != NULL){
            int l=head->l;
            int c=head->c;
            if(matrix[l][c].state=='+')
                matrix[l][c].state='X';
            else
                matrix[l][c].state='+';
            head=head->next;
        }
        DeleteList(&head);
    }
}