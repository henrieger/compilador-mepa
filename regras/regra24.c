# include "regra24.h"
# include "../compilador.h"


void comparaExpressaoParametro()
{
  char *procedimento = top(pilhaIdents, TAM_TOKEN);

  attrsSimbolo_t *proc_fun_attr = buscaProcedimentoOuFuncao(procedimento);

  listaParam_t *parametros;
  if (proc_fun_attr->cat == PROCEDIMENTO)
  {
    if (proc_fun_attr->procAttr.numParam == qtd_param)
      printErro("Parâmetro inesperado no procedimento %s", procedimento);

    parametros = proc_fun_attr->procAttr.parametros;
  }
  else if (proc_fun_attr->cat == FUNCAO)
  {
    if (proc_fun_attr->funAttr.numParam == qtd_param)
      printErro("Parâmetro inesperado na função %s", procedimento);

    parametros = proc_fun_attr->funAttr.parametros;
  }

  listaParam_t parametroAtual = parametros[qtd_param];

  tipoDado_t tipoParametroReal = topTipo(pilhaTipos);
  if (parametroAtual.tipo != tipoParametroReal)
    printErro(
      "Parâmetro de tipo %d recebeu expressão de tipo %d na função %s",
      parametroAtual.tipo, tipoParametroReal, procedimento
    );

  qtd_param++;
}
