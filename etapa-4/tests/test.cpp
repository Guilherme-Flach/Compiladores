// #include "../src/main.c"

#include "gtest/gtest.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <string>

extern "C" {
int yylex_destroy(void);
int yy_scan_string(const char *);
int yyparse();

#include "../asd.h"
#include "../stack_functions.h"
}

asd_tree_t *arvore = NULL;
// The fixture for testing class Foo.
class SyntaxParserTest : public testing::Test {
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  SyntaxParserTest() {
    // You can do set-up work for each test here.
  }

  ~SyntaxParserTest() override {
    yylex_destroy();
    asd_free(arvore);
  }

  // Class members declared here can be used by all tests in the test suite
  // for CompilerTest.

protected:
  int SyntaxParse(std::string input) {
    yy_scan_string(input.c_str());
    int ret = yyparse();
    return ret;
  }

  void EXPECT_ACCEPT(std::string input) {
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    int ret = SyntaxParse(input);
    std::string parserStdOut = testing::internal::GetCapturedStdout();
    std::string parserStdErr = testing::internal::GetCapturedStderr();

    EXPECT_EQ(ret, PARSING_SUCCESS);
    // StdOut should always be empty.
    EXPECT_EQ(parserStdOut, "");
    // Nothing on stdError in case of success
    EXPECT_EQ(parserStdErr, "");
  }

  const int PARSING_SUCCESS = 0;
  const int PARSING_ERROR = 1;
};

class SemanticToolsTest : public testing::Test {};

TEST_F(SyntaxParserTest, AcceptsEmptyProgram) { EXPECT_ACCEPT(""); }

TEST_F(SyntaxParserTest, AcceptsEmptyFunction) {
  EXPECT_ACCEPT("foo -> inteiro := [];");
}

TEST_F(SyntaxParserTest, AcceptsFunctionWithParameters) {
  EXPECT_ACCEPT("foo -> inteiro com bar := inteiro := [];");
}

TEST_F(SyntaxParserTest, AcceptsFunctionWithParametersAndOptionalCom) {
  EXPECT_ACCEPT("foo -> inteiro bar := inteiro := [];");
}

TEST_F(SyntaxParserTest, AcceptsVarDeclaration) {
  EXPECT_ACCEPT("var x := inteiro;");
}

TEST_F(SyntaxParserTest, AcceptsMultipleCommands) {
  EXPECT_ACCEPT("var x := inteiro,\n"
                "var y := decimal,\n"
                "z -> inteiro := []\n"
                ";");
}

TEST_F(SyntaxParserTest, AcceptsSimpleProgram) {
  // main->inteiro := [ var y := decimal com 3.14 retorna y := decimal ];

  EXPECT_ACCEPT("main -> inteiro := [\n"
                "var y := decimal com 3.14\n"
                "retorna y := decimal\n"
                "];");
}

TEST_F(SyntaxParserTest, AcceptsComplexFunction) {
  EXPECT_ACCEPT("foo -> decimal com\n"
                "  bar := decimal,\n"
                "  baz := inteiro,\n"
                "  qux := inteiro\n"
                ":= [\n"
                "  var fred := inteiro\n"
                "  fred := +(bar + baz - qux) / -(qux * bar)\n"
                "  barney := fred % bar\n"
                "  se (!(fred + barney > bar)) [\n"
                "    retorna fred := inteiro\n"
                "  ]\n"
                "  senao [\n"
                "    enquanto (fred > barney | fred >= bar & fred < qux &\n"
                "             fred <= baz) [\n"
                "      fred := fred + 1\n"
                "    ]\n"
                "    retorna foo(fred + 1, barney /2, qux * 3) := inteiro\n"
                "  ]\n"
                "]\n"
                ";");
}

TEST_F(SyntaxParserTest, AcceptsComplexExpression) {
  EXPECT_ACCEPT("foo -> inteiro := [\n"
                "var bar := decimal\n"
                "bar := !(bar + foo)\n"
                "bar := 1++1\n"
                "bar := 1+-1\n"
                "bar := 1--1\n"
                "bar := 1*-1\n"
                "bar := 1*+1\n"
                "bar := 1/+1\n"
                "bar := 1/+1\n"
                "bar := 1/+1\n"
                "bar := 1+-+-1\n"
                "bar := !!1\n"
                "];\n");
}

TEST_F(SemanticToolsTest, PushPopWorks) {
  stack_node_t *stack = NULL;
  symbol_table_t *table = make_symbol_table_node();

  symbol_table_entry *s1 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"foo");
  argument *s1_a1 = make_argument((char *)"fred", SYMBOL_TYPE::S_INTEGER);
  append_argument_to_symbol(s1, s1_a1);
  append_symbol_to_table(table, s1);
  stack = push_symbol_table(stack, table);

  symbol_table_entry *s1_pointer = stack->table_contents->symbol;

  // Check insertion of symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s1_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s1_pointer->type);
  EXPECT_STREQ("foo", s1_pointer->value);

  // Check insertion of symbol arguments
  EXPECT_STREQ("fred", s1_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s1_pointer->arguments->arg->type);

  stack = pop_symbol_table(stack);
  EXPECT_EQ(nullptr, stack);
}

TEST_F(SemanticToolsTest, FindSymbolWorks) {
  stack_node_t *stack = NULL;
  symbol_table_t *table = make_symbol_table_node();

  symbol_table_entry *s1 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"foo");
  argument *s1_a1 = make_argument((char *)"fred", SYMBOL_TYPE::S_INTEGER);
  append_argument_to_symbol(s1, s1_a1);
  append_symbol_to_table(table, s1);
  stack = push_symbol_table(stack, table);

  symbol_table_entry *s1_pointer = find_symbol((char *)"foo", stack);
  EXPECT_NE(nullptr, s1_pointer);

  // Check fetching of symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s1_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s1_pointer->type);
  EXPECT_STREQ("foo", s1_pointer->value);

  // Check fetching of symbol arguments
  EXPECT_STREQ("fred", s1_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s1_pointer->arguments->arg->type);

  stack = pop_symbol_table(stack);
  EXPECT_EQ(nullptr, stack);
}

TEST_F(SemanticToolsTest, AcceptsMultipleSymbolsAndArgs) {
  stack_node_t *stack = NULL;
  symbol_table_t *table = make_symbol_table_node();

  // Symbol 1
  symbol_table_entry *s1 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"foo");
  argument *s1_a1 = make_argument((char *)"fred", SYMBOL_TYPE::S_INTEGER);
  argument *s1_a2 = make_argument((char *)"barney", SYMBOL_TYPE::S_FLOAT);

  // Add arguments first and then append
  append_symbol_to_table(table, s1);

  append_argument_to_symbol(s1, s1_a1);
  append_argument_to_symbol(s1, s1_a2);

  // Symbol 2
  symbol_table_entry *s2 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"bar");
  argument *s2_a1 = make_argument((char *)"fizz", SYMBOL_TYPE::S_INTEGER);
  argument *s2_a2 = make_argument((char *)"buzz", SYMBOL_TYPE::S_FLOAT);

  // Append first and then add arguments
  append_symbol_to_table(table, s2);

  append_argument_to_symbol(s2, s2_a1);
  append_argument_to_symbol(s2, s2_a2);

  // Push table to stack
  stack = push_symbol_table(stack, table);

  symbol_table_entry *s2_pointer = find_symbol((char *)"bar", stack);
  EXPECT_NE(nullptr, s2_pointer);

  // Check fetching of second symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s2_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->type);
  EXPECT_STREQ("bar", s2_pointer->value);

  // Check fetching of second symbol arguments
  EXPECT_STREQ("fizz", s2_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s2_pointer->arguments->arg->type);
  EXPECT_STREQ("buzz", s2_pointer->arguments->next_arg->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->arguments->next_arg->arg->type);

  stack = pop_symbol_table(stack);
  EXPECT_EQ(nullptr, stack);
}

TEST_F(SemanticToolsTest, AcceptsMultipleTables) {
  stack_node_t *stack = NULL;
  symbol_table_t *table1 = make_symbol_table_node();

  // Symbol 1
  symbol_table_entry *s1 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"foo");
  argument *s1_a1 = make_argument((char *)"fred", SYMBOL_TYPE::S_INTEGER);
  argument *s1_a2 = make_argument((char *)"barney", SYMBOL_TYPE::S_FLOAT);

  // Add arguments first and then append
  append_symbol_to_table(table1, s1);

  append_argument_to_symbol(s1, s1_a1);
  append_argument_to_symbol(s1, s1_a2);

  // Add symbols first then append
  stack = push_symbol_table(stack, table1);

  symbol_table_t *table2 = make_symbol_table_node();

  // Symbol 2
  symbol_table_entry *s2 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"bar");
  argument *s2_a1 = make_argument((char *)"fizz", SYMBOL_TYPE::S_INTEGER);
  argument *s2_a2 = make_argument((char *)"buzz", SYMBOL_TYPE::S_FLOAT);

  // Push first and then add symbols
  stack = push_symbol_table(stack, table2);

  // Append first and then add arguments
  append_symbol_to_table(table2, s2);

  append_argument_to_symbol(s2, s2_a1);
  append_argument_to_symbol(s2, s2_a2);

  symbol_table_entry *s1_pointer = find_symbol((char *)"foo", stack);
  EXPECT_NE(nullptr, s1_pointer);

  // Check fetching of first symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s1_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s1_pointer->type);
  EXPECT_STREQ("foo", s1_pointer->value);

  // Check fetching of first symbol arguments
  EXPECT_STREQ("fred", s1_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s1_pointer->arguments->arg->type);

  symbol_table_entry *s2_pointer = find_symbol((char *)"bar", stack);

  EXPECT_NE(nullptr, s2_pointer);

  // Check fetching of second symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s2_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->type);
  EXPECT_STREQ("bar", s2_pointer->value);

  // Check fetching of second symbol arguments
  EXPECT_STREQ("fizz", s2_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s2_pointer->arguments->arg->type);
  EXPECT_STREQ("buzz", s2_pointer->arguments->next_arg->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->arguments->next_arg->arg->type);

  stack = pop_symbol_table(stack);
  stack = pop_symbol_table(stack);
  EXPECT_EQ(nullptr, stack);
}

TEST_F(SemanticToolsTest, AcceptsMultipleTablesWithAlternatePushPop) {
  stack_node_t *stack = NULL;
  symbol_table_t *table1 = make_symbol_table_node();

  // Symbol 1
  symbol_table_entry *s1 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"foo");
  argument *s1_a1 = make_argument((char *)"fred", SYMBOL_TYPE::S_INTEGER);
  argument *s1_a2 = make_argument((char *)"barney", SYMBOL_TYPE::S_FLOAT);

  // Add arguments first and then append
  append_symbol_to_table(table1, s1);

  append_argument_to_symbol(s1, s1_a1);
  append_argument_to_symbol(s1, s1_a2);

  // Add symbols first then append
  stack = push_symbol_table(stack, table1);

  symbol_table_t *table2 = make_symbol_table_node();

  // Symbol 2
  symbol_table_entry *s2 = make_symbol_table_entry(
      SYMBOL_NATURE::S_LITERAL, SYMBOL_TYPE::S_FLOAT, (char *)"bar");
  argument *s2_a1 = make_argument((char *)"fizz", SYMBOL_TYPE::S_INTEGER);
  argument *s2_a2 = make_argument((char *)"buzz", SYMBOL_TYPE::S_FLOAT);

  // Push first and then add symbols
  stack = push_symbol_table(stack, table2);

  // Append first and then add arguments
  append_symbol_to_table(table2, s2);

  append_argument_to_symbol(s2, s2_a1);
  append_argument_to_symbol(s2, s2_a2);

  symbol_table_entry *s2_pointer = find_symbol((char *)"bar", stack);

  EXPECT_NE(nullptr, s2_pointer);

  // Check fetching of second symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s2_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->type);
  EXPECT_STREQ("bar", s2_pointer->value);

  // Check fetching of second symbol arguments
  EXPECT_STREQ("fizz", s2_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s2_pointer->arguments->arg->type);
  EXPECT_STREQ("buzz", s2_pointer->arguments->next_arg->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s2_pointer->arguments->next_arg->arg->type);

  stack = pop_symbol_table(stack);

  symbol_table_entry *s1_pointer = find_symbol((char *)"foo", stack);
  EXPECT_NE(nullptr, s1_pointer);

  // Can't find s2 anymore (it was popped!)
  s2_pointer = find_symbol((char *)"bar", stack);
  EXPECT_EQ(NULL, s2_pointer);

  // Check fetching of first symbol
  EXPECT_EQ(SYMBOL_NATURE::S_LITERAL, s1_pointer->nature);
  EXPECT_EQ(SYMBOL_TYPE::S_FLOAT, s1_pointer->type);
  EXPECT_STREQ("foo", s1_pointer->value);

  // Check fetching of first symbol arguments
  EXPECT_STREQ("fred", s1_pointer->arguments->arg->name);
  EXPECT_EQ(SYMBOL_TYPE::S_INTEGER, s1_pointer->arguments->arg->type);

  stack = pop_symbol_table(stack);
  EXPECT_EQ(nullptr, stack);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
