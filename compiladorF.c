
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *     Modificado por: Henrique Luiz Rieger
 *      Atualizado em: [12/10/2023, 20h:06m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include "compilador.h"


/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

// Atributos herdados
int num_vars;
int nivel_lexico = 0;
int desloc = 0;

// Pilhas
tabelaSimbolos_t tabelaSimbolos;
pilhaTipos_t pilhaTipos;
pilhaRotulos_t pilhaRotulos;
pilha_t *pilhaAttrs;
pilha_t *pilhaOperacoes;
pilha_t *pilhaIdents;

// Tipos de dados primitivos
tipoDado_t INTEGER;
tipoDado_t REAL;
tipoDado_t BOOLEAN;

// Variáveis auxiliares
char token[TAM_TOKEN];
int qtd_param;
simbolos simbolo;

FILE* fp=NULL;


void geraCodigo (char* rot, char* comando)
{
  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }

  if ( rot == NULL ) {
    fprintf(fp, "     %s\n", comando); fflush(fp);
  } else {
    fprintf(fp, "%s: %s \n", rot, comando); fflush(fp);
  }
}


int imprimeErro (const char* erro)
{
  return fprintf(stderr, "Erro na linha %d - %s\n", nl, erro);
}


void yyerror(const char *s)
{
  imprimeErro(s);
  exit(-1);
} 


void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno)
{
  tipoDado_t dadoPos = popTipo(pilhaTipos);
  tipoDado_t dadoPre = popTipo(pilhaTipos);

  if (tipoCertoPos > TIPO_NULO && dadoPos != tipoCertoPos)
    printErro("Variável não tem o tipo esperado");
        
  if (tipoCertoPre > TIPO_NULO && dadoPre != tipoCertoPre)
    printErro("Expressão antes não tem o tipo esperado");

  if (dadoPre != dadoPos)
    printErro("Expressão prévia não tem o mesmo tipo do termo");

  char comando[256];
  pop(pilhaOperacoes, (void *) comando, TAM_OP);
  geraCodigo(NULL, comando);

  pushTipo(pilhaTipos, tipoRetorno);
  # ifdef DEBUG
  printf("-- PILHA DE TIPOS --\n");
  imprimePilha(pilhaTipos, sizeof(tipoDado_t));
  # endif
}


attrsSimbolo_t *buscaProcedimentoOuFuncao(char *nomeProcOuFun)
{
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, nomeProcOuFun);
  if (!attr)
    printErro("Procedimento ou função %s não encontrado", nomeProcOuFun);

  if (attr->cat != PROCEDIMENTO && attr->cat != FUNCAO)
    printErro("Símbolo %s não é procedimento nem função", nomeProcOuFun);

  return attr;
}


void printComando(char *rotulo, const char *format, ...)
{
  char comando[256];

  va_list args;
  va_start (args, format);
  vsprintf(comando, format, args);
  va_end(args);
  
  geraCodigo(rotulo, comando);
}


void printErro(const char *format, ...)
{
  char erro[256];

  va_list args;
  va_start (args, format);
  vsprintf(erro, format, args);
  va_end(args);

  yyerror(erro);
}
