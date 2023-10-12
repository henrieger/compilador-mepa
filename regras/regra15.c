# include "regra15.h"
# include "../compilador.h"


void defineTipoParams()
{
  tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
  defineTipoUltimasNEntradas(tabelaSimbolos, qtd_param, tipo);
  qtd_param = 0;
}
