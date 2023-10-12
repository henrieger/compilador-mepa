# include "regra29.h"
# include "../compilador.h"

# include <stdio.h>


void carregaVariavel()
{
  // Recupera atributos da variável
  attrsSimbolo_t attr;
  pop(pilhaAttrs, &attr, sizeof(attrsSimbolo_t));
  if (attr.cat != VAR_SIMPLES && attr.cat != PARAM_FORMAL)
    printErro("Símbolo %s não é variável nem parametro formal", token);

  // Coloca tipo na pilha e carrega valor/indireto
  if (attr.cat == VAR_SIMPLES)
  {
    pushTipo(pilhaTipos, attr.vsAttr.tipo);
    printComando(NULL, "CRVL %d,%d", attr.nivel, attr.vsAttr.desloc);
  }
  else if (attr.cat == PARAM_FORMAL)
  {
    pushTipo(pilhaTipos, attr.pfAttr.tipo);
    if (attr.pfAttr.porRef)
      printComando(NULL, "CRVI %d,%d", attr.nivel, attr.pfAttr.desloc);
    else
      printComando(NULL, "CRVL %d,%d", attr.nivel, attr.pfAttr.desloc);
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
