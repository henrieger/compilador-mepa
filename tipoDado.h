# ifndef __tipoDado_H__
# define __tipoDado_H__

# include "tabelaSimbolos.h"

# include "pilha.h"

# define tipoDado int 
# define pilhaTipos pilha_t *
# define TIPO_NULO -1

// Insere um tipo de dado novo na tabela de simbolos
int insereTipoDado(tabelaSimbolos ts, char *ident, unsigned int tam, char nivel);

// Retorna o tipo de dados na pilha
tipoDado buscaTipoDado(tabelaSimbolos ts, char *ident); 

// Retorna um ponteiro para o tipo de dados na pilha
void *getTipoDadoPointer(tabelaSimbolos ts, tipoDado td);

// Define o tipo de dados das últimas n entradas da tabela de símbolos
int defineTipoUltimasNEntradas(tabelaSimbolos ts, int n, tipoDado td);

// Retorna o tamanho da pilha de tipos
int tamPilhaTipos(pilhaTipos pt);

// Insere um tipo na pilha de tipos
int pushTipo(pilhaTipos pt, tipoDado td);

// Retorna o tipo no topo da pilha de tipos
tipoDado topTipo(pilhaTipos pt);

// Remove um tipo da pilha de tipos
tipoDado popTipo(pilhaTipos pt);

# endif
