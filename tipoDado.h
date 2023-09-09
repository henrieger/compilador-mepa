# ifndef __TIPO_DADO_H__
# define __TIPO_DADO_H__

# include "tabelaSimbolos.h"

# include "pilha.h"

# define tipoDado_t int 
# define pilhaTipos_t pilha_t *
# define TIPO_NULO -1

// Insere um tipo de dado novo na tabela de simbolos
int insereTipoDado(tabelaSimbolos_t ts, char *ident, unsigned int tam, char nivel);

// Retorna o tipo de dados na pilha
tipoDado_t buscaTipoDado(tabelaSimbolos_t ts, char *ident); 

// Retorna um ponteiro para o tipo de dados na pilha
void *getTipoDadoPointer(tabelaSimbolos_t ts, tipoDado_t td);

// Define o tipo de dados das últimas n entradas da tabela de símbolos
int defineTipoUltimasNEntradas(tabelaSimbolos_t ts, int n, tipoDado_t td);

// Retorna o tamanho da pilha de tipos
int tamPilhaTipos(pilhaTipos_t pt);

// Insere um tipo na pilha de tipos
int pushTipo(pilhaTipos_t pt, tipoDado_t td);

// Retorna o tipo no topo da pilha de tipos
tipoDado_t topTipo(pilhaTipos_t pt);

// Remove um tipo da pilha de tipos
tipoDado_t popTipo(pilhaTipos_t pt);

# endif
