%{
# include <stdio.h>
# include "regras.h"
# include "compilador.h"
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
%token READ WRITE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// Regra 1
programa:
    { geraCodigo (NULL, "INPP"); }
    PROGRAM cabecalho_programa PONTO_E_VIRGULA
    bloco PONTO
    { geraCodigo (NULL, "PARA"); }
;

cabecalho_programa:
    IDENT
    | IDENT ABRE_PARENTESES lista_idents FECHA_PARENTESES
;


// Regra 2
bloco: 
    parte_declara_vars { desviaProcedures(); }
    parte_declara_subrotinas { entraCodigoBloco(); }
    comando_composto
;


// Regra 8
parte_declara_vars: 
     var
;


var:
    { desloc = 0; } VAR declara_vars
    |
;


// Regra 9
declara_vars: 
    declara_vars declara_var
    | declara_var
;

declara_var :
    { num_vars = 0; } lista_id_var DOIS_PONTOS tipo { amemTipo(); } PONTO_E_VIRGULA
;

tipo:
    IDENT { verificaTipo(); }
;

lista_id_var:
    lista_id_var VIRGULA IDENT { insereVarsTabSim(); }
    | IDENT { insereVarsTabSim(); }
;


// Regra 10
lista_idents: 
    lista_idents VIRGULA IDENT
    | IDENT
;

lista_idents_params_valor: 
    lista_idents_params_valor VIRGULA IDENT { insereParamsTabSim(FALSE); }
    | IDENT { insereParamsTabSim(FALSE); }
;

lista_idents_params_ref: 
    lista_idents_params_ref VIRGULA IDENT { insereParamsTabSim(TRUE); }
    | IDENT { insereParamsTabSim(TRUE); }
;

lista_idents_read:
    lista_idents_read VIRGULA IDENT { leituraIdents(); }
    | IDENT { leituraIdents (); }


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
    PROCEDURE IDENT { entraProcedure(); } parametros_formais PONTO_E_VIRGULA
    bloco
    { retornaProcedure(); }
;


// Regra 13
declara_funcao:
    FUNCTION IDENT parametros_formais DOIS_PONTOS tipo PONTO_E_VIRGULA
    bloco
;


// Regra 14
parametros_formais:
    ABRE_PARENTESES { qtd_param = 0; } lista_param_formais { insereListaParam(); } FECHA_PARENTESES
    |
;

lista_param_formais: 
    secao_param_formais
    PONTO_E_VIRGULA lista_param_formais
    | secao_param_formais
;


// Regra 15
secao_param_formais:
    lista_idents_params_valor DOIS_PONTOS tipo { defineTipoParams(); }
    | VAR lista_idents_params_ref DOIS_PONTOS tipo { defineTipoParams(); }
    // | FUNCTION lista_idents DOIS_PONTOS tipo
    // | PROCEDURE lista_idents DOIS_PONTOS tipo
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
    | read
    | write
;

comando_com_identificador:
    IDENT { push(pilhaIdents, token, TAM_TOKEN); }
    atribuicao_ou_procedimento
;

atribuicao_ou_procedimento:
    atribuicao
    | chamada_procedimento
;


// Regra 19
atribuicao:
    ATRIBUICAO expressao { atribuicao(); }
;


// Regra 20
chamada_procedimento:
    opt_lista_expressoes { chamaProcedimento(); }
;

opt_lista_expressoes:
    ABRE_PARENTESES {qtd_param = 0;} lista_expressoes FECHA_PARENTESES
    |
;


// Regra 22
comando_condicional:
    IF expressao { entraIf(); }
    THEN comando_sem_rotulo { fimIf(); }
    else { fimElse(); }
;

else:
    ELSE comando_sem_rotulo
    | %prec LOWER_THAN_ELSE
;

// Regra 23
comando_repetitivo:
    WHILE { entraWhile(); } expressao { avaliaLacoWhile(); } DO
    comando_sem_rotulo
    { fimWhile(); }
;


// Regra 24
lista_expressoes:
    IDENT { comparaIdentParametro(); } VIRGULA lista_expressoes
    | expressao { comparaExpressaoParametro(); } VIRGULA lista_expressoes
    | expressao { comparaExpressaoParametro(); }
;

lista_expressoes_write:
    expressao { geraCodigo(NULL, "IMPR"); } VIRGULA lista_expressoes_write
    | expressao { geraCodigo(NULL, "IMPR"); }
;


// Regra 25
expressao: 
    expressao_simples
    | expressao_simples relacao expressao_simples { avaliaExpressao(TIPO_NULO, TIPO_NULO, BOOLEAN); }
;


// Regra 26
relacao: 
    IGUAL         { push(pilhaOperacoes, (void *) "CMIG", TAM_OP); }
    | DIF         { push(pilhaOperacoes, (void *) "CMDG", TAM_OP); }
    | MAIOR       { push(pilhaOperacoes, (void *) "CMMA", TAM_OP); }
    | MAIOR_IGUAL { push(pilhaOperacoes, (void *) "CMAG", TAM_OP); }
    | MENOR       { push(pilhaOperacoes, (void *) "CMME", TAM_OP); }
    | MENOR_IGUAL { push(pilhaOperacoes, (void *) "CMEG", TAM_OP); }
;


// Regra 27
expressao_simples: 
    lista_termos
    | MAIS lista_termos
    | MENOS lista_termos { geraCodigo(NULL, "INVR"); }
;

lista_termos:
    lista_termos operacao_termos_int termo    { avaliaExpressao(INTEGER, INTEGER, INTEGER); } 
    | lista_termos operacao_termos_bool termo { avaliaExpressao(BOOLEAN, BOOLEAN, BOOLEAN); } 
    | termo
;

operacao_termos_int:
    MAIS    { push(pilhaOperacoes, (void *) "SOMA", TAM_OP); }
    | MENOS { push(pilhaOperacoes, (void *) "SUBT", TAM_OP); }
;

operacao_termos_bool:
    OR { push(pilhaOperacoes, (void *) "DISJ", TAM_OP); }
;


// Regra 28
termo: 
    termo operacao_fatores_bool fator   { avaliaExpressao(BOOLEAN, BOOLEAN, BOOLEAN); } 
    | termo operacao_fatores_int fator  { avaliaExpressao(INTEGER, INTEGER, INTEGER); } 
    | fator
;

operacao_fatores_int:
    VEZES { push(pilhaOperacoes, (void *) "MULT", TAM_OP); }
    | DIV { push(pilhaOperacoes, (void *) "DIVI", TAM_OP); }
;
    
operacao_fatores_bool:
    AND { push(pilhaOperacoes, (void *) "CONJ", TAM_OP); }
;


// Regra 29
fator: 
    variavel { carregaVariavel(); }
    | NUMERO { carregaConstante(); }
     // | chamada_funcao
    | ABRE_PARENTESES expressao FECHA_PARENTESES
    | NOT fator
;


// Regra 30
variavel: 
    IDENT { empilhaAtributosVar(); }
;


// Regra 31
// chamada_funcao:
//     IDENT
//     | IDENT 
//       ABRE_PARENTESES variavel FECHA_PARENTESES
// ;


// "Regra 36" - READ
read:
    READ ABRE_PARENTESES lista_idents_read FECHA_PARENTESES
;


// "Regra 37" - WRITE
write:
    WRITE ABRE_PARENTESES lista_expressoes_write FECHA_PARENTESES


%%

int main (int argc, char** argv)
{
  FILE* fp;
  extern FILE* yyin;

  if (argc != 2)
  {
    printf("Usage: %s <arq>a %d\n", argv[0], argc);
    return -1;
  }

  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("Usage: %s <arq>b\n", argv[0]);
    return -1;
  }

  // Inicia a Tabela de Símbolos
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
