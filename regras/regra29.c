# include "regra29.h"
# include "../compilador.h"

# include <stdio.h>


void carregaVariavel()
{
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, token);
  if (!attr)
    printErro("Variável %s não definida", token);

  // Coloca tipo na pilha e carrega valor/indireto
  switch (attr->cat)
  {
    case VAR_SIMPLES:
      pushTipo(pilhaTipos, attr->vsAttr.tipo);
      if (topChar(pilhaContextos))
        printComando(NULL, "CREN %d, %d", attr->nivel, attr->vsAttr.desloc);
      else
        printComando(NULL, "CRVL %d, %d", attr->nivel, attr->vsAttr.desloc);
      break;

    case PARAM_FORMAL:
      pushTipo(pilhaTipos, attr->pfAttr.tipo);
      if (topChar(pilhaContextos))
      {
        if (attr->pfAttr.porRef)
          printComando(NULL, "CRVL %d, %d", attr->nivel, attr->pfAttr.desloc);
        else
          printComando(NULL, "CREN %d, %d", attr->nivel, attr->pfAttr.desloc);
      }
      else
      {
        if (attr->pfAttr.porRef)
          printComando(NULL, "CRVI %d, %d", attr->nivel, attr->pfAttr.desloc);
        else
          printComando(NULL, "CRVL %d, %d", attr->nivel, attr->pfAttr.desloc);
      }
      break;

    case FUNCAO:
      push(pilhaIdents, token, TAM_TOKEN);
      printComando(NULL, "AMEM 1");
      break;

    default:
      printErro("Símbolo %s não é variável nem parametro formal", token);
  }

  # ifdef DEBUG
  printf("-- PILHA DE TIPOS --\n");
  imprimePilha(pilhaTipos, sizeof(tipoDado_t));
  # endif
}


void carregaConstante()
{
    pushTipo(pilhaTipos, INTEGER);
    printComando(NULL, "CRCT %s", token);
        
    # ifdef DEBUG
    printf("-- PILHA DE TIPOS --\n");
    imprimePilha(pilhaTipos, sizeof(tipoDado_t));
    # endif
}
