# include "pilha.h"

# include <stdlib.h>
# include <string.h>

# define MIN_TAM_PILHA 32

// Cria uma nova pilha
pilha_t *inicializaPilha()
{
  pilha_t *p = malloc(sizeof(pilha_t));
  p->tam = MIN_TAM_PILHA;
  p->topo = 0;
  p->mem = malloc(MIN_TAM_PILHA);

  return p;
}

// Retorna o tamanho da pilha
size_t tamPilha(pilha_t *p)
{
  return p->topo;
}

// Checa se a pilha está vazia
char pilhaVazia(pilha_t *p)
{
  return (p->tam == 0) || (p->topo == 0);
}

// Adiciona elemento a uma pilha
int push(pilha_t * p, void *e, size_t tam)
{
  if (p->topo + tam > p->tam)
  {
    p->tam = (p->tam + tam) * 2;
    p->mem = realloc(p->mem, p->tam);

    if (!p->mem) return 1;
  }

  memcpy(p->mem + p->topo, e, tam);
  
  p->topo += tam;

  return 0;
}

// Pega o elemento no topo da pilha
void *top(pilha_t *p, size_t tam)
{
  if (tam <= 0 || tam > p->topo) return NULL;
  return p->mem + p->topo - tam;
}

// Retira o elemento do topo da pilha
int pop(pilha_t *p, void *ret, size_t tam)
{
  if (tam <= 0) return 1;
  if (!ret) return 2;

  void *t = top(p, tam);
  if (!t) return 3;
  
  memcpy(ret, t, tam);
  p->topo -= tam;

  size_t novoTam = p->tam / 2;
  novoTam = (novoTam < MIN_TAM_PILHA) ? MIN_TAM_PILHA : novoTam;

  if (p->topo < novoTam)
  {
    p->tam = novoTam;
    p->mem = realloc(p->mem, novoTam);
  }

  return 0;
}

// Destroi elementos internos da pilha
int destroiPilha(pilha_t *p)
{
  if (!p) return 1;
  if (p->mem) free(p->mem);
  free(p);
  return 0;
}

// ---- FUNCOES EXTRAS ---- //

// Tamanho de uma pilha de char
size_t tamPilhaChar(pilha_t *p)
{
  return tamPilha(p) / sizeof(char);
}

// Tamanho de uma pilha de short
size_t tamPilhaShort(pilha_t *p)
{
  return tamPilha(p) / sizeof(short);
}

// Tamanho de uma pilha de inteiros
size_t tamPilhaInt(pilha_t *p)
{
  return tamPilha(p) / sizeof(int);
}

// Tamanho de uma pilha de long
size_t tamPilhaLong(pilha_t *p)
{
  return tamPilha(p) / sizeof(long);
}

// Adiciona um char à pilha
int pushChar(pilha_t *p, char e)
{
  return push(p, &e, sizeof(char));
}

// Adiciona short à pilha
int pushShort(pilha_t *p, short e)
{
  return push(p, &e, sizeof(short));
}

// Adiciona inteiro à pilha
int pushInt(pilha_t *p, int e)
{
  return push(p, &e, sizeof(int));
}

// Adiciona long à pilha
int pushLong(pilha_t *p, long e)
{
  return push(p, &e, sizeof(long));
}

// Pega o char no topo da pilha
char topChar(pilha_t *p)
{
  return *((char *) top(p, sizeof(char)));
}

// Pega o short no topo da pilha
short topShort(pilha_t *p)
{
  return *((short *) top(p, sizeof(short)));
}

// Pega o inteiro no topo da pilha
int topInt(pilha_t *p)
{
  return *((int *) top(p, sizeof(int)));
}

// Pega o long no topo da pilha
long topLong(pilha_t *p)
{
  return *((long *) top(p, sizeof(long)));
}

// Remove um char do topo da pilha
char popChar(pilha_t *p)
{
  char ret;
  if (pop(p, &ret, sizeof(char))) return 0;
  return ret;
}

// Remove um short do topo da pilha
short popShort(pilha_t *p)
{
  short ret;
  if (pop(p, &ret, sizeof(short))) return 0;
  return ret;
}

// Remove um inteiro do topo da pilha
int popInt(pilha_t *p)
{
  int ret;
  if (pop(p, &ret, sizeof(int))) return 0;
  return ret;
}

// Remove um long do topo da pilha
long popLong(pilha_t *p)
{
  long ret;
  if (pop(p, &ret, sizeof(long))) return 0;
  return ret;
}
