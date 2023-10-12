# include "regra14.h"
# include "../compilador.h"


void insereListaParam()
{
  // Conta quantidade de parâmetros inseridos antes da função
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(top(pilhaIdents, TAM_TOKEN));
  int qtdParamTotal = 0;
  simbolo_t *s, *topo = top(tabelaSimbolos, sizeof(simbolo_t));
  for (s = topo; s->attrs != attr; s--)
    qtdParamTotal++;

  // Atribui quantidade calculada à função
  if (attr->cat == PROCEDIMENTO)
    attr->procAttr.numParam = qtdParamTotal;
  else if (attr->cat == FUNCAO)
    attr->funAttr.numParam = qtdParamTotal;

  // Coloca dados dos parâmetros na lista de parâmetros
  listaParam_t *listaParam = inicializaListaParam(qtdParamTotal);
  int i = 0;
  while (++s <= topo)
  {
    s->attrs->pfAttr.desloc = -(4+i);
    listaParam_t p = param(s->attrs->pfAttr.tipo, s->attrs->pfAttr.porRef);
    listaParam[i] = p;
    i++;
  }
  
  // Atribui lista de parâmetros à função
  if (attr->cat == PROCEDIMENTO)
    attr->procAttr.parametros = listaParam;
  else if (attr->cat == FUNCAO)
    attr->funAttr.parametros = listaParam;
  
  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  # endif
}
