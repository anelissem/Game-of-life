#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

int main(int argc, const char *argv[])
{
  // deschiderea fisierului de intrare
  FILE *f;
  if ((f = fopen(argv[1], "r")) == NULL)
  {
    printf("Error at the opening of the input file\n");
    return 1;
  }
  int T, N, M, K;
  // citirea numarului testului
  fscanf(f, "%d\n", &T);
  if (T == 5) /// consider testul 5 bonusul
  {
    /// citesc numarul de generatii
    fscanf(f, "%d\n", &K);
    /// aflu dimensiunile matricei ce contine a K - a generatie
    N = 0;
    M = 0;
    char linie[256];
    while (fgets(linie, sizeof(linie), f) && strcmp(linie, "\n") != 0)
    {
      N++;
      if (M == 0)
        M = strlen(linie) - 1;
    }
    fclose(f);
    /// alocarea dinamica pentru matrice
    CEL **matrix;
    matrix = (CEL **)malloc(N * sizeof(CEL *));
    if (matrix == NULL)
    {
      printf("Error at the momory allocation for the entire matrix\n");
      return 1;
    }
    for (int i = 0; i < N; i++)
    {
      matrix[i] = (CEL *)malloc(M * sizeof(CEL));
      if (matrix[i] == NULL)
      {
        printf("Error at the memory allocation for the rows of the matrix\n");
        for (int k = 0; k < i; k++)
        {
          free(matrix[k]);
        }
        free(matrix);
        matrix = NULL;
        return 1;
      }
    }
    /// deschid fisierul de output in modul write pentru a sterge testele anterioare
    FILE *out;
    if ((out = fopen(argv[2], "w")) == NULL)
    {
      printf("Error at opening the output file\n");
      for (int j = 0; j < N; j++)
      {
        free(matrix[j]);
      }
      free(matrix);
      fclose(f);
      return 1;
    }
    fclose(out);
    /// creez stiva citita din fisier
    GEN *stacktop = create_stack_matrix(argv, matrix, N, M, K, T);
    make_first_generation(argv, &stacktop, matrix, N, M, K, T);
    print(matrix, N, M, argv);
    deleteStack(&stacktop);
    for (int j = 0; j < N; j++)
    {
      free(matrix[j]);
    }
    free(matrix);
    fclose(f);
    return 0;
  }
  else // pt celalalte cazuri
  {
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
    for (int i = 0; i < N; i++)
    {
      matrix[i] = (CEL *)malloc(M * sizeof(CEL));
      if (matrix[i] == NULL)
      {
        printf("Error at the memory allocation for the rows of the matrix\n");
        for (int j = 0; j < i; j++)
        {
          free(matrix[j]);
        }
        free(matrix);
        matrix = NULL;
        return 1;
      }
    }
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
      {
        fscanf(f, " %c", &matrix[i][j].state);
        matrix[i][j].neighbors = 0;
      }
    FILE *out;
    if ((out = fopen(argv[2], "w")) == NULL)
    {
      printf("Error at opening the output file\n");
      for (int j = 0; j < N; j++)
      {
        free(matrix[j]);
      }
      free(matrix);
      fclose(f);
      return 1;
    }
    fclose(out);
    if (T == 1 || T == 2)
    {
      GEN *stacktop = NULL;
      rules_stack(matrix, N, M, K, &stacktop, argv, T);
      deleteStack(&stacktop);
      for (int j = 0; j < N; j++)
      {
        free(matrix[j]);
      }
      free(matrix);
      return 0;
    }
    else
    {
      out = fopen(argv[2], "w");
      if (out == NULL)
      {
        printf("Error opening output file\n");
        return 1;
      }
      fclose(out);
      TREE *root = NULL; // initializez arborele
      LIST *gen0 = NULL; // initializez lista in care stochez prima generatie
      /// creez lista ce contine coordonatele celulelor vii din prima generatie
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          if (matrix[i][j].state == 'X')
          {
            AddAtEnd(&gen0, i, j);
          }
        }
      }
      root = create_initial_tree(matrix, gen0, N, M); /// creez radacina arborelui
      if (root == NULL)
      {
        printf("Error at the memory allocation for the root of the tree\n");
        for (int i = 0; i < N; i++)
        {
          free(matrix[i]);
        }
        free(matrix);
        fclose(f);
        return 1;
      }
      if (T == 3)
        print(matrix, N, M, argv);
      change_rules(&root, matrix, argv, N, M, K, T);
      CEL **matrice = (CEL **)malloc(N * sizeof(CEL *));
      if (matrice == NULL)
      {
        printf("Error at the memory allocation for the matrix\n");
        delete_tree(&root);
        for (int i = 0; i < N; i++)
        {
          free(matrix[i]);
        }
        free(matrix);
        fclose(f);
        return 1;
      }
      for (int i = 0; i < N; i++)
      {
        matrice[i] = (CEL *)malloc(M * sizeof(CEL));
        if (matrice[i] == NULL)
        {
          printf("Error at the memory allocation for the rows of the matrix\n");
          delete_tree(&root);
          for (int j = 0; j < i; j++)
          {
            free(matrice[j]);
          }
          free(matrice);
          fclose(f);
          return 1;
        }
      }
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          matrice[i][j].state = '+';
        }
      }
      if( T == 4)
        task4(root,matrice,N,M,argv);
      delete_tree(&root);
      for (int i = 0; i < N; i++)
      {
        free(matrix[i]);
        free(matrice[i]);
      }
      free(matrix);
      free(matrice);
      return 0;
    }
  }
}