# include "tipoDado.h"

# include <string.h>

// Insere um tipo de dado novo na tabela de simbolos
int insereTipoDado(tabelaSimbolos ts, char *ident, unsigned int tam, char nivel)
{
  struct tipoDadoAttr tdAttr = _tipoDado(tam);
  attrsSimbolo_t *attrs = inicializaAttrsSimbolo(TIPO_DADO, nivel);
  return insereSimbolo(ts, ident, attrs);
}

// Retorna um ponteiro para o tipo de dados na pilha
tipoDado buscaTipoDado(tabelaSimbolos ts, char *ident)
{
  for (int i = tamPilha(ts) - sizeof(simbolo_t); i >= 0; i -= sizeof(simbolo_t))
  {
    simbolo_t *s = (simbolo_t *) (ts->mem + i);
    int cmp = strncmp(ident, s->ident, TAM_TOKEN);
    if (!cmp) return s;
  }

  return NULL;
}
