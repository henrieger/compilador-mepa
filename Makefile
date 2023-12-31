 # -------------------------------------------------------------------
 #            Arquivo: Makefile
 # -------------------------------------------------------------------
 #              Autor: Bruno Müller Junior
 #               Data: 08/2007
 #     Modificado por: Henrique Luiz Rieger
 #      Atualizado em: [18/10/2023, 14h:24m]
 #
 # -------------------------------------------------------------------

CC = gcc
CFLAGS = 
# LDLIBS = -ll -ly -lc

all: compilador

testa-pilha: pilha.o
pilha.o: pilha.c pilha.h

testa-tabela-simbolos: CFLAGS += -g -DDEBUG
testa-tabela-simbolos: testa-tabela-simbolos.c tabelaSimbolos.o pilha.o tipoDado.o
tabelaSimbolos.o: tabelaSimbolos.c tabelaSimbolos.h
tipoDado.o: tipoDado.c tipoDado.h
rotulos.o: rotulos.c rotulos.h

compilador: lex.yy.c compilador.tab.c compilador.o regras.o compilador.h pilha.o tabelaSimbolos.o tipoDado.o rotulos.o
	${CC} ${CFLAGS} lex.yy.c compilador.tab.c compilador.o regra*.o pilha.o tabelaSimbolos.o tipoDado.o rotulos.o -o compilador -ll -ly -lc

lex: lex.yy.c compilador.tab.c compilador.o
	${CC} lex.yy.c -DLEXMAIN compilador.o -o lex -ll

lex.yy.c: compilador.l compilador.h
	flex compilador.l

compilador.tab.c: compilador.y compilador.h
	bison compilador.y -d -v

compilador.o : compilador.h compiladorF.c
	${CC} ${CFLAGS} -c compiladorF.c -o compilador.o

regras.o: regras.h
	${CC} ${CFLAGS} -c regras/*.c

debug: CFLAGS += -g -DDEBUG
debug: all

clean :
	rm -f compilador.tab.* lex.yy.c *.o compilador lex testa-pilha testa-tabela-simbolos compilador.output
