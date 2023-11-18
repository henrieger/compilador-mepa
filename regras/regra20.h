# ifndef __REGRA_20__
# define __REGRA_20__

// Checa se símbolo é procedimento ou função e cria AMEM para função
void checaProcedimentoOuFuncao();

// Gera código da chamada de procedimento baseado no último ident empilhado
void chamaProcedimento();

// Gera código da chamada de função baseado no último ident empilhado
void chamaFuncao();

# endif
