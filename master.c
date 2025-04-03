#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>

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
  GEN *stacktop = NULL;
  rules_stack(matrix, N, M, K, &stacktop, argv,T);
  deleteStack(&stacktop);
  for (int j = 0; j < N; j++)
  {
    free(matrix[j]);
  }
  free(matrix);
  fclose(f);
  return 0;
}