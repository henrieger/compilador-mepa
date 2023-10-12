# include "regra10.h"
# include "../compilador.h"


void insereParamsValorTabSim()
{
  attrsSimbolo_t *attrs = inicializaAttrsSimbolo(PARAM_FORMAL, nivel_lexico); 
  attrs->pfAttr = paramFormal(TIPO_NULO, -1, 0);
  insereSimbolo(tabelaSimbolos, token, attrs);
  qtd_param++;
        
  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  # endif
}
