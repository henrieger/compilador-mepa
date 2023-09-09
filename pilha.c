# include "pilha.h"

# include <stdio.h>
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
inline size_t tamPilha(pilha_t *p)
{
  return p->topo;
}

// Checa se a pilha está vazia
inline char pilhaVazia(pilha_t *p)
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

  void *t = top(p, tam);
  if (!t) return 2;
  
  if (ret)
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

// Imprime o conteudo da pilha byte a byte
void imprimePilha(pilha_t *p, int tam)
{
  for (int i = tamPilha(p) - tam; i >= 0; i -= tam)
  {
    printf("%d: ", i);
    for (int j = 0; j < tam; j++)
      printf("%x ", ((unsigned char *)(p->mem))[i+j]);
    printf("\n");
  }
}

// ---- FUNCOES EXTRAS ---- //

// Tamanho de uma pilha de char
inline size_t tamPilhaChar(pilha_t *p)
{
  return tamPilha(p) / sizeof(char);
}

// Tamanho de uma pilha de short
inline size_t tamPilhaShort(pilha_t *p)
{
  return tamPilha(p) / sizeof(short);
}

// Tamanho de uma pilha de inteiros
inline size_t tamPilhaInt(pilha_t *p)
{
  return tamPilha(p) / sizeof(int);
}

// Tamanho de uma pilha de long
inline size_t tamPilhaLong(pilha_t *p)
{
  return tamPilha(p) / sizeof(long);
}

// Tamanho de uma pilha de ponteiros
inline size_t tamPilhaPointer(pilha_t *p)
{
  return tamPilha(p) / sizeof(void *);
}

// Adiciona um char à pilha
inline int pushChar(pilha_t *p, char e)
{
  return push(p, &e, sizeof(char));
}

// Adiciona short à pilha
inline int pushShort(pilha_t *p, short e)
{
  return push(p, &e, sizeof(short));
}

// Adiciona inteiro à pilha
inline int pushInt(pilha_t *p, int e)
{
  return push(p, &e, sizeof(int));
}

// Adiciona long à pilha
inline int pushLong(pilha_t *p, long e)
{
  return push(p, &e, sizeof(long));
}

// Adiciona um ponteiro à pilha
inline int pushPointer(pilha_t *p, void *e)
{
  return push(p, &e, sizeof(void *));
}

// Pega o char no topo da pilha
inline char topChar(pilha_t *p)
{
  return *((char *) top(p, sizeof(char)));
}

// Pega o short no topo da pilha
inline short topShort(pilha_t *p)
{
  return *((short *) top(p, sizeof(short)));
}

// Pega o inteiro no topo da pilha
inline int topInt(pilha_t *p)
{
  return *((int *) top(p, sizeof(int)));
}

// Pega o long no topo da pilha
inline long topLong(pilha_t *p)
{
  return *((long *) top(p, sizeof(long)));
}

// Pega o ponteiro no topo da pilha
inline void *topPointer(pilha_t *p)
{
  return *((void **) top(p, sizeof(void *)));
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

// Remove um ponteiro do topo da pilha
void *popPointer(pilha_t *p)
{
  void *ret;
  if (pop(p, &ret, sizeof(void *))) return 0;
  return ret;
}
