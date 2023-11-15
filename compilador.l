
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

void subrotina_token(const char *impressao);

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
    subrotina_token("program\t\t\t");
    return PROGRAM;
}

label {
    subrotina_token("label\t\t\t");
    return LABEL;
}

type {
    subrotina_token("type\t\t\t");
    return TYPE;
}

array {
    subrotina_token("array\t\t\t");
    return ARRAY;
}

"\[" {
    subrotina_token("\tabre_colchetes\t");
    return ABRE_COLCHETES;
}

"\]" {
    subrotina_token("\tfecha_colchetes");
    return FECHA_COLCHETES;
}

of {
    subrotina_token("\tof\t\t");
    return OF;
}

\.\. {
    subrotina_token("\tponto_ponto\t");
    return PONTO_PONTO;
}

var {
    subrotina_token("var\t\t\t");
    return VAR;
}

procedure {
    subrotina_token("procedure\t\t");
    return PROCEDURE;
}

function {
    subrotina_token("function\t\t");
    return FUNCTION;
}

begin {
    subrotina_token("begin\t\t\t");
    return T_BEGIN;
}

end {
    subrotina_token("end\t\t\t");
    return T_END;
}

:= {
    subrotina_token("\tatribuicao\t");
    return ATRIBUICAO;
}

\; {
    subrotina_token("\tpt_virgula\t");
    return PONTO_E_VIRGULA;
}

\: {
    subrotina_token("\tdois_pontos\t");
    return DOIS_PONTOS;
}

\, {
    subrotina_token("\tvirgula\t\t");
    return VIRGULA;
}

\. {
    subrotina_token("\tponto\t\t");
    return PONTO;
}

\( {
    subrotina_token("\tabre_parenteses\t");
    return ABRE_PARENTESES;
}

\) {
    subrotina_token("\tfecha_parenteses");
    return FECHA_PARENTESES;
}

goto {
    subrotina_token("goto\t\t\t");
    return GOTO;
}

if {
    subrotina_token("if\t\t\t");
    return IF;
}

then {
    subrotina_token("then\t\t\t");
    return THEN;
}

else {
    subrotina_token("else\t\t\t");
    return ELSE;
}

while {
    subrotina_token("while\t\t\t");
    return WHILE;
}

do {
    subrotina_token("do\t\t\t");
    return DO;
}

\= {
    subrotina_token("\tigual\t\t");
    return IGUAL;
}

\<> {
    subrotina_token("\tdif\t\t");
    return DIF;
}

\< {
    subrotina_token("\tmenor\t\t");
    return MENOR;
}

\<\= {
    subrotina_token("\tmenor_igual\t");
    return MENOR_IGUAL;
}

\>\= {
    subrotina_token("\tmaior_igual\t");
    return MAIOR_IGUAL;
}

\> {
    subrotina_token("\tmaior\t\t");
    return MAIOR;
}

\+ {
    subrotina_token("\tmais\t\t");
    return MAIS;
}

\- {
    subrotina_token("\tmenos\t\t");
    return MENOS;
}

or {
    subrotina_token("\tor\t\t");
    return OR;
}

\* {
    subrotina_token("\tvezes\t\t");
    return VEZES;
}

div {
    subrotina_token("\tdiv\t\t");
    return DIV;
}

and {
    subrotina_token("\tand\t\t");
    return AND;
}

not {
    subrotina_token("\tnot\t\t");
    return NOT;
}

read {
    subrotina_token("\tread\t\t");
    return READ;
}

write {
    subrotina_token("\twrite\t\t");
    return WRITE;
}

{numero} {
    subrotina_token("\tnumero\t\t");
    return NUMERO;
}

{ident} {
    subrotina_token("\tident\t\t");
    return IDENT;
}


%%

void subrotina_token(const char *impressao)
{
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
