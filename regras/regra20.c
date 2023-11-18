# include "regra20.h"
# include "../compilador.h"


void checaProcedimentoOuFuncao()
{
  char *proc_ou_fun = top(pilhaIdents, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(proc_ou_fun);

  // AMEM para funções
  if (attr->cat == FUNCAO)
    printComando(NULL, "AMEM 1");
      
}


void chamaProcedimento()
{
  char procedimento[TAM_TOKEN];
  pop(pilhaIdents, procedimento, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(procedimento);

  // CHPR
  printComando(NULL, "CHPR %s, %d", attr->procAttr.rotulo, nivel_lexico);

  // DMEM que deleta retorno da função
  if (attr->cat == FUNCAO)
    printComando(NULL, "DMEM 1");
}


void chamaFuncao()
{
  char simbolo[TAM_TOKEN];
  pop(pilhaIdents, simbolo, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, simbolo);

  if (attr->cat != FUNCAO)
    printErro("Apenas funções podem receber parâmetros em expressão");

  // AMEM e CHPR
  printComando(NULL, "CHPR %s, %d", attr->procAttr.rotulo, nivel_lexico);
}
