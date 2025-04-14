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
    ///alocarea dinamica pentru matrice
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
            return 1;
        }
    }
    ///deschid fisierul de output in modul write pentru a sterge testele anterioare
    FILE *out;
    if ((out = fopen(argv[2], "w")) == NULL)
    {
        printf("Error at opening the output file\n");
        return 1;
    }
    fclose(out);
    ///creez stiva citita din fisier
    GEN *stacktop = create_stack_matrix(argv,matrix,N,M,K,T);
    make_first_generation(argv,&stacktop,matrix,N,M,K,T);
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