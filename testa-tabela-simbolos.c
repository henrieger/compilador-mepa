# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "compilador.h"
# include "pilha.h"
# include "tabelaSimbolos.h"
# include "tipoDado.h"

void printSimbolo(char *ident, attrsSimbolo_t* as)
{
  printf("Símbolo: %s\n{\n\tCategoria: %d\n\tNível: %d\n", ident, as->cat, as->nivel);
  switch (as->cat)
  {
    case VAR_SIMPLES:
      printf("\tDeslocamento: %d\n\tTipo: %p\n", as->vsAttr.desloc, as->vsAttr.tipo);
      break;
    case PARAM_FORMAL:
      printf("\tDeslocamento: %d\n\tTipo: %p\n\tPor referência: %d\n", as->pfAttr.desloc, as->pfAttr.tipo, as->pfAttr.porRef);
      break;
    case PROCEDIMENTO:
      printf("\tRótulo: %s\n\tNúmero de parâmetros: %d\n\tParâmetros:\n\t{\n", as->procAttr.rotulo, as->procAttr.numParam);
      for (int i = 0; i < as->procAttr.numParam; i++)
        printf("\t\tTipo: %p\n\t\tPor referência: %d\n", as->procAttr.parametros[i].tipo, as->procAttr.parametros[i].porRef);
      printf("\t}\n");
      break;
    case FUNCAO:
      printf("\tTipo do retorno: %p\n\tRótulo: %s\n\tNúmero de parâmetros: %d\n\tParâmetros:\n\t{\n", as->funAttr.tipoRetorno, as->funAttr.rotulo, as->funAttr.numParam);
      for (int i = 0; i < as->funAttr.numParam; i++)
        printf("\t\tTipo: %p\n\t\tPor referência: %d\n", as->funAttr.parametros[i].tipo, as->funAttr.parametros[i].porRef);
      printf("\t}\n");
      break;
    case ROTULO:
      printf("\tLinha de código: %d\n", as->rotAttr.linhaCodigo);
      break;
    case TIPO_DADO:
      printf("\tTamanho: %d\n", as->tdAttr.tam);
      break;
  }
  printf("}\n");
}

void printTabelaSimbolos(tabelaSimbolos ts)
{
  printf(" -- TABELA DE SIMBOLOS --\n");
  for (int i = tamPilha(ts) - sizeof(simbolo_t); i >= 0; i -= sizeof(simbolo_t))
  {
    simbolo_t *s =(simbolo_t *) (ts->mem + i);
    printSimbolo(s->ident, s->attrs);
  }
}

int main()
{
  tabelaSimbolos ts = inicializaPilha();

  printTabelaSimbolos(ts);

  insereTipoDado(ts, "integer", 4, 0);
  insereTipoDado(ts, "real", 4, 0);
  insereTipoDado(ts, "boolean", 1, 0);

  tipoDado INTEGER = buscaTipoDado(ts, "integer");
  tipoDado REAL = buscaTipoDado(ts, "real");
  tipoDado BOOLEAN = buscaTipoDado(ts, "boolean");

  printTabelaSimbolos(ts);
  printf("\nTipo INTEGER: %p\n", INTEGER);
  printf("Tipo REAL: %p\n", REAL);
  printf("Tipo BOOLEAN: %p\n", BOOLEAN);

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
