
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

# define TAM_OP 5

// Atributos herdados
int num_vars;
int nivel_lexico = 0;
int desloc = 0;

// Pilhas
tabelaSimbolos tabSim;
pilhaTipos pilhaTipo;
pilha_t *pilhaAttrs;
pilha_t *pilhaOperacoes;

// Tipos de dados primitivos
tipoDado INTEGER;
tipoDado REAL;
tipoDado BOOLEAN;
// # define INTEGER buscaTipoDado(tabSim, "integer")
// # define REAL buscaTipoDado(tabSim, "real")
// # define BOOLEAN buscaTipoDado(tabSim, "boolean")



// Variáveis auxiliares
char comando[256]; // contem comandos para impressao
char erro[256]; // contem msgs de erro

// Assinaturas de funções
void avaliaExpressao(tipoDado tipoCertoPre, tipoDado tipoCertoPos, tipoDado tipoRetorno);

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

%%

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

bloco:
    parte_declara_vars
    {
    }

    comando_composto

    {
      int dmem_qtd = 0;
      simbolo_t *ultimoSimbolo = (simbolo_t *) top(tabSim, sizeof(simbolo_t));
      
      while (
        ultimoSimbolo &&
        (void *) ultimoSimbolo > (void *) tabSim &&
        ultimoSimbolo->attrs->cat == VAR_SIMPLES &
        ultimoSimbolo->attrs->nivel == nivel_lexico
      )
      {
        dmem_qtd++;
        ultimoSimbolo--;
      }

      retiraSimbolos(tabSim, dmem_qtd);
      char dmemStr[16];
      sprintf(dmemStr, "DMEM %d", dmem_qtd);
      geraCodigo(NULL, dmemStr);
    }
;




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
        tipoDado tipo = buscaTipoDado(tabSim, token);
        # ifdef DEBUG
        printf("Tipo encontrado para %s: %d\n", token, tipo);
        # endif
        if (tipo == TIPO_NULO)
        {
          sprintf(erro, "Tipo %s não definido", token);
          yyerror(erro);
        }
        defineTipoUltimasNEntradas(tabSim, num_vars, tipo);
        # ifdef DEBUG
        printTabelaSimbolos(tabSim);
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
        insereSimbolo(tabSim, token, attrs);
        desloc++; num_vars++;
        # ifdef DEBUG
        printTabelaSimbolos(tabSim);
        # endif
    }
    | IDENT
    {
        /* insere vars na tabela de s�mbolos */
        attrsSimbolo_t *attrs = inicializaAttrsSimbolo(VAR_SIMPLES, nivel_lexico); 
        attrs->vsAttr = varSimples(TIPO_NULO, desloc);
        insereSimbolo(tabSim, token, attrs);
        desloc++; num_vars++;
        # ifdef DEBUG
        printTabelaSimbolos(tabSim);
        # endif
    }
;

lista_idents:
    lista_idents VIRGULA IDENT
    | IDENT
;


comando_composto:
    T_BEGIN comandos T_END
;

comandos:
    comando PONTO_E_VIRGULA comandos
    | comando
;

comando:
    rotulo comando_sem_rotulo
    | comando_sem_rotulo
;

rotulo:
    NUMERO DOIS_PONTOS
;

comando_sem_rotulo:
    atribuicao
    |
;

atribuicao:
    variavel ATRIBUICAO expressao
    {
      attrsSimbolo_t attr;
      pop(pilhaAttrs, &attr, sizeof(attrsSimbolo_t));
      tipoDado tipoExpressao = popTipo(pilhaTipo);
      # ifdef DEBUG
      printf("Tipo da expressao: %d\nTipo da variavel: %d\n", tipoExpressao, attr.vsAttr.tipo);
      # endif
      if (attr.vsAttr.tipo != tipoExpressao)
      {
        yyerror("Tipo da expressão incompatível com a variável");
      }
      sprintf(comando, "ARMZ %d %d", attr.nivel, attr.vsAttr.desloc);
      geraCodigo(NULL, comando);
    }
;

expressao:
    expressao_simples
    | expressao_simples relacao expressao_simples {avaliaExpressao(TIPO_NULO, TIPO_NULO, BOOLEAN);}
;

relacao:
    IGUAL {push(pilhaOperacoes, (void *) "CMIG", TAM_OP);}
    | DIF {push(pilhaOperacoes, (void *) "CMDG", TAM_OP);}
    | MAIOR {push(pilhaOperacoes, (void *) "CMMA", TAM_OP);}
    | MAIOR_IGUAL {push(pilhaOperacoes, (void *) "CMAI", TAM_OP);}
    | MENOR {push(pilhaOperacoes, (void *) "CMME", TAM_OP);}
    | MENOR_IGUAL {push(pilhaOperacoes, (void *) "CMEI", TAM_OP);}
;

expressao_simples:
    lista_termos
    | MAIS lista_termos
    | MENOS lista_termos
    {
    }
;

lista_termos:
    lista_termos operacao_termos_int termo {printf("oi");avaliaExpressao(INTEGER, INTEGER, INTEGER);} 
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
      pushTipo(pilhaTipo, attr.vsAttr.tipo);
      # ifdef DEBUG
      printf("-- PILHA DE TIPOS --\n");
      imprimePilha(pilhaTipo, sizeof(tipoDado));
      # endif
      sprintf(comando, "CRVL %d %d", attr.nivel, attr.vsAttr.desloc);
      geraCodigo(NULL, comando);
    }
     | NUMERO
      {
        pushTipo(pilhaTipo, INTEGER);
        # ifdef DEBUG
        printf("-- PILHA DE TIPOS --\n");
        imprimePilha(pilhaTipo, sizeof(tipoDado));
        # endif
        sprintf(comando, "CRCT %s", token);
        geraCodigo(NULL, comando);
      }
     // | chamada_funcao
     | ABRE_PARENTESES expressao FECHA_PARENTESES
     | NOT fator
;

variavel:
    IDENT
    {
      attrsSimbolo_t *attr = buscaSimbolo(tabSim, token);
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

// chamada_funcao:
//     IDENT
//     | IDENT 
//       ABRE_PARENTESES variavel FECHA_PARENTESES
// ;

%%

void avaliaExpressao(tipoDado tipoCertoPre, tipoDado tipoCertoPos, tipoDado tipoRetorno)
{
  tipoDado dadoPos = popTipo(pilhaTipo);
  tipoDado dadoPre = popTipo(pilhaTipo);

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

  pushTipo(pilhaTipo, tipoRetorno);
  # ifdef DEBUG
  printf("-- PILHA DE TIPOS --\n");
  imprimePilha(pilhaTipo, sizeof(tipoDado));
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
  tabSim = inicializaPilha();

  insereTipoDado(tabSim, "integer", 4, 0);
  insereTipoDado(tabSim, "real", 4, 0);
  insereTipoDado(tabSim, "boolean", 1, 0);

  INTEGER = buscaTipoDado(tabSim, "integer");
  REAL = buscaTipoDado(tabSim, "real");
  BOOLEAN = buscaTipoDado(tabSim, "boolean");
  
  # ifdef DEBUG
  printf("INTEGER: %d\nREAL: %d\nBOOLEAN: %d\n", INTEGER, REAL, BOOLEAN);
  # endif

// Inicializa pilhas extras
  pilhaTipo = inicializaPilha();
  pilhaAttrs = inicializaPilha();
  pilhaOperacoes = inicializaPilha();

// Parsing do codigo
  yyin=fp;
  yyparse();

// Destroi estruturas
  retiraSimbolos(tabSim, 3);
  destroiPilha(tabSim);
  destroiPilha(pilhaTipo);
  destroiPilha(pilhaAttrs);
  destroiPilha(pilhaOperacoes);

  return 0;
}
