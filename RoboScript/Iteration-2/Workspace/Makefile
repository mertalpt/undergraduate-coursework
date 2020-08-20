all: yacc
	gcc -o parser.exe y.tab.c

yacc: lex
	yacc parser.y

lex:
	lex lexer.l

verbose: lex
	yacc -v parser.y

clean:
	rm lex.yy.c && rm y.tab.c && rm y.output && rm parser.exe

test: all
	./parser.exe < exampleProgram.txt
