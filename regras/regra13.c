# include "regra12.h"
# include "../compilador.h"


void entraFuncao()
{
  // Incrementa nível léxico
  nivel_lexico++;
 
  // Cria o rótulo de entrada da função
  rotulo_t rotuloFun;
  attrsSimbolo_t *attr = inicializaAttrsSimbolo(FUNCAO, nivel_lexico);
  proximoRotulo(rotuloFun);
  attr->funAttr = funcao(TIPO_NULO, desloc, rotuloFun, 0, NULL);
  
  // ENPR
  printComando(rotuloFun, "ENPR %d", nivel_lexico);

  // Insere o nome da função na tabela de símbolos
  insereSimbolo(tabelaSimbolos, token, attr);
  
  // Coloca o nome da função na pilha de identificadores
  push(pilhaIdents, token, TAM_TOKEN);
}


void ajustaFuncao()
{
  char *funcao = top(pilhaIdents, TAM_TOKEN);
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(funcao);

  tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
  attr->funAttr.tipoRetorno = tipo;

  attr->funAttr.desloc = - 4 - attr->funAttr.numParam;
}


void retornaFuncao()
{
  // Retira nome da função da pilha de identificadores
  char funcao[TAM_TOKEN];
  pop(pilhaIdents, funcao, TAM_TOKEN);

  // RTPR
  attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(funcao);
  printComando(NULL, "RTPR %d, %d", nivel_lexico, attr->funAttr.numParam);
 
  // Decrementa nível léxico
  nivel_lexico--;
}
