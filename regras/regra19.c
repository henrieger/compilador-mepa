# include "regra19.h"
# include "../compilador.h"

# include <stdio.h>


void atribuicao()
{
  // Recupera símbolo a ser atribuído
  char simbolo[TAM_TOKEN];
  pop(pilhaIdents, simbolo, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, simbolo);

  // Verifica existência e tipo do símbolo
  if (!attr)
    printErro("Símbolo %s não definido", simbolo);

  if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL && attr->cat != FUNCAO)
    printErro("Símbolo %s não é atribuível", simbolo);

  // Recupera e compara tipo da expressão
  tipoDado_t tipoExpressao = popTipo(pilhaTipos);
  tipoDado_t tipoSimbolo;
  switch (attr->cat)
  {
    case VAR_SIMPLES:
      tipoSimbolo = attr->vsAttr.tipo;
      break;
    case PARAM_FORMAL:
      tipoSimbolo = attr->pfAttr.tipo;
      break;
    case FUNCAO:
      tipoSimbolo = attr->funAttr.tipoRetorno;
      break;
    default:
      break;
  }

  # ifdef DEBUG
    printf("Tipo da expressao: %d\nTipo da simbolo: %d\n", tipoExpressao, tipoSimbolo);
  # endif
      
  
  if (tipoSimbolo != tipoExpressao)
    yyerror("Tipo da expressão incompatível com o símbolo");

  // Gera comando para armazenar valor da expressão
  if (attr->cat == VAR_SIMPLES)
    printComando(NULL, "ARMZ %d, %d", attr->nivel, attr->vsAttr.desloc);
  else if (attr->cat == FUNCAO)
    printComando(NULL, "ARMZ %d, %d", attr->nivel, attr->funAttr.desloc);
  else if (attr->cat == PARAM_FORMAL)
  {
    if (attr->pfAttr.porRef)
      printComando(NULL, "ARMI %d, %d", attr->nivel, attr->pfAttr.desloc);
    else
      printComando(NULL, "ARMZ %d, %d", attr->nivel, attr->pfAttr.desloc);
  } 
}
