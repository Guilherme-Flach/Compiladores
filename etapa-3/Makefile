# 00343210 - Eduarda Tessari Pereira
# 00342108 - Guilherme Ismael Flach

CC=gcc
CFLAGS=-I. -Wall -Werror -fsanitize=address
DEPS=parser.tab.h parser.tab.c lex.yy.c
OBJ=main.o lex.yy.o parser.tab.o

etapa3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) $(DEPS) etapa3

test: tests/test.o lex.yy.o parser.tab.o
	g++ $^ -o tests/$@ -lgtest -lgtest_main

tests/test.o: tests/test.cpp
	g++ -c tests/test.cpp -o $@