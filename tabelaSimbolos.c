# include "tabelaSimbolos.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "compilador.h"
# include "pilha.h"

// Inicializa atributos de um simbolo
attrsSimbolo_t *inicializaAttrsSimbolo(enum simbCat cat, char nivel)
{
  attrsSimbolo_t *as = malloc(sizeof(attrsSimbolo_t));
  if (!as) return NULL;

  as->cat = cat;
  as->nivel = nivel;

  return as;
}

// Inicializa lista de parametros de função/procedimento
listaParam_t *inicializaListaParam(int tam)
{
  listaParam_t *l = malloc(tam * sizeof(listaParam_t));
  return l;
}

// Retorna um parametro de função/procedimento
listaParam_t param(int tipo, char porRef)
{
  listaParam_t p;
  p.tipo = tipo;
  p.porRef = porRef;

  return p;
}

// Retorna struct com atributos de variavel simples
struct varSimplesAttr varSimples(int tipo, unsigned int desloc)
{
  struct varSimplesAttr vs;
  vs.tipo = tipo;
  vs.desloc = desloc;

  return vs;
}

// Retorna struct com atributos de parametro formal
struct paramFormalAttr paramFormal(int tipo, unsigned int desloc, char porRef)
{
  struct paramFormalAttr pf;
  pf.tipo = tipo;
  pf.desloc = desloc;
  pf.porRef = porRef;

  return pf;
}

// Retorna struct com atributos de procedimento
struct procedimentoAttr procedimento(char *rotulo, unsigned int numParam, listaParam_t *parametros)
{
  struct procedimentoAttr p;
  memcpy(p.rotulo, rotulo, TAM_TOKEN);
  p.numParam = numParam;
  p.parametros = parametros;

  return p;
}

// Retorna struct com atributos de procedimento
struct funcaoAttr funcao(int tipo, char *rotulo, unsigned int numParam, listaParam_t *parametros)
{
  struct funcaoAttr f;
  f.tipoRetorno = tipo;
  memcpy(f.rotulo, rotulo, TAM_TOKEN);
  f.numParam = numParam;
  f.parametros = parametros;

  return f;
}

// Retorna struct com atributos de rótulo
struct rotuloAttr rotulo(unsigned int linhaCodigo)
{
  struct rotuloAttr r;
  r.linhaCodigo = linhaCodigo;

  return r;
}

// Insere um simbolo na tabela de simbolos
int insereSimbolo(tabelaSimbolos_t ts, char *ident, attrsSimbolo_t *as)
{
  simbolo_t s;

  memcpy(s.ident, ident, TAM_TOKEN);
  s.attrs = as;

  push(ts, &s, sizeof(simbolo_t));

  return 0;
}

// Retorna struct com atributos de tipo de dados
struct tipoDadoAttr tipoDado(unsigned int tam)
{
  struct tipoDadoAttr td;
  td.tam = tam;

  return td;
}

// Busca atributos de um simbolo
attrsSimbolo_t *buscaSimbolo(tabelaSimbolos_t ts, char *ident)
{
  for (int i = tamPilha(ts) - sizeof(simbolo_t); i >= 0; i -= sizeof(simbolo_t))
  {
    simbolo_t *s = (simbolo_t *) (ts->mem + i);
    int cmp = strncmp(ident, s->ident, TAM_TOKEN);
    if (!cmp) return s->attrs;
  }

  return NULL;
}

// Retira últimos n símbolos da tabela
int retiraSimbolos(tabelaSimbolos_t ts, int n)
{
  for (int i = 0; i < n; i++)
  {
    simbolo_t s;
    int ret = pop(ts, &s, sizeof(simbolo_t));
    if (ret) return ret;
    destroiAttrsSimbolo(s.attrs);
  }

  return 0;
}

// Destroi lista de parametros
void destroiListaParametros(listaParam_t *l)
{
  free(l);
}

// Destroi estruturas do símbolo
void destroiAttrsSimbolo(attrsSimbolo_t *as)
{
  if (as->cat == PROCEDIMENTO && as->procAttr.parametros)
    destroiListaParametros(as->procAttr.parametros);

  if (as->cat == FUNCAO && as->funAttr.parametros)
    destroiListaParametros(as->funAttr.parametros);

  free(as);
}

// Imprime um simbolo na tela
void printSimbolo(char *ident, attrsSimbolo_t* as)
{
  printf("Símbolo: %s\n{\n\tCategoria: %d\n\tNível: %d\n", ident, as->cat, as->nivel);
  switch (as->cat)
  {
    case VAR_SIMPLES:
      printf("\tDeslocamento: %d\n\tTipo: %d\n", as->vsAttr.desloc, as->vsAttr.tipo);
      break;
    case PARAM_FORMAL:
      printf("\tDeslocamento: %d\n\tTipo: %d\n\tPor referência: %d\n", as->pfAttr.desloc, as->pfAttr.tipo, as->pfAttr.porRef);
      break;
    case PROCEDIMENTO:
      printf("\tRótulo: %s\n\tNúmero de parâmetros: %d\n\tParâmetros:\n\t{\n", as->procAttr.rotulo, as->procAttr.numParam);
      for (int i = 0; i < as->procAttr.numParam; i++)
        printf("\t\tTipo: %d\n\t\tPor referência: %d\n", as->procAttr.parametros[i].tipo, as->procAttr.parametros[i].porRef);
      printf("\t}\n");
      break;
    case FUNCAO:
      printf("\tTipo do retorno: %d\n\tRótulo: %s\n\tNúmero de parâmetros: %d\n\tParâmetros:\n\t{\n", as->funAttr.tipoRetorno, as->funAttr.rotulo, as->funAttr.numParam);
      for (int i = 0; i < as->funAttr.numParam; i++)
        printf("\t\tTipo: %d\n\t\tPor referência: %d\n", as->funAttr.parametros[i].tipo, as->funAttr.parametros[i].porRef);
      printf("\t}\n");
      break;
    case ROTULO:
      printf("\tLinha de código: %d\n", as->rotAttr.linhaCodigo);
      break;
    case TIPO_DADO:
      printf("\tTamanho: %d\n", as->tdAttr.tam);
      break;
  }
  printf("}\n");
}

// Imprime a tabela de simbolos
void printTabelaSimbolos(tabelaSimbolos_t ts)
{
  printf(" -- TABELA DE SIMBOLOS --\n");
  for (int i = tamPilha(ts) - sizeof(simbolo_t); i >= 0; i -= sizeof(simbolo_t))
  {
    simbolo_t *s =(simbolo_t *) (ts->mem + i);
    printSimbolo(s->ident, s->attrs);
  }
}
