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


void leituraIdents()
{
  // Recupera atributos do identificador
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, token);
  if (!attr)
    printErro("Símbolo %s não definido", token);

  // Erro se ident não for variável ou parâmetro
  if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL)
    printErro("Símbolo %s não é variável nem parametro formal", token);

  // LEIT
  geraCodigo(NULL, "LEIT");

  // Armazena valor/indireto dependendo dos atributos
  if (attr->cat == VAR_SIMPLES)
  {
    printComando(NULL, "ARMZ %d,%d", attr->nivel, attr->vsAttr.desloc);
  }
  else if (attr->cat == PARAM_FORMAL)
  {
    if (attr->pfAttr.porRef)
      printComando(NULL, "ARMI %d,%d", attr->nivel, attr->pfAttr.desloc);
    else
      printComando(NULL, "ARMZ %d,%d", attr->nivel, attr->pfAttr.desloc);
  }
}
