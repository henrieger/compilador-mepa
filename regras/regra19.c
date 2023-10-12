# include "regra19.h"
# include "../compilador.h"

# include <stdio.h>


void atribuicao()
{
  // Recupera símbolo a ser atribuído
  char variavel[TAM_TOKEN];
  pop(pilhaIdents, variavel, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, variavel);

  // Verifica existência e tipo do símbolo
  if (!attr)
    printErro("Variável %s não definida", variavel);

  if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL)
    printErro("Símbolo %s não é variável nem parâmetro formal", variavel);

  // Coloca símbolo na pilha de atributos
  push(pilhaAttrs, (void *) attr, sizeof(attrsSimbolo_t));

  // Recupera e compara tipo da expressão
  tipoDado_t tipoExpressao = popTipo(pilhaTipos);
  
  # ifdef DEBUG
  printf("Tipo da expressao: %d\nTipo da variavel: %d\n", tipoExpressao, attr->vsAttr.tipo);
  # endif
      
  if (attr->vsAttr.tipo != tipoExpressao)
    yyerror("Tipo da expressão incompatível com a variável");

  // Gera comando para armazenar valor da expressão
  if (attr->cat == VAR_SIMPLES)
    printComando(NULL, "ARMZ %d,%d", attr->nivel, attr->vsAttr.desloc);
  else if (attr->cat == PARAM_FORMAL)
  {
    if (attr->pfAttr.porRef)
      printComando(NULL, "ARMI %d,%d", attr->nivel, attr->pfAttr.desloc);
    else
      printComando(NULL, "ARMZ %d,%d", attr->nivel, attr->pfAttr.desloc);
  } 
}
