# include "regra22.h"
# include "../compilador.h"


void entraIf()
{
  rotulo_t rotuloPre, rotuloPos;

  proximoRotulo(rotuloPre);
  proximoRotulo(rotuloPos);

  // DSVF para cláusula ELSE
  printComando(NULL, "DSVF %s", rotuloPre);

  insereDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);
}


void fimIf()
{
  rotulo_t rotuloPre, rotuloPos;
  removeDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);

  // DSVS para depois da cláusula ELSE
  printComando(NULL, "DSVS %s", rotuloPos);
  geraCodigo(rotuloPre, "NADA");

  insereRotulo(pilhaRotulos, rotuloPos);
}


void fimElse()
{
  rotulo_t rotuloPos;
  popRotulo(pilhaRotulos, rotuloPos);

  // Rótulo de entrada da cláusula ELSE
  geraCodigo(rotuloPos, "NADA");
}
