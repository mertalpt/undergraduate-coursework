all: yacc
	gcc -o parser.exe y.tab.c

yacc: lex
	yacc CS315f18_group50.yacc

lex:
	lex CS315f18_group50.lex

verbose: lex
	yacc -v CS315f18_group50.yacc

clean:
	rm lex.yy.c && rm y.tab.c && rm parser.exe

test: all
	./parser.exe < CS315f18_group50.test
