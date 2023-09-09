# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "compilador.h"
# include "pilha.h"
# include "tabelaSimbolos.h"
# include "tipoDado.h"

int main()
{
  tabelaSimbolos_t ts = inicializaPilha();

  printTabelaSimbolos(ts);

  insereTipoDado(ts, "integer", 4, 0);
  insereTipoDado(ts, "real", 4, 0);
  insereTipoDado(ts, "boolean", 1, 0);

  tipoDado_t INTEGER = buscaTipoDado(ts, "integer");
  tipoDado_t REAL = buscaTipoDado(ts, "real");
  tipoDado_t BOOLEAN = buscaTipoDado(ts, "boolean");

  printTabelaSimbolos(ts);
  printf("\nTipo INTEGER: %d\n", INTEGER);
  printf("Tipo REAL: %d\n", REAL);
  printf("Tipo BOOLEAN: %d\n", BOOLEAN);

  // var teste: integer;
  attrsSimbolo_t *teste_as = inicializaAttrsSimbolo(VAR_SIMPLES, 0);
  teste_as->vsAttr = varSimples(INTEGER, 0);
  insereSimbolo(ts, "teste", teste_as);

  // procedure proc(var valor1, valor2: real);
  // var aux: char;
  attrsSimbolo_t *proc_as = inicializaAttrsSimbolo(PROCEDIMENTO, 0);
  listaParam_t *l = inicializaListaParam(2);  
  l[0] = param(REAL, 0);
  l[1] = param(REAL, 0);
  proc_as->procAttr = procedimento("ABCDEF", 2, l);
  insereSimbolo(ts, "proc", proc_as);

  attrsSimbolo_t *valor1_as = inicializaAttrsSimbolo(PARAM_FORMAL, 1);
  valor1_as->pfAttr = paramFormal(REAL, 0, 0);
  insereSimbolo(ts, "valor1", valor1_as);

  attrsSimbolo_t *valor2_as = inicializaAttrsSimbolo(PARAM_FORMAL, 1);
  valor2_as->pfAttr = paramFormal(REAL, 1, 0);
  insereSimbolo(ts, "valor2", valor2_as);

  printf("\n");
  printTabelaSimbolos(ts);

  attrsSimbolo_t *resultadoBusca = buscaSimbolo(ts, "proc");
  printf("\nSímbolo encontrado:\n");
  printSimbolo("proc", resultadoBusca);

  resultadoBusca = buscaSimbolo(ts, "noExiste");
  if (resultadoBusca)
  {
    printf("\nSímbolo encontrado:\n");
    printSimbolo("noExiste", resultadoBusca);
  }
  else
   printf("\nSímbolo 'noExiste' não encontrado\n");

  printf("\n");
  printTabelaSimbolos(ts);

  // Remove proc da tabela
  retiraSimbolos(ts, 3);

  printf("\n");
  printTabelaSimbolos(ts);

  // Remove var da tabela
  retiraSimbolos(ts, 1);
  
  printf("\n");
  printTabelaSimbolos(ts);

  // Remove tipos basicos de dados da tabela
  retiraSimbolos(ts, 3);

  printf("\n");
  printTabelaSimbolos(ts);

  destroiPilha(ts);
}
