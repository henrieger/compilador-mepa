# include "regra20.h"
# include "../compilador.h"


void chamaProcedimento()
{
  char procedimento[TAM_TOKEN];
  pop(pilhaIdents, procedimento, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(procedimento);
      
  // CHPR
  printComando(NULL, "CHPR %s, %d", attr->procAttr.rotulo, nivel_lexico);
}
