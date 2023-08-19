# include <stdio.h>
# include <stdlib.h>

# include "pilha.h"

# define TAM 10
# define TYPE long

int main(int argc, char **argv)
{
  pilha_t *p = inicializaPilha();

  if (pilhaVazia(p))
    printf("Pilha vazia");
  else
   printf("Pilha não vazia");

  printf("Tamanho da estrutura: %ld.\n", p->tam);

  TYPE teste[TAM];
  for (int i = 0; i < TAM; i++)
    teste[i] = i;
  
  for (int i = 0; i < TAM; i++)
  {
    pushLong(p, teste[i]);
    printf("Inseriu %ld. Topo da pilha em %ld.\n", teste[i], tamPilha(p));
  }

  printf("Tamanho da estrutura: %ld.\n", p->tam);

  TYPE topo = topLong(p);
  printf("Topo da pilha: %ld\n", topo);

  for (int i = 0; i < TAM; i++)
  {
    topo = popLong(p);
    printf("Saiu da pilha: %ld. Topo da pilha em %ld.\n", topo, tamPilha(p));
  }

  printf("Tamanho da estrutura: %ld.\n", p->tam);
  destroiPilha(p);
}
