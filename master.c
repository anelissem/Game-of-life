#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, const char *argv[])
{
  FILE *f;
  if ((f = fopen(argv[1], "r")) == NULL)
  {
    printf("Error at the opening of the input file");
    return 1;
  }
  int T, N, M, K;
  fscanf(f, "%d\n", &T);
  if (T == 5)
  {
    fscanf(f, "%d\n", &K);
    int N = 0;
    int M = 0;
    char linie[256];
    while (fgets(linie, sizeof(linie), f) && strcmp(linie, "\n") != 0)
    {
        N++;
        if (M == 0)
            M = strlen(linie) - 1;
    }
    fclose(f);
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        printf("Error at the opening of the input file");
        return 1;
    }
    fscanf(f, "%d\n", &T);
    fscanf(f, "%d\n", &K);
    CEL **matrix;
    matrix = (CEL **)malloc(N * sizeof(CEL *));
    if (matrix == NULL)
    {
        printf("Error at the momory allocation for the entire matrix\n");
    }
    for (int j = 0; j < N; j++)
    {
        matrix[j] = (CEL *)malloc(M * sizeof(CEL));
        if (matrix[j] == NULL)
        {
            printf("Error at the memory allocation for the rows of the matrix\n");
            for (int k = 0; k < j; k++)
            {
                free(matrix[k]);
            }
            free(matrix);
            fclose(f);
            return 1;
        }
    }
    for (int j = 0; j < N; j++)
        for (int q = 0; q < M; q++)
        {
            fscanf(f, " %c", &matrix[j][q].state);
            matrix[j][q].neighbors = 0;
        }
    FILE *out;
    if ((out = fopen(argv[2], "w")) == NULL)
    {
        printf("Error at opening the output file\n");
        return 1;
    }
    fclose(out);
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
                    number = number * 10 + (linie[i] - '0'); // Construiește numărul
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
    while(stacktop!=NULL){
        LIST *head=pop(&stacktop);
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
    print(matrix,N,M,argv);
    deleteStack(&stacktop);
    for (int j = 0; j < N; j++)
    {
        free(matrix[j]);
    }
    free(matrix);
    fclose(f);
    return 0;
  }
  else
  {
    fscanf(f, "%d %d\n", &N, &M);
    fscanf(f, "%d\n", &K);
    /// allocating memory for the matrix that represents the game table
    CEL **matrix;
    matrix = (CEL **)malloc(N * sizeof(CEL *));
    if (matrix == NULL)
    {
      printf("Error at the momory allocation for the entire matrix\n");
    }
    for (int j = 0; j < N; j++)
    {
      matrix[j] = (CEL *)malloc(M * sizeof(CEL));
      if (matrix[j] == NULL)
      {
        printf("Error at the memory allocation for the rows of the matrix\n");
        return 1;
      }
    }
    for (int j = 0; j < N; j++)
      for (int q = 0; q < M; q++)
      {
        fscanf(f, " %c", &matrix[j][q].state);
        matrix[j][q].neighbors = 0;
      }
    FILE *out;
    if ((out = fopen(argv[2], "w")) == NULL)
    {
      printf("Error at opening the output file\n");
      return 1;
    }
    fclose(out);
    GEN *stacktop = NULL;
    rules_stack(matrix, N, M, K, &stacktop, argv, T);
    deleteStack(&stacktop);
    for (int j = 0; j < N; j++)
    {
      free(matrix[j]);
    }
    free(matrix);
    fclose(f);
  }
  return 0;
}