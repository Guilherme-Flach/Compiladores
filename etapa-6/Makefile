# 00343210 - Eduarda Tessari Pereira
# 00342108 - Guilherme Ismael Flach

CC=gcc
CXX=g++
CFLAGS=-fsanitize=address -I. -Wall -Werror -g
CXXFLAGS=-fsanitize=address -I. -Wall -Werror -g -lgtest -lgtest_main
DEPS=parser.tab.h parser.tab.c lex.yy.c 
COMMON_OBJ=lex.yy.o parser.tab.o asd.o stack_functions.o semantic_checks.o iloc_functions.o

OBJ=main.o $(COMMON_OBJ)
TEST_OBJ=tests/test.o tests/syntax_parser_tests.o tests/semantic_tools_tests.o tests/semantic_analysis_tests.o $(COMMON_OBJ)

etapa6: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

parser.tab.c parser.tab.h: parser.y
	bison -d --report=all --report-file=parser.output parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) $(DEPS) etapa6

test: $(TEST_OBJ)
	$(CXX) $^ -o tests/$@  $(CXXFLAGS)

tests/%.o: tests/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)