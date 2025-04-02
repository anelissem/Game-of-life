#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
struct celula
{
  char state;
  int neighbors;
};
typedef struct celula CEL;
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

void rules(CEL **matrix, int N, int M, int K, const char *argv[])
{
  int change_whole = 0;
  print(matrix, N, M, argv);
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
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        if (matrix[i][j].state == 'X')
        {
          if (matrix[i][j].neighbors < 2)
          { /// subpopulare
            matrix[i][j].state = '+';
            changes++;
          }
          else if (matrix[i][j].neighbors > 3)
          { /// suprapopulare
            matrix[i][j].state = '+';
            changes++;
          }
        }
        else if (matrix[i][j].state == '+')
        {
          if (matrix[i][j].neighbors == 3)
          { /// reproducere
            matrix[i][j].state = 'X';
            changes++;
          }
        }
      }
    }
    if (changes != 0)
    {
      change_whole++;
      print(matrix, N, M, argv);
    }
    else
    {
      break;
    }
  }
}
void rules_stack(CEL **matrix, int N, int M, int K,GEN **stacktop, const char *argv[])
{
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
    ///creating the list representing the new positions of the cels that change
    LIST *head=NULL;
    int ok=0;///pt a verifica daca am bagat sau nu primul element in lista
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        if (matrix[i][j].state == 'X')
        {
          if (matrix[i][j].neighbors < 2)
          { /// subpopulare
            if(ok==0){
              AddAtBeginning(&head,i,j);
              ok=1;
            }
            else
              AddAtEnd(&head,i,j);
            matrix[i][j].state = '+';
            changes++;
          }
          else if (matrix[i][j].neighbors > 3)
          { /// suprapopulare
            if(ok==0){
              AddAtBeginning(&head,i,j);
              ok=1;
            }
            else
              AddAtEnd(&head,i,j);
            matrix[i][j].state = '+';
            changes++;
          }
        }
        else if (matrix[i][j].state == '+')
        {
          if (matrix[i][j].neighbors == 3)
          { /// reproducere
            if(ok==0){
              AddAtBeginning(&head,i,j);
              ok=1;
            }
            else
              AddAtEnd(&head,i,j);
            matrix[i][j].state = 'X';
            changes++;
          }
        }
      }
    }
    if (changes != 0)
    {
      change_whole++;
      push(stacktop,head);
      print_list(head,argv,change_whole);
    }
    else
    {
      break;
    }
    DeleteList(&head);
  }
}
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
    return 1;
  }
  for (int j = 0; j < N; j++)
  {
    matrix[j] = (CEL *)malloc(M * sizeof(CEL));
    if (matrix[j] == NULL)
    {
      printf("Error at the momory allocation for the rows of the matrix\n");
      return 1;
    }
  }
  for (int j = 0; j < N; j++)
    for (int q = 0; q < M; q++)
    {
      fscanf(f, " %c", &matrix[j][q].state);
      matrix[j][q].neighbors = 0;
    }
  if (T == 1)
  {
    rules(matrix, N, M, K, argv);
  }
  else if (T == 2)
  {
    GEN *stacktop = NULL;
    rules_stack(matrix,N,M,K,&stacktop,argv);
    deleteStack(&stacktop);
  }
  for (int j = 0; j < N; j++)
  {
    free(matrix[j]);
  }
  free(matrix);
  fclose(f);
  return 0;
}
