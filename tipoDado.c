# include "tipoDado.h"
#include "pilha.h"

# include <string.h>

// Insere um tipo de dado novo na tabela de simbolos
int insereTipoDado(tabelaSimbolos_t ts, char *ident, unsigned int tam, char nivel)
{
  struct tipoDadoAttr tdAttr = tipoDado(tam);
  attrsSimbolo_t *attrs = inicializaAttrsSimbolo(TIPO_DADO, nivel);
  attrs->tdAttr = tdAttr;
  return insereSimbolo(ts, ident, attrs);
}

// Retorna o tipo de dados na pilha
tipoDado_t buscaTipoDado(tabelaSimbolos_t ts, char *ident)
{
  for (int i = tamPilha(ts) - sizeof(simbolo_t); i >= 0; i -= sizeof(simbolo_t))
  {
    simbolo_t *s = (simbolo_t *) (ts->mem + i);
    int cmp = strncmp(ident, s->ident, TAM_TOKEN);
    if (!cmp) return i;
  }

  return TIPO_NULO;
}

// Retorna um ponteiro para o tipo de dados na pilha
void *getTipoDadoPointer(tabelaSimbolos_t ts, tipoDado_t td)
{
  return ts->mem+td;
}

// Define o tipo de dados das últimas n entradas da tabela de símbolos
int defineTipoUltimasNEntradas(tabelaSimbolos_t ts, int n, tipoDado_t td)
{
  for (int i = 0; i < n; i++)
  {
    simbolo_t *s = top(ts, sizeof(simbolo_t)) - (i*sizeof(simbolo_t));
    if (!s) return 1;

    s->attrs->vsAttr.tipo = td;
  } 

  return 0;
}

// Retorna o tamanho da pilha de tipos
inline int tamPilhaTipos(pilhaTipos_t pt)
{
  return tamPilhaInt(pt);
}

// Insere um tipo na pilha de tipos
inline int pushTipo(pilhaTipos_t pt, tipoDado_t td)
{
  return pushInt(pt, td);
}

// Retorna o tipo no topo da pilha de tipos
inline tipoDado_t topTipo(pilhaTipos_t pt)
{
  return topInt(pt);
}

// Remove um tipo da pilha de tipos
inline tipoDado_t popTipo(pilhaTipos_t pt)
{
  return popInt(pt);
}
