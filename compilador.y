
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
pilha_t *pilhaIdents;

// Tipos de dados primitivos
tipoDado_t INTEGER;
tipoDado_t REAL;
tipoDado_t BOOLEAN;


// Variáveis auxiliares
char comando[256]; // contem comandos para impressao
char erro[256]; // contem msgs de erro
rotulo_t rotuloPre; // "R00"
rotulo_t rotuloPos; // "R01"
int qtd_param; // quantidade de parametros na chamada de funcao

// Assinaturas de funções
void avaliaExpressao(tipoDado_t tipoCertoPre, tipoDado_t tipoCertoPos, tipoDado_t tipoRetorno);
void comparaExpressaoParametro(pilhaTipos_t pilhaTipos, tabelaSimbolos_t tabelaSimbolos);
attrsSimbolo_t *buscaProcedimentoOuFuncao(tabelaSimbolos_t tabelaSimbolos, char *nomeProcOuFun);

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
      proximoRotulo(rotuloPre);
      sprintf(comando, "DSVS %s", rotuloPre);
      geraCodigo(NULL, comando);
      insereRotulo(pilhaRotulos, rotuloPre);
    }
    parte_declara_subrotinas
    {
      popRotulo(pilhaRotulos, rotuloPre);
      geraCodigo(rotuloPre, "NADA");
      # ifdef DEBUG
      printTabelaSimbolos(tabelaSimbolos);
      # endif
    }

    comando_composto

    {
      int dmem_qtd = 0, simbolos_qtd = 0;
      simbolo_t *ultimoSimbolo = (simbolo_t *) top(tabelaSimbolos, sizeof(simbolo_t));
      
      while (ultimoSimbolo && ((void *) ultimoSimbolo > (void *) tabelaSimbolos->mem) && (ultimoSimbolo->attrs->nivel >= nivel_lexico))
      {
      simbolos_qtd++;
        if ((ultimoSimbolo->attrs->nivel == nivel_lexico) && (ultimoSimbolo->attrs->cat == VAR_SIMPLES))
          dmem_qtd++;
        ultimoSimbolo--;
      }

      retiraSimbolos(tabelaSimbolos, simbolos_qtd-1);
      # ifdef DEBUG
      printf("DMEM_QTD: %d, SIMBOLOS_QTD: %d\n", dmem_qtd, simbolos_qtd);
      # endif
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
    {
      tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
      if (tipo == TIPO_NULO)
      {
        sprintf(erro, "Tipo %s não definido", token);
        yyerror(erro);
      }
    }
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

lista_idents_params_valor: 
    lista_idents_params_valor VIRGULA IDENT
    {
        /* insere último param por valor na tabela de símbolos */
        attrsSimbolo_t *attrs = inicializaAttrsSimbolo(PARAM_FORMAL, nivel_lexico); 
        attrs->pfAttr = paramFormal(TIPO_NULO, -1, 0);
        insereSimbolo(tabelaSimbolos, token, attrs);
        qtd_param++;
        # ifdef DEBUG
        printTabelaSimbolos(tabelaSimbolos);
        # endif
    }
    | IDENT
    {
        /* insere param por valor na tabela de símbolos */
        attrsSimbolo_t *attrs = inicializaAttrsSimbolo(PARAM_FORMAL, nivel_lexico); 
        attrs->pfAttr = paramFormal(TIPO_NULO, -1, 0);
        insereSimbolo(tabelaSimbolos, token, attrs);
        qtd_param++;
        # ifdef DEBUG
        printTabelaSimbolos(tabelaSimbolos);
        # endif
    }
;


// Regra 11


parte_declara_subrotinas:
    subrotinas
    |
;

subrotinas:
  subrotinas declara_subrotina PONTO_E_VIRGULA
  | declara_subrotina PONTO_E_VIRGULA
;

declara_subrotina:
    declara_procedure
    | declara_funcao
;


// Regra 12
declara_procedure:
    PROCEDURE
    IDENT
    {
      nivel_lexico++;
      attrsSimbolo_t *attr = inicializaAttrsSimbolo(PROCEDIMENTO, nivel_lexico);
      proximoRotulo(rotuloPre);
      attr->procAttr = procedimento(rotuloPre, 0, NULL);
      sprintf(comando, "ENPR %d", nivel_lexico);
      geraCodigo(rotuloPre, comando);
      insereSimbolo(tabelaSimbolos, token, attr);
      push(pilhaIdents, token, TAM_TOKEN);
    }
    parametros_formais
    PONTO_E_VIRGULA bloco
    {
      pop(pilhaIdents, NULL, TAM_TOKEN);
      sprintf(comando, "RTPR %d,%d", nivel_lexico, 0);
      geraCodigo(NULL, comando);
      nivel_lexico--;
    }
;


// Regra 13
declara_funcao:
    FUNCTION IDENT parametros_formais DOIS_PONTOS tipo PONTO_E_VIRGULA bloco
;


// Regra 14
parametros_formais:
    ABRE_PARENTESES {qtd_param = 0;} lista_param_formais
    {
      attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(tabelaSimbolos, top(pilhaIdents, TAM_TOKEN));
      int qtdParamTotal = 0;
      simbolo_t *s, *topo = top(tabelaSimbolos, sizeof(simbolo_t));
      for (s = topo; s->attrs != attr; s--)
        qtdParamTotal++;
      attr->procAttr.numParam = qtdParamTotal;

      listaParam_t *listaParam = inicializaListaParam(qtdParamTotal);
      int i = 0;
      while (++s <= topo)
      {
        s->attrs->pfAttr.desloc = -(4+i);
        listaParam_t p = param(s->attrs->pfAttr.tipo, s->attrs->pfAttr.porRef);
        listaParam[i] = p;
        i++;
      }
      attr->procAttr.parametros = listaParam;
      # ifdef DEBUG
      printTabelaSimbolos(tabelaSimbolos);
      # endif
    } FECHA_PARENTESES
    |
;

lista_param_formais: 
    secao_param_formais
    PONTO_E_VIRGULA lista_param_formais
    | secao_param_formais
;


// Regra 15
secao_param_formais:
    lista_idents_params_valor DOIS_PONTOS tipo 
    {
      tipoDado_t tipo = buscaTipoDado(tabelaSimbolos, token);
      defineTipoUltimasNEntradas(tabelaSimbolos, qtd_param, tipo);
      qtd_param = 0;
    }
    | VAR lista_idents DOIS_PONTOS tipo
    | FUNCTION lista_idents DOIS_PONTOS tipo
    | PROCEDURE lista_idents DOIS_PONTOS tipo
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
    comando_com_identificador
    | comando_composto
    | comando_condicional
    | comando_repetitivo
;

comando_com_identificador:
    IDENT {push(pilhaIdents, token, TAM_TOKEN);}
    atribuicao_ou_procedimento
;

atribuicao_ou_procedimento:
    atribuicao
    | chamada_procedimento
;


// Regra 19
atribuicao:
    ATRIBUICAO expressao
    {
      char variavel[TAM_TOKEN];
      pop(pilhaIdents, variavel, TAM_TOKEN);
      attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, variavel);
      if (!attr)
      {
        sprintf(erro, "Variável %s não definida", variavel);
        yyerror(erro);
      }
      if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL)
      {
        sprintf(erro, "Símbolo %s não é variável nem parâmetro formal", variavel);
        yyerror(erro);
      }
      push(pilhaAttrs, (void *) attr, sizeof(attrsSimbolo_t));

      tipoDado_t tipoExpressao = popTipo(pilhaTipos);
      # ifdef DEBUG
      printf("Tipo da expressao: %d\nTipo da variavel: %d\n", tipoExpressao, attr->vsAttr.tipo);
      # endif
      if (attr->vsAttr.tipo != tipoExpressao)
      {
        yyerror("Tipo da expressão incompatível com a variável");
      }
      if (attr->cat == VAR_SIMPLES)
        sprintf(comando, "ARMZ %d,%d", attr->nivel, attr->vsAttr.desloc);
      if (attr->cat == PARAM_FORMAL)
        sprintf(comando, "ARMZ %d,%d", attr->nivel, attr->pfAttr.desloc);
      geraCodigo(NULL, comando);
    }
;


// Regra 20
chamada_procedimento:
    opt_lista_expressoes
    {
      char procedimento[TAM_TOKEN];
      pop(pilhaIdents, procedimento, TAM_TOKEN);
      attrsSimbolo_t *attr = buscaProcedimentoOuFuncao(tabelaSimbolos, procedimento);
      
      sprintf(comando, "CHPR %s,%d", attr->procAttr.rotulo, nivel_lexico);
      geraCodigo(NULL, comando);
    }
;

opt_lista_expressoes:
    ABRE_PARENTESES {qtd_param = 0;} lista_expressoes FECHA_PARENTESES
    |
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


// Regra 24
lista_expressoes:
    expressao {comparaExpressaoParametro(pilhaTipos, tabelaSimbolos);} VIRGULA lista_expressoes
    | expressao {comparaExpressaoParametro(pilhaTipos, tabelaSimbolos);}
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
      if (attr.cat != VAR_SIMPLES && attr.cat != PARAM_FORMAL)
      {
        sprintf(erro, "Símbolo %s não é variável nem parametro formal", token);
        yyerror(erro);
      }
      if (attr.cat == VAR_SIMPLES)
      {
        pushTipo(pilhaTipos, attr.vsAttr.tipo);
        sprintf(comando, "CRVL %d,%d", attr.nivel, attr.vsAttr.desloc);
      }
      if (attr.cat == PARAM_FORMAL)
      {
        pushTipo(pilhaTipos, attr.pfAttr.tipo);
        sprintf(comando, "CRVL %d,%d", attr.nivel, attr.pfAttr.desloc);
      }
      # ifdef DEBUG
      printf("-- PILHA DE TIPOS --\n");
      imprimePilha(pilhaTipos, sizeof(tipoDado_t));
      # endif
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
      if (attr->cat != VAR_SIMPLES && attr->cat != PARAM_FORMAL)
      {
        sprintf(erro, "Símbolo %s não é variável nem parametro formal", token);
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

attrsSimbolo_t *buscaProcedimentoOuFuncao(tabelaSimbolos_t tabelaSimbolos, char *nomeProcOuFun)
{
  attrsSimbolo_t *attr = buscaSimbolo(tabelaSimbolos, nomeProcOuFun);
  if (!attr)
  {
    sprintf(erro, "Procedimento ou função %s não encontrado", nomeProcOuFun);
    yyerror(erro);
  }
  if (attr->cat != PROCEDIMENTO && attr->cat != FUNCAO)
  {
    sprintf(erro, "Símbolo %s não é procedimento nem função", nomeProcOuFun);
    yyerror(erro);
  }

  return attr;
}

void comparaExpressaoParametro(pilhaTipos_t pilhaTipos, tabelaSimbolos_t tabelaSimbolos)
{
  char *procedimento = top(pilhaIdents, TAM_TOKEN);

  attrsSimbolo_t *proc_fun_attr = buscaProcedimentoOuFuncao(tabelaSimbolos, procedimento);

  listaParam_t *parametros;
  if (proc_fun_attr->cat == PROCEDIMENTO)
  {
    if (proc_fun_attr->procAttr.numParam == qtd_param)
    {
      sprintf(erro, "Parâmetro inesperado na função %s", procedimento);
      yyerror(erro);
    }
    parametros = proc_fun_attr->procAttr.parametros;
  }
  if (proc_fun_attr->cat == FUNCAO)
  {
    if (proc_fun_attr->funAttr.numParam == qtd_param)
    {
      sprintf(erro, "Parâmetro inesperado na função %s", procedimento);
      yyerror(erro);
    }
    parametros = proc_fun_attr->funAttr.parametros;
  }

  listaParam_t parametroAtual = parametros[qtd_param];

  if (parametroAtual.porRef)
  {
    sprintf(erro, "Parâmetro passado por referência recebeu expressão na função %s", procedimento);
    yyerror(erro);
  }

  tipoDado_t tipoParametroReal = topTipo(pilhaTipos);
  if (parametroAtual.tipo != tipoParametroReal)
  {
    sprintf(
      erro,
      "Parâmetro de tipo %d recebeu expressão de tipo %d na função %s",
      parametroAtual.tipo, tipoParametroReal, procedimento
    );
    yyerror(erro);
  }

  qtd_param++;
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
  pilhaIdents = inicializaPilha();

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
