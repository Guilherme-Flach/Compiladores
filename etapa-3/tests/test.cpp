// #include "../src/main.c"

#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <string>

extern "C" {
int yylex_destroy(void);
int yy_scan_string(const char *);
int yyparse();

#include "../asd.h"
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

  // TODO: Memory leak aqui
  EXPECT_ACCEPT("main -> inteiro := [\n"
                "var y := decimal com 3.14\n"
                "retorna y := decimal\n"
                "];");
}

TEST_F(SyntaxParserTest, AcceptsComplexFunction) {
  // TODO: Memory leak aqui
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
  // TODO: Memory leak aqui
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

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
