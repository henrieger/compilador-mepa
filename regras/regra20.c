# include "regra20.h"
# include "../compilador.h"


void chamaProcedimento()
{
  char procedimento[TAM_TOKEN];
  pop(pilhaIdents, procedimento, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(procedimento);

  // AMEM para funções
  if (attr->cat == FUNCAO)
    printComando(NULL, "AMEM 1");
      
  // CHPR
  printComando(NULL, "CHPR %s, %d", attr->procAttr.rotulo, nivel_lexico);

  // DMEM que deleta retorno da função
  if (attr->cat == FUNCAO)
    printComando(NULL, "DMEM 1");
}
