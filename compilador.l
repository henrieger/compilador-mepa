
/* -------------------------------------------------------------------
 *            Arquivo: compilador.l
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

%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

//#define IMPRIME(STR) {};
#define IMPRIME(STR) printf("%s    # %s \n", STR, token);

void subrotina_token(simbolos simb, const char *impressao);

int nl = 1;

%}

pulo_linha [\n]

descarte [\t ]+

ident [a-zA-Z][a-zA-Z0-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;}

{descarte}

{comentario}

program {
    subrotina_token(simb_program, "program\t\t\t");
    return PROGRAM;
}

label {
    subrotina_token(simb_label, "label\t\t\t");
    return LABEL;
}

type {
    subrotina_token(simb_type, "type\t\t\t");
    return TYPE;
}

array {
    subrotina_token(simb_array, "array\t\t\t");
    return ARRAY;
}

"\[" {
    subrotina_token(simb_abre_colchetes, "\tabre_colchetes\t");
    return ABRE_COLCHETES;
}

"\]" {
    subrotina_token(simb_fecha_colchetes, "\tfecha_colchetes");
    return FECHA_COLCHETES;
}

of {
    subrotina_token(simb_of, "\tof\t\t");
    return OF;
}

\.\. {
    subrotina_token(simb_ponto_ponto, "\tponto_ponto\t");
    return PONTO_PONTO;
}

var {
    subrotina_token(simb_var, "var\t\t\t");
    return VAR;
}

procedure {
    subrotina_token(simb_procedure, "procedure\t\t");
    return PROCEDURE;
}

function {
    subrotina_token(simb_function, "function\t\t");
    return FUNCTION;
}

begin {
    subrotina_token(simb_begin, "begin\t\t\t");
    return T_BEGIN;
}

end {
    subrotina_token(simb_end, "end\t\t\t");
    return T_END;
}

:= {
    subrotina_token(simb_atribuicao, "\tatribuicao\t");
    return ATRIBUICAO;
}

\; {
    subrotina_token(simb_ponto_e_virgula, "\tpt_virgula\t");
    return PONTO_E_VIRGULA;
}

\: {
    subrotina_token(simb_dois_pontos, "\tdois_pontos\t");
    return DOIS_PONTOS;
}

\, {
    subrotina_token(simb_virgula, "\tvirgula\t\t");
    return VIRGULA;
}

\. {
    subrotina_token(simb_ponto, "\tponto\t\t");
    return PONTO;
}

\( {
    subrotina_token(simb_abre_parenteses, "\tabre_parenteses\t");
    return ABRE_PARENTESES;
}

\) {
    subrotina_token(simb_fecha_parenteses, "\tfecha_parenteses");
    return FECHA_PARENTESES;
}

goto {
    subrotina_token(simb_goto, "goto\t\t\t");
    return GOTO;
}

if {
    subrotina_token(simb_if, "if\t\t\t");
    return IF;
}

then {
    subrotina_token(simb_then, "then\t\t\t");
    return THEN;
}

else {
    subrotina_token(simb_else, "else\t\t\t");
    return ELSE;
}

while {
    subrotina_token(simb_while, "while\t\t\t");
    return WHILE;
}

do {
    subrotina_token(simb_do, "do\t\t\t");
    return DO;
}

\= {
    subrotina_token(simb_igual, "\tigual\t\t");
    return IGUAL;
}

\<> {
    subrotina_token(simb_dif, "\tdif\t\t");
    return DIF;
}

\< {
    subrotina_token(simb_maior, "\tmenor\t\t");
    return MENOR;
}

\<\= {
    subrotina_token(simb_maior_igual, "\tmenor_igual\t");
    return MENOR_IGUAL;
}

\>\= {
    subrotina_token(simb_menor_igual, "\tmaior_igual\t");
    return MAIOR_IGUAL;
}

\> {
    subrotina_token(simb_menor, "\tmaior\t\t");
    return MAIOR;
}

\+ {
    subrotina_token(simb_mais, "\tmais\t\t");
    return MAIS;
}

\- {
    subrotina_token(simb_menos, "\tmenos\t\t");
    return MENOS;
}

or {
    subrotina_token(simb_or, "\tor\t\t");
    return OR;
}

\* {
    subrotina_token(simb_vezes, "\tvezes\t\t");
    return VEZES;
}

div {
    subrotina_token(simb_div, "\tdiv\t\t");
    return DIV;
}

and {
    subrotina_token(simb_and, "\tand\t\t");
    return AND;
}

not {
    subrotina_token(simb_not, "\tnot\t\t");
    return NOT;
}

read {
    subrotina_token(simb_read, "\tread\t\t");
    return READ;
}

write {
    subrotina_token(simb_write, "\twrite\t\t");
    return WRITE;
}

{numero} {
    subrotina_token(simb_numero, "\tnumero\t\t");
    return NUMERO;
}

{ident} {
    subrotina_token(simb_identificador, "\tident\t\t");
    return IDENT;
}


%%

void subrotina_token(simbolos simb, const char *impressao)
{
  simbolo = simb;
  strncpy(token, yytext, TAM_TOKEN);
  IMPRIME(impressao);
}

# ifdef LEXMAIN
int main (int argc, char ** argv) {
    yyin = fopen (argv[1], "r");
    
    // Imprime conteudo do arquivo de entrada
    char c = fgetc(yyin);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(yyin);
    }
    rewind(yyin);
    
    printf("---------------------------------------------------------\n");
    
    int ret = yylex();
    while (ret)
        ret = yylex();
    fclose(yyin);
}
# endif
