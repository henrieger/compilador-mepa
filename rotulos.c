# include "rotulos.h"

#include <stdio.h>
# include <string.h>

int rotuloAtual = 0;

// Gera o próximo rótulo e guarda no endereço especificado
void proximoRotulo(rotulo_t r)
{
  # ifdef DEBUG
  printf("ÚLTIMO RÓTULO GERADO: %d\n", rotuloAtual);
  # endif
  sprintf(r, "R%02d", rotuloAtual);
  rotuloAtual = (rotuloAtual + 1) % 100;
}

// Retorna o tamanho da pilha de rótulos
inline int tamPilhaRotulos(pilhaRotulos_t pr)
{
  return tamPilha(pr) / TAM_ROTULO;
}

// Empilha um rótulo
inline int insereRotulo(pilhaRotulos_t pr, rotulo_t r)
{
  return push(pr, r, TAM_ROTULO);
}

// Retorna o topo da pilha de rótulos
int topRotulo(pilhaRotulos_t pr, rotulo_t r)
{
  char *res = top(pr, TAM_ROTULO);
  return sprintf(r, "%s", res);
}

// Desempilha um rótulo
inline int popRotulo(pilhaRotulos_t pr, rotulo_t r)
{
  return pop(pr, (void *) r, TAM_ROTULO);
}

// Empilha dois rótulos
int insereDoisRotulos(pilhaRotulos_t pr, rotulo_t rotPre, rotulo_t rotPos)
{
  int r0 = insereRotulo(pr, rotPre);
  int r1 = insereRotulo(pr, rotPos);
  return r0 + r1;
}

// Desempilha dois rótulos
int removeDoisRotulos(pilhaRotulos_t pr, rotulo_t rotPre, rotulo_t rotPos)
{
  int r1 = popRotulo(pr, rotPos);
  int r0 = popRotulo(pr, rotPre);
  return r0 + r1;
}
