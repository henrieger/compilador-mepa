/* -------------------------------------------------------------------
 *            Arquivo: compilador.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *     Modificado por: Henrique Luiz Rieger
 *      Atualizado em: [20/08/2023, 14h:38m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e variáveis globais do compilador (via extern)
 *
 * ------------------------------------------------------------------- */

# ifndef __COMPILADOR_H__
# define __COMPILADOR_H__

#define TAM_TOKEN 16

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
} simbolos;



/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;


/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

void geraCodigo (char*, char*);
int yylex();
void yyerror(const char *s);

# endif
