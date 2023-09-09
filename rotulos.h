# ifndef __ROTULOS_H__
# define __ROTULOS_H__

# include "pilha.h"

# define pilhaRotulos_t pilha_t *
# define TAM_ROTULO 4
typedef char rotulo_t[TAM_ROTULO];

extern int rotuloAtual;

// Gera o próximo rótulo e guarda no endereço especificado
void proximoRotulo(rotulo_t r);

// Retorna o tamanho da pilha de rótulos
int tamPilhaRotulos(pilhaRotulos_t pr);

// Empilha um rótulo
int insereRotulo(pilhaRotulos_t pr, rotulo_t r);

// Retorna o topo da pilha de rótulos
int topRotulo(pilhaRotulos_t pr, rotulo_t r);

// Desempilha um rótulo
int popRotulo(pilhaRotulos_t pr, rotulo_t r);

// Empilha dois rótulos
int insereDoisRotulos(pilhaRotulos_t pr, rotulo_t rotPre, rotulo_t rotPos);

// Desempilha dois rótulos
int removeDoisRotulos(pilhaRotulos_t pr, rotulo_t rotPre, rotulo_t rotPos);

# endif
