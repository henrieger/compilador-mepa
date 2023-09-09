# ifndef __TABELA_SIMBOLOS_H__
# define __TABELA_SIMBOLOS_H__

# include "compilador.h"
# include "pilha.h"

# define tabelaSimbolos pilha_t *

enum simbCat
{
  VAR_SIMPLES,
  PARAM_FORMAL,
  PROCEDIMENTO,
  FUNCAO,
  ROTULO,
  TIPO_DADO
};

struct varSimplesAttr
{
  int tipo;
  unsigned int desloc;
};

struct paramFormalAttr
{
  int tipo;
  unsigned int desloc;
  char porRef;
};

typedef struct listaParam
{
  int tipo;
  char porRef;
} listaParam_t;

struct procedimentoAttr
{
  char rotulo[16];
  unsigned int numParam;
  listaParam_t *parametros;
};

struct funcaoAttr
{
  char rotulo[16];
  int tipoRetorno;
  unsigned int numParam;
  listaParam_t *parametros;
};

struct rotuloAttr
{
  unsigned int linhaCodigo;
};

struct tipoDadoAttr
{
  unsigned int tam;
};

typedef struct attrsSimbolo
{
  enum simbCat cat;
  char nivel;
  union
  {
    struct varSimplesAttr vsAttr;
    struct paramFormalAttr pfAttr;
    struct procedimentoAttr procAttr;
    struct funcaoAttr funAttr;
    struct rotuloAttr rotAttr;
    struct tipoDadoAttr tdAttr;
  };
} attrsSimbolo_t;

typedef struct simbolo
{
  char ident[TAM_TOKEN];
  attrsSimbolo_t *attrs;
} simbolo_t;

// Inicializa atributos de um simbolo
attrsSimbolo_t *inicializaAttrsSimbolo(enum simbCat cat, char nivel);

// Inicializa lista de parametros de função/procedimento
listaParam_t *inicializaListaParam(int tam);

// Retorna um parametro de função/procedimento
listaParam_t param(int tipo, char porRef);

// Retorna struct com atributos de variavel simples
struct varSimplesAttr varSimples(int tipo, unsigned int desloc);

// Retorna struct com atributos de parametro formal
struct paramFormalAttr paramFormal(int tipo, unsigned int desloc, char porRef);

// Retorna struct com atributos de procedimento
struct procedimentoAttr procedimento(char *rotulo, unsigned int numParam, listaParam_t *parametros);

// Retorna struct com atributos de procedimento
struct funcaoAttr funcao(int tipo, char *rotulo, unsigned int numParam, listaParam_t *parametros);

// Retorna struct com atributos de rótulo
struct rotuloAttr rotulo(unsigned int linhaCodigo);

// Retorna struct com atributos de tipo de dados
struct tipoDadoAttr _tipoDado(unsigned int tam);

// Insere um simbolo na tabela de simbolos
int insereSimbolo(tabelaSimbolos ts, char *ident, attrsSimbolo_t *as);

// Busca atributos de um simbolo
attrsSimbolo_t *buscaSimbolo(tabelaSimbolos ts, char *ident);

// Retira últimos n símbolos da tabela
int retiraSimbolos(tabelaSimbolos ts, int n);

// Destroi lista de parametros
void destroiListaParametros(listaParam_t *l);

// Destroi estruturas do símbolo
void destroiAttrsSimbolo(attrsSimbolo_t *as);

// Imprime um simbolo na tela
void printSimbolo(char *ident, attrsSimbolo_t* as);

// Imprime a tabela de simbolos
void printTabelaSimbolos(tabelaSimbolos ts);

# endif
