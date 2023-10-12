# include "regra30.h"
# include "../compilador.h"


void empilhaAtributosVar()
{
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, token);
  if (!attr)
    printErro("Variável %s não definida", token);

  if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL)
    printErro("Símbolo %s não é variável nem parametro formal", token);

  push(pilhaAttrs, (void *) attr, sizeof(attrsSimbolo_t));
}
