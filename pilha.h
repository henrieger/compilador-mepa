# ifndef __PILHA_H__
# define __PILHA_H__

# include <stdlib.h>

typedef struct pilha
{
  size_t tam;
  size_t topo;
  void *mem;
} pilha_t;

// Cria uma nova pilha
pilha_t *inicializaPilha();

// Retorna o tamanho da pilha
size_t tamPilha(pilha_t *p);

// Checa se a pilha está vazia
char pilhaVazia(pilha_t *p);

// Adiciona elemento a uma pilha
int push(pilha_t * p, void *e, size_t tam);

// Pega o elemento no topo da pilha
void *top(pilha_t *p, size_t tam);

// Retira o elemento do topo da pilha
int pop(pilha_t *p, void *ret, size_t tam);

// Destroi elementos internos da pilha
int destroiPilha(pilha_t *p);

// ---- FUNCOES EXTRAS ---- //

// Tamanho de uma pilha de char
size_t tamPilhaChar(pilha_t *p);

// Tamanho de uma pilha de short
size_t tamPilhaShort(pilha_t *p);

// Tamanho de uma pilha de inteiros
size_t tamPilhaInt(pilha_t *p);

// Tamanho de uma pilha de long
size_t tamPilhaLong(pilha_t *p);

// Tamanho de uma pilha de ponteiros
size_t tamPilhaPointer(pilha_t *p);

// Adiciona um char à pilha
int pushChar(pilha_t *p, char e);

// Adiciona short à pilha
int pushShort(pilha_t *p, short e);

// Adiciona inteiro à pilha
int pushInt(pilha_t *p, int e);

// Adiciona long à pilha
int pushLong(pilha_t *p, long e);

// Adiciona um ponteiro à pilha
int pushPointer(pilha_t *p, void *e);

// Pega o char no topo da pilha
char topChar(pilha_t *p);

// Pega o short no topo da pilha
short topShort(pilha_t *p);

// Pega o inteiro no topo da pilha
int topInt(pilha_t *p);

// Pega o long no topo da pilha
long topLong(pilha_t *p);

// Pega o ponteiro no topo da pilha
void *topPointer(pilha_t *p);

// Remove um char do topo da pilha
char popChar(pilha_t *p);

// Remove um short do topo da pilha
short popShort(pilha_t *p);

// Remove um inteiro do topo da pilha
int popInt(pilha_t *p);

// Remove um long do topo da pilha
long popLong(pilha_t *p);

// Remove um ponteiro do topo da pilha
void *popPointer(pilha_t *p);

# endif
