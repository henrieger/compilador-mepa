# include "regra9.h"
# include "../compilador.h"

# include <stdio.h>


void amemTipo()
{
  // Define o tipo
  tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
  
  # ifdef DEBUG
  printf("Tipo encontrado para %s: %d\n", token, tipo);
  # endif
        
  defineTipoUltimasNEntradas(tabelaSimbolos, num_vars, tipo);
        
  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  # endif

  // AMEM
  printComando(NULL, "AMEM %d", num_vars);
}


void verificaTipo()
{
  tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
  if (tipo == TIPO_NULO)
    printErro("Tipo %s não definido", token);
}


void insereVarsTabSim()
{
  attrsSimbolo_t *attrs = inicializaAttrsSimbolo(VAR_SIMPLES, nivel_lexico); 
  attrs->vsAttr = varSimples(TIPO_NULO, desloc);
  insereSimbolo(tabelaSimbolos, token, attrs);
  desloc++; num_vars++;
  
  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  # endif
}
