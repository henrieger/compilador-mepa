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

# define TRUE 1
# define FALSE 0


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
extern pilha_t *pilhaContextos;

// Tipos de dados primitivos
extern tipoDado_t INTEGER;
extern tipoDado_t REAL;
extern tipoDado_t BOOLEAN;

// Variáveis auxiliares
extern char token[TAM_TOKEN]; // token atual
extern int qtd_param; // quantidade de parametros na chamada de funcao

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

// Imprime uma linha de código com rótulo
void geraCodigo (char*, char*);

// Fecha o arquivo MEPA
void fechaMEPA();

// Invoca o analisador léxico
int yylex();

// Informa erro ao usuário e aborta
void yyerror(const char *s);

// Avalia se a expressão pode ser realizada dados os tipos de entrada e empilha o retorno
void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno);

// Retorina atributos do procedimento ou função a partir do nome
attrsSimbolo_t *buscaProcedimentoOuFuncao(char *nomeProcOuFun);

// Envelopa geraCodigo com parâmetros variáveis
void printComando(char *rotulo, const char *format, ...);

// Envelopa yyerror com parâmetros variáveis
void printErro(const char *format, ...);

// Analisa o próximo parâmetro formal e ativa o contexto de parâmetro equivalente
void ativaContexto();

// Desativa o contexto de parâmetro atual
void desativaContexto();

// Checa se expressões compostas são permitidas no contexto atual
void checaContexto();

# endif
