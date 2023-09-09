 # -------------------------------------------------------------------
 #            Arquivo: Makefile
 # -------------------------------------------------------------------
 #              Autor: Bruno Müller Junior
 #               Data: 08/2007
 #     Modificado por: Henrique Luiz Rieger
 #      Atualizado em: [08/09/2023, 23h:59m]
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

compilador: lex.yy.c compilador.tab.c compilador.o compilador.h pilha.o tabelaSimbolos.o tipoDado.o
	${CC} ${CFLAGS} lex.yy.c compilador.tab.c compilador.o pilha.o tabelaSimbolos.o tipoDado.o -o compilador -ll -ly -lc

lex: lex.yy.c compilador.tab.c compilador.o
	${CC} lex.yy.c -DLEXMAIN compilador.o -o lex -ll

lex.yy.c: compilador.l compilador.h
	flex compilador.l

compilador.tab.c: compilador.y compilador.h
	bison compilador.y -d -v

compilador.o : compilador.h compiladorF.c
	${CC} -c compiladorF.c -o compilador.o

debug: CFLAGS += -g -DDEBUG
debug: all

clean :
	rm -f compilador.tab.* lex.yy.c compilador.o compilador lex
