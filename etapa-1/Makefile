CC=gcc
CFLAGS=-I.
DEPS = tokens.h
OBJ = main.o lex.yy.o

etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

lex.yy.c:
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)



test: test.o	
	g++ out/test.o -o out/test -lgtest -lgtest_main

test.o: 
	g++ -c tests/test.cpp -o out/test.o 


