
// Testar se funciona corretamente o empilhamento de par�metros
// passados por valor ou por refer�ncia.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "pilha.h"
#include "tabelaSimbolos.h"
#include "tipoDado.h"
#include "rotulos.h"

# define TAM_OP 5

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

// Tipos de dados primitivos
tipoDado_t INTEGER;
tipoDado_t REAL;
tipoDado_t BOOLEAN;
// # define INTEGER buscaTipoDado(tabelaSimbolos, "integer")
// # define REAL buscaTipoDado(tabelaSimbolos, "real")
// # define BOOLEAN buscaTipoDado(tabelaSimbolos, "boolean")



// Variáveis auxiliares
char comando[256]; // contem comandos para impressao
char erro[256]; // contem msgs de erro
rotulo_t rotuloPre; // "R00"
rotulo_t rotuloPos; // "R01"

// Assinaturas de funções
void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno);

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token LABEL TYPE ARRAY PROCEDURE FUNCTION
%token ABRE_COLCHETES FECHA_COLCHETES OF PONTO_PONTO
%token GOTO IF THEN ELSE
%token WHILE DO
%token IGUAL DIF MAIOR MAIOR_IGUAL MENOR MENOR_IGUAL
%token MAIS MENOS OR
%token VEZES DIV AND
%token NOT
%token NUMERO

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// Regra 1
programa:
    {
        geraCodigo (NULL, "INPP");
    }
    PROGRAM cabecalho_programa PONTO_E_VIRGULA
    bloco PONTO
    {
        geraCodigo (NULL, "PARA");
    }
;

cabecalho_programa:
    IDENT
    | IDENT ABRE_PARENTESES lista_idents FECHA_PARENTESES
;


// Regra 2
bloco: 
    parte_declara_vars
    {
    }

    comando_composto

    {
      int dmem_qtd = 0;
      simbolo_t *ultimoSimbolo = (simbolo_t *) top(tabelaSimbolos, sizeof(simbolo_t));
      
      while (
        ultimoSimbolo &&
        (void *) ultimoSimbolo > (void *) tabelaSimbolos &&
        ultimoSimbolo->attrs->cat == VAR_SIMPLES &
        ultimoSimbolo->attrs->nivel == nivel_lexico
      )
      {
        dmem_qtd++;
        ultimoSimbolo--;
      }

      retiraSimbolos(tabelaSimbolos, dmem_qtd);
      char dmemStr[16];
      sprintf(dmemStr, "DMEM %d", dmem_qtd);
      geraCodigo(NULL, dmemStr);
    }
;


// Regra 8
parte_declara_vars: 
     var
;


var:
    {
      desloc = 0;
    }
    VAR declara_vars
    |
;


// Regra 9
declara_vars: 
    declara_vars declara_var
    | declara_var
;

declara_var :
    {
        num_vars = 0;
    }
    lista_id_var DOIS_PONTOS
    tipo
    {
        // Define o tipo
        tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
        # ifdef DEBUG
        printf("Tipo encontrado para %s: %d\n", token, tipo);
        # endif
        if (tipo == TIPO_NULO)
        {
          sprintf(erro, "Tipo %s não definido", token);
          yyerror(erro);
        }
        defineTipoUltimasNEntradas(tabelaSimbolos, num_vars, tipo);
        # ifdef DEBUG
        printTabelaSimbolos(tabelaSimbolos);
        # endif

        /* AMEM */
        sprintf(comando, "AMEM %d", num_vars);
        geraCodigo(NULL, comando);
    }
    PONTO_E_VIRGULA
;

tipo:
    IDENT
;

lista_id_var:
    lista_id_var VIRGULA IDENT
    {
        /* insere �ltima vars na tabela de s�mbolos */
        attrsSimbolo_t *attrs = inicializaAttrsSimbolo(VAR_SIMPLES, nivel_lexico); 
        attrs->vsAttr = varSimples(TIPO_NULO, desloc);
        insereSimbolo(tabelaSimbolos, token, attrs);
        desloc++; num_vars++;
        # ifdef DEBUG
        printTabelaSimbolos(tabelaSimbolos);
        # endif
    }
    | IDENT
    {
        /* insere vars na tabela de s�mbolos */
        attrsSimbolo_t *attrs = inicializaAttrsSimbolo(VAR_SIMPLES, nivel_lexico); 
        attrs->vsAttr = varSimples(TIPO_NULO, desloc);
        insereSimbolo(tabelaSimbolos, token, attrs);
        desloc++; num_vars++;
        # ifdef DEBUG
        printTabelaSimbolos(tabelaSimbolos);
        # endif
    }
;


// Regra 10
lista_idents: 
    lista_idents VIRGULA IDENT
    | IDENT
;


// Regra 16
comando_composto: 
    T_BEGIN comandos T_END
;

comandos:
    comando PONTO_E_VIRGULA comandos
    | comando
;


// Regra 17
comando: 
    rotulo comando_sem_rotulo
    | comando_sem_rotulo
;

rotulo:
    NUMERO DOIS_PONTOS
;


// Regra 18
comando_sem_rotulo: 
    atribuicao
    | comando_composto
    | comando_condicional
    | comando_repetitivo
;


// Regra 19
atribuicao: 
    variavel ATRIBUICAO expressao
    {
      attrsSimbolo_t attr;
      pop(pilhaAttrs, &attr, sizeof(attrsSimbolo_t));
      tipoDado_t tipoExpressao = popTipo(pilhaTipos);
      # ifdef DEBUG
      printf("Tipo da expressao: %d\nTipo da variavel: %d\n", tipoExpressao, attr.vsAttr.tipo);
      # endif
      if (attr.vsAttr.tipo != tipoExpressao)
      {
        yyerror("Tipo da expressão incompatível com a variável");
      }
      sprintf(comando, "ARMZ %d,%d", attr.nivel, attr.vsAttr.desloc);
      geraCodigo(NULL, comando);
    }
;


// Regra 22
comando_condicional:
    IF expressao
    {
      proximoRotulo(rotuloPre);
      proximoRotulo(rotuloPos);

      sprintf(comando, "DSVF %s", rotuloPre);
      geraCodigo(NULL, comando);

      insereDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);
    }
    THEN comando_sem_rotulo
    {
      removeDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);

      sprintf(comando, "DSVS %s", rotuloPos);
      geraCodigo(NULL, comando);

      geraCodigo(rotuloPre, "NADA");

      insereRotulo(pilhaRotulos, rotuloPos);
    }
    else
    {
      popRotulo(pilhaRotulos, rotuloPos);
      geraCodigo(rotuloPos, "NADA");
    }
;

else:
    ELSE comando_sem_rotulo
    | %prec LOWER_THAN_ELSE
;

// Regra 23
comando_repetitivo:
    WHILE
    {
      proximoRotulo(rotuloPre);
      proximoRotulo(rotuloPos);

      geraCodigo(rotuloPre, "NADA");
    }
    expressao
    {
      sprintf(comando, "DSVF %s", rotuloPos);
      geraCodigo(NULL, comando);

      insereDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);
    }
    DO comando_sem_rotulo
    {
      removeDoisRotulos(pilhaRotulos, rotuloPre, rotuloPos);

      sprintf(comando, "DSVS %s", rotuloPre);
      geraCodigo(NULL, comando);

      geraCodigo(rotuloPos, "NADA");
    }
;


// Regra 25
expressao: 
    expressao_simples
    | expressao_simples relacao expressao_simples {avaliaExpressao(TIPO_NULO, TIPO_NULO, BOOLEAN);}
;


// Regra 26
relacao: 
    IGUAL {push(pilhaOperacoes, (void *) "CMIG", TAM_OP);}
    | DIF {push(pilhaOperacoes, (void *) "CMDG", TAM_OP);}
    | MAIOR {push(pilhaOperacoes, (void *) "CMMA", TAM_OP);}
    | MAIOR_IGUAL {push(pilhaOperacoes, (void *) "CMAG", TAM_OP);}
    | MENOR {push(pilhaOperacoes, (void *) "CMME", TAM_OP);}
    | MENOR_IGUAL {push(pilhaOperacoes, (void *) "CMEG", TAM_OP);}
;


// Regra 27
expressao_simples: 
    lista_termos
    | MAIS lista_termos
    | MENOS lista_termos {geraCodigo(NULL, "INVR");}
;

lista_termos:
    lista_termos operacao_termos_int termo {avaliaExpressao(INTEGER, INTEGER, INTEGER);} 
    | lista_termos operacao_termos_bool termo {avaliaExpressao(BOOLEAN, BOOLEAN, BOOLEAN);} 
    | termo
;

operacao_termos_int:
    MAIS    {push(pilhaOperacoes, (void *) "SOMA", TAM_OP);}
    | MENOS {push(pilhaOperacoes, (void *) "SUBT", TAM_OP);}
;

operacao_termos_bool:
    OR {push(pilhaOperacoes, (void *) "DISJ", TAM_OP);}
;


// Regra 28
termo: 
    termo operacao_fatores_bool fator {avaliaExpressao(BOOLEAN, BOOLEAN, BOOLEAN);} 
    | termo operacao_fatores_int fator {avaliaExpressao(INTEGER, INTEGER, INTEGER);} 
    | fator
;

operacao_fatores_int:
    VEZES {push(pilhaOperacoes, (void *) "MULT", TAM_OP);}
    | DIV {push(pilhaOperacoes, (void *) "DIVI", TAM_OP);}
;
    
operacao_fatores_bool:
    AND {push(pilhaOperacoes, (void *) "CONJ", TAM_OP);}
;


// Regra 29
fator: 
    variavel
    {
      attrsSimbolo_t attr;
      pop(pilhaAttrs, &attr, sizeof(attrsSimbolo_t));
      if (attr.cat != VAR_SIMPLES)
      {
        sprintf(erro, "Símbolo %s não é variável", token);
        yyerror(erro);
      }
      pushTipo(pilhaTipos, attr.vsAttr.tipo);
      # ifdef DEBUG
      printf("-- PILHA DE TIPOS --\n");
      imprimePilha(pilhaTipos, sizeof(tipoDado_t));
      # endif
      sprintf(comando, "CRVL %d,%d", attr.nivel, attr.vsAttr.desloc);
      geraCodigo(NULL, comando);
    }
     | NUMERO
      {
        pushTipo(pilhaTipos, INTEGER);
        # ifdef DEBUG
        printf("-- PILHA DE TIPOS --\n");
        imprimePilha(pilhaTipos, sizeof(tipoDado_t));
        # endif
        sprintf(comando, "CRCT %s", token);
        geraCodigo(NULL, comando);
      }
     // | chamada_funcao
     | ABRE_PARENTESES expressao FECHA_PARENTESES
     | NOT fator
;


// Regra 30
variavel: 
    IDENT
    {
      attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, token);
      if (!attr)
      {
        sprintf(erro, "Variável %s não definida", token);
        yyerror(erro);
      }
      if (attr->cat != VAR_SIMPLES)
      {
        sprintf(erro, "Símbolo %s não é variável", token);
        yyerror(erro);
      }
      push(pilhaAttrs, (void *) attr, sizeof(attrsSimbolo_t));
    }
;


// Regra 31
// chamada_funcao:
//     IDENT
//     | IDENT 
//       ABRE_PARENTESES variavel FECHA_PARENTESES
// ;

%%

void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno)
{
  tipoDado_t dadoPos = popTipo(pilhaTipos);
  tipoDado_t dadoPre = popTipo(pilhaTipos);

  if (tipoCertoPos > TIPO_NULO && dadoPos != tipoCertoPos)
  {
    sprintf(erro, "Variável não tem o tipo esperado");
    yyerror(erro);
  }
        
  if (tipoCertoPre > TIPO_NULO && dadoPre != tipoCertoPre)
  {
    sprintf(erro, "Expressão antes não tem o tipo esperado");
    yyerror(erro);
  }

  if (dadoPre != dadoPos)
  {
    sprintf(erro, "Expressão prévia não tem o mesmo tipo do termo");
    yyerror(erro);
  }

  pop(pilhaOperacoes, (void *) comando, TAM_OP);
  geraCodigo(NULL, comando);

  pushTipo(pilhaTipos, tipoRetorno);
  # ifdef DEBUG
  printf("-- PILHA DE TIPOS --\n");
  imprimePilha(pilhaTipos, sizeof(tipoDado_t));
  # endif
}

int main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }


/* -------------------------------------------------------------------
 *  Inicia a Tabela de S�mbolos
 * ------------------------------------------------------------------- */
  tabelaSimbolos = inicializaPilha();

  insereTipoDado(tabelaSimbolos, "integer", 4, 0);
  insereTipoDado(tabelaSimbolos, "real", 4, 0);
  insereTipoDado(tabelaSimbolos, "boolean", 1, 0);

  INTEGER = buscaTipoDado(tabelaSimbolos, "integer");
  REAL = buscaTipoDado(tabelaSimbolos, "real");
  BOOLEAN = buscaTipoDado(tabelaSimbolos, "boolean");
  
  # ifdef DEBUG
  printf("INTEGER: %d\nREAL: %d\nBOOLEAN: %d\n", INTEGER, REAL, BOOLEAN);
  # endif

// Inicializa pilhas extras
  pilhaTipos = inicializaPilha();
  pilhaAttrs = inicializaPilha();
  pilhaOperacoes = inicializaPilha();
  pilhaRotulos = inicializaPilha();

// Parsing do codigo
  yyin=fp;
  yyparse();

// Destroi estruturas
  retiraSimbolos(tabelaSimbolos, 3);
  destroiPilha(tabelaSimbolos);
  destroiPilha(pilhaTipos);
  destroiPilha(pilhaAttrs);
  destroiPilha(pilhaOperacoes);
  destroiPilha(pilhaRotulos);

  return 0;
}
