# include "regra12.h"
# include "../compilador.h"


void entraProcedure()
{
  // Incrementa nível léxico
  nivel_lexico++;
 
  // Cria o rótulo de entrada do procedimento
  rotulo_t rotuloProc;
  attrsSimbolo_t *attr = inicializaAttrsSimbolo(PROCEDIMENTO, nivel_lexico);
  proximoRotulo(rotuloProc);
  attr->procAttr = procedimento(rotuloProc, 0, NULL);
  
  // ENPR
  printComando(rotuloProc, "ENPR %d", nivel_lexico);

  // Insere o nome do procedimento na tabela de símbolos
  insereSimbolo(tabelaSimbolos, token, attr);
  
  // Coloca o nome do procedimento na pilha de identificadores
  push(pilhaIdents, token, TAM_TOKEN);
}


void retornaProcedure()
{
  // Retira nome do procedimento da pilha de identificadores
  char *procedimento;
  pop(pilhaIdents, procedimento, TAM_TOKEN);

  // RTPR
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(procedimento);
  printComando(NULL, "RTPR %d,%d", nivel_lexico, attr->procAttr.numParam);
 
  // Decrementa nível léxico
  nivel_lexico--;
}
