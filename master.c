#include "stivelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
      // declararea grafului
      G *g = (G *)malloc(sizeof(G));
      if (g == NULL)
      {
        printf("Error at the memory allocation for the graph\n");
        delete_tree(&root);
        for (int i = 0; i < N; i++)
        {
          free(matrix[i]);
        }
        free(matrix);
        fclose(f);
        return 1;
      }
      LIST *head = NULL;
      head = root->cells;
      CEL **matrice = (CEL **)malloc(N * sizeof(CEL *));
      if (matrice == NULL)
      {
        printf("Error at the memory allocation for the adjacency matrix\n");
        delete_tree(&root);
        free(g);
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
          printf("Error at the memory allocation for the adjacency matrix\n");
          for (int j = 0; j < i; j++)
          {
            free(matrice[j]);
          }
          free(matrice);
          free(g);
          delete_tree(&root);
          for (int j = 0; j < N; j++)
          {
            free(matrix[j]);
          }
          free(matrix);
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
      // aici scot lista stocata in nodul radacina pentru a obtine coordonatele celulelor vii din generstia 0
      while (head != NULL)
      {
        int l = head->l;
        int c = head->c;
        if (l >= 0 && l < N && c >= 0 && c < M)
        {
          matrice[l][c].state = 'X';
        }
        head = head->next;
      }
      // aflarea numarului de noduri din graf
      g->V = 0;
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          if (matrice[i][j].state == 'X')
          {

            g->V++;
          }
        }
      }
      g->adiacenta = (int **)calloc(g->V, sizeof(int *)); // Alocă vectorul de pointeri
      if (g->adiacenta == NULL)
      {
        printf("Error at the memory allocation for the adjacency matrix\n");
        delete_tree(&root);
        free(g);
        for (int i = 0; i < N; i++)
        {
          free(matrix[i]);
        }
        free(matrix);
        fclose(f);
        return 1;
      }
      for (int i = 0; i < g->V; i++)
      {
        g->adiacenta[i] = (int *)calloc(g->V, sizeof(int)); // Alocă fiecare rând
        if (g->adiacenta[i] == NULL)
        {
          printf("Error at the memory allocation for row %d\n", i);
          for (int j = 0; j < i; j++)
          {
            free(g->adiacenta[j]);
          }
          free(g->adiacenta);
          delete_tree(&root);
          free(g);
          for (int k = 0; k < N; k++)
          {
            free(matrix[k]);
          }
          free(matrix);
          fclose(f);
          return 1;
        }
      }
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
        for (int k = 0; k < N; k++)
        {
          free(matrix[k]);
        }
        free(matrix);
        fclose(f);
        return 1;
      }
      int index = 0;
      for (int i = 0; i < N; i++)
      {
        for (int j = 0; j < M; j++)
        {
          if (matrice[i][j].state == 'X')
          {
            visited[index].l = i;
            visited[index].c = j;
            visited[index].val = 0;
            visited[index].vizitat = 0;
            index++;
          }
        }
      }
      /// fac matricea de adiacenta pentru graful generat de generatia 0
      for (int i = 0; i < g->V; i++)
      {
        for (int j = 0; j < g->V; j++)
        {
          if (i != j)
          {

            if (visited[j].l == visited[i].l - 1 && visited[j].c == visited[i].c - 1)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l - 1 && visited[j].c == visited[i].c + 1)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l + 1 && visited[j].c == visited[i].c - 1)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l + 1 && visited[j].c == visited[i].c + 1)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l - 1 && visited[j].c == visited[i].c)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l + 1 && visited[j].c == visited[i].c)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l && visited[j].c == visited[i].c - 1)
            {
              g->adiacenta[i][j] = 1;
            }
            else if (visited[j].l == visited[i].l && visited[j].c == visited[i].c + 1)
            {
              g->adiacenta[i][j] = 1;
            }
          }
        }
      }
      /*if (T != 3) aici verific daca mi-a creat corect matricea
      {
        for (int i = 0; i < g->V; i++)
        {
          for (int j = 0; j < g->V; j++)
          {
            printf("%d ", g->adiacenta[i][j]);
          }
          printf("\n");
        }
      }*/
      /// fac o functie tip change_rules tot pe gandirea de preorder unde aplic pe fiecare nod ce am facut pt root dar am grija cu copiile matricilor
      for (int i = 0; i < N; i++)
      {
        free(matrice[i]);
      }
      DeleteList(&head);
      free(matrice);
      free(visited);
      delete_tree(&root);
      deleteGraph(g);
    }
    for (int i = 0; i < N; i++)
    {
      free(matrix[i]);
    }
    free(matrix);
    return 0;
  }
}