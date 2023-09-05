# ifndef __tipoDado_H__
# define __tipoDado_H__

# include "tabelaSimbolos.h"

# include "pilha.h"

# define tipoDado void *
# define pilhaTipos pilha_t *

// Insere um tipo de dado novo na tabela de simbolos
int insereTipoDado(tabelaSimbolos ts, char *ident, unsigned int tam, char nivel);

// Retorna um ponteiro para o tipo de dados na pilha
tipoDado buscaTipoDado(tabelaSimbolos ts, char *ident); 

# endif
