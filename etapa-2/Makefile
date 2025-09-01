CC=gcc
CFLAGS=-I.
DEPS = parser.tab.h
OBJ = main.o lex.yy.o parser.tab.o

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)	

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: test.o	
	g++ tests/test.o -o tests/test -lgtest -lgtest_main

test.o: 
	g++ -c tests/test.cpp -o tests/test.o 


