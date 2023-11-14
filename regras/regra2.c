# include "regra2.h"
# include "../compilador.h"

# include <stdio.h>
# include <string.h>


void desviaProcedures()
{
  rotulo_t rotuloBloco;
  proximoRotulo(rotuloBloco);
  printComando(NULL, "DSVS %s", rotuloBloco);
  insereRotulo(pilhaRotulos, rotuloBloco);
}


void entraCodigoBloco()
{
  rotulo_t rotuloBloco;
  popRotulo(pilhaRotulos, rotuloBloco);
  geraCodigo(rotuloBloco, "NADA");

  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  # endif
}


// Condições para parar de iterar na tabela de símbolos para contagem do DMEM
char simbolosDoNivelLexico(simbolo_t *s, tabelaSimbolos_t ts)
{
    return s && ((void *) s > (void *) ts->mem) && (s->attrs->nivel >= nivel_lexico);
}

void encerraBloco()
{
  int dmem_qtd = 0, simbolos_qtd = 0;
  simbolo_t *ultimoSimbolo = (simbolo_t *) top(tabelaSimbolos, sizeof(simbolo_t));
      
  // Itera por todos os símbolos do nível léxico atual na tabela
  while (simbolosDoNivelLexico(ultimoSimbolo, tabelaSimbolos))
  {
    char *ident = top(pilhaIdents, TAM_TOKEN);
    if (ident && !strncmp(ultimoSimbolo->ident, ident, TAM_TOKEN))
      break;

    simbolos_qtd++;

    // Se o símbolo for uma variável simples, aumenta a quantidade para o DMEM
    if ((ultimoSimbolo->attrs->nivel == nivel_lexico) && (ultimoSimbolo->attrs->cat == VAR_SIMPLES))
      dmem_qtd++;

    ultimoSimbolo--;
  }

  // Retira todos os símbolos do nível léxico encontrados na tabela, exceto a própria proc/fun
  retiraSimbolos(tabelaSimbolos, simbolos_qtd);
  
  # ifdef DEBUG
  printTabelaSimbolos(tabelaSimbolos);
  printf("DMEM_QTD: %d, SIMBOLOS_QTD: %d\n", dmem_qtd, simbolos_qtd);
  # endif
  
  // DMEM
  printComando(NULL, "DMEM %d", dmem_qtd);
}
