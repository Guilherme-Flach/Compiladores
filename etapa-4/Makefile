# 00343210 - Eduarda Tessari Pereira
# 00342108 - Guilherme Ismael Flach

CC=gcc
CXX=g++
CFLAGS=-fsanitize=address -I. -Wall -Werror -g
CXXFLAGS=-fsanitize=address -I. -Wall -Werror -g -lgtest -lgtest_main
DEPS=parser.tab.h parser.tab.c lex.yy.c 
OBJ=main.o lex.yy.o parser.tab.o asd.o stack_functions.o

etapa4: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

parser.tab.c parser.tab.h: parser.y
	bison -d --report=all --report-file=parser.output parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) $(DEPS) etapa3

test: tests/test.o lex.yy.o parser.tab.o asd.o stack_functions.o
	$(CXX) $^ -o tests/$@  $(CXXFLAGS)

tests/test.o: tests/test.cpp
	$(CXX) -c tests/test.cpp -o $@ $(CXXFLAGS)