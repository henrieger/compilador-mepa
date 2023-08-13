 # -------------------------------------------------------------------
 #            Arquivo: Makefile
 # -------------------------------------------------------------------
 #              Autor: Bruno Müller Junior
 #               Data: 08/2007
 #     Modificado por: Henrique Luiz Rieger
 #      Atualizado em: [12/08/2023, 16h:21m]
 #
 # -------------------------------------------------------------------

$DEPURA=1

compilador: lex.yy.c compilador.tab.c compilador.o compilador.h
	gcc lex.yy.c compilador.tab.c compilador.o -o compilador -ll -ly -lc

lex: lex.yy.c compilador.tab.c compilador.o
	gcc lex.yy.c -DLEXMAIN compilador.o -o lex -ll

lex.yy.c: compilador.l compilador.h
	flex compilador.l

compilador.tab.c: compilador.y compilador.h
	bison compilador.y -d -v

compilador.o : compilador.h compiladorF.c
	gcc -c compiladorF.c -o compilador.o

clean :
	rm -f compilador.tab.* lex.yy.c compilador.o compilador lex
