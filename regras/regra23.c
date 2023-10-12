# include "regra23.h"
# include "../compilador.h"

rotulo_t rotuloPre, rotuloPos;


void entraWhile()
{
  proximoRotulo(rotuloPre);
  proximoRotulo(rotuloPos);

  geraCodigo(rotuloPre, "NADA");
}


void avaliaLacoWhile()
{
  printComando(NULL, "DSVF %s", rotuloPos);
  insereDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);
}


void fimWhile()
{
  removeDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);
  printComando(NULL, "DSVS %s", rotuloPre);
  geraCodigo(rotuloPos, "NADA");
}
