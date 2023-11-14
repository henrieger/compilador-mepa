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


void comparaIdentParametro()
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

  listaParam_t parametroFormal = parametros[qtd_param];
  attrsSimbolo_t *parametroReal = buscaSimbolo(tabelaSimbolos, token);

  tipoDado_t tipoParametroReal;
  if (parametroReal->cat == VAR_SIMPLES)
    tipoParametroReal = parametroReal->vsAttr.tipo;
  else if (parametroReal->cat == PARAM_FORMAL)
    tipoParametroReal = parametroReal->pfAttr.tipo;
  else if (parametroReal->cat == FUNCAO)
    tipoParametroReal = parametroReal->funAttr.tipoRetorno;
  else
    printErro("Parâmetro %s não é um identificador válido como parâmetro", token);

  if (parametroFormal.tipo != tipoParametroReal)
    printErro(
      "Parâmetro de tipo %d recebeu expressão de tipo %d na função %s",
      parametroFormal.tipo, tipoParametroReal, procedimento
    );

  if (parametroFormal.porRef)
  {
    if (parametroReal->cat == FUNCAO)
      printErro("Parâmetro passado por referência recebeu função na função %s", procedimento);
    else if (parametroReal->cat == VAR_SIMPLES)
      printComando(NULL, "CREN %d,%d", parametroReal->nivel, parametroReal->vsAttr.desloc);
    else if (parametroReal->cat == PARAM_FORMAL && !(parametroReal->pfAttr.porRef))
      printComando(NULL, "CREN %d,%d", parametroReal->nivel, parametroReal->pfAttr.desloc);
    else if (parametroReal->cat == PARAM_FORMAL && !(parametroReal->pfAttr.porRef))
      printComando(NULL, "CRVL %d,%d", parametroReal->nivel, parametroReal->pfAttr.desloc);
  }
  else
  {
    // if (parametroReal->cat == FUNCAO)
       // Chama função
    if (parametroReal->cat == VAR_SIMPLES)
      printComando(NULL, "CRVL %d,%d", parametroReal->nivel, parametroReal->vsAttr.desloc);
    else if (parametroReal->cat == PARAM_FORMAL && !(parametroReal->pfAttr.porRef))
      printComando(NULL, "CRVL %d,%d", parametroReal->nivel, parametroReal->pfAttr.desloc);
    else if (parametroReal->cat == PARAM_FORMAL && !(parametroReal->pfAttr.porRef))
      printComando(NULL, "CRVI %d,%d", parametroReal->nivel, parametroReal->pfAttr.desloc);
  }

  qtd_param++;
}
