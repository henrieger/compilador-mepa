/* -------------------------------------------------------------------
 *            Arquivo: compilador.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *     Modificado por: Henrique Luiz Rieger
 *      Atualizado em: [18/10/2023, 18h:54m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e variáveis globais do compilador (via extern)
 *
 * ------------------------------------------------------------------- */

# ifndef __COMPILADOR_H__
# define __COMPILADOR_H__

# include "tamanhos.h"
# include "pilha.h"
# include "tabelaSimbolos.h"
# include "tipoDado.h"
# include "rotulos.h"

typedef enum simbolos {
  simb_program, simb_var, simb_begin, simb_end,
  simb_identificador, simb_numero,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_label, simb_type, simb_array, simb_procedure, simb_function,
  simb_abre_colchetes, simb_fecha_colchetes, simb_of, simb_ponto_ponto,
  simb_goto, simb_if, simb_then, simb_else,
  simb_while, simb_do,
  simb_igual, simb_dif, simb_maior, simb_maior_igual, simb_menor, simb_menor_igual,
  simb_mais, simb_menos, simb_or,
  simb_vezes, simb_div, simb_and,
  simb_not,
  simb_read, simb_write,
} simbolos;


/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

// Atributos herdados
extern int num_vars;
extern int nivel_lexico;
extern int desloc;
extern int nl;

// Pilhas
extern tabelaSimbolos_t tabelaSimbolos;
extern pilhaTipos_t pilhaTipos;
extern pilhaRotulos_t pilhaRotulos;
extern pilha_t *pilhaAttrs;
extern pilha_t *pilhaOperacoes;
extern pilha_t *pilhaIdents;

// Tipos de dados primitivos
extern tipoDado_t INTEGER;
extern tipoDado_t REAL;
extern tipoDado_t BOOLEAN;

// Variáveis auxiliares
extern char token[TAM_TOKEN]; // token atual
extern int qtd_param; // quantidade de parametros na chamada de funcao
extern simbolos simbolo; // simbolo lido pelo analisador lexico

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

// Imprime uma linha de código com rótulo
void geraCodigo (char*, char*);

// Invoca o analisador léxico
int yylex();

// Informa erro ao usuário e aborta
void yyerror(const char *s);

// Avalia se a expressão pode ser realizada dados os tipos de entrada e empilha o retorno
void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno);

// Compara se o tipo do parâmetro formal corresponde ao tipo do parâmetro real
void comparaExpressaoParametro();

// Retorina atributos do procedimento ou função a partir do nome
attrsSimbolo_t *buscaProcedimentoOuFuncao(char *nomeProcOuFun);

// Envelopa geraCodigo com parâmetros variáveis
void printComando(char *rotulo, const char *format, ...);

// Envelopa yyerror com parâmetros variáveis
void printErro(const char *format, ...);

# endif
