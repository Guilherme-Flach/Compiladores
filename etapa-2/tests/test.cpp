// #include "../src/main.c"

#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <string>

extern "C" {
int yylex_destroy(void);
int yy_scan_string(const char *);
int yyparse();
}

// The fixture for testing class Foo.
class SyntaxParserTest : public testing::Test {
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  SyntaxParserTest() {
    // You can do set-up work for each test here.
  }

  ~SyntaxParserTest() override { yylex_destroy(); }

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

  void EXPECT_REJECT(std::string input, int lineNo,
                     const std::string unexpectedToken,
                     const std::string expectedToken) {
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    int ret = SyntaxParse(input);
    std::string parserStdOut = testing::internal::GetCapturedStdout();
    std::string parserStdErr = testing::internal::GetCapturedStderr();

    EXPECT_EQ(ret, PARSING_ERROR);
    // StdOut should always be empty.
    EXPECT_EQ(parserStdOut, "");
    // Should tell what token is misplaced in case of error
    const std::string errorMessage =
        "Erro sintático na linha " + std::to_string(lineNo) +
        ": syntax error, unexpected " + unexpectedToken + ", expecting " +
        expectedToken + "\n";

    EXPECT_EQ(parserStdErr, errorMessage);
  }

  void EXPECT_REJECT(std::string input) {
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    int ret = SyntaxParse(input);
    std::string parserStdOut = testing::internal::GetCapturedStdout();
    std::string parserStdErr = testing::internal::GetCapturedStderr();

    EXPECT_EQ(ret, PARSING_ERROR);
    // StdOut should always be empty.
    EXPECT_EQ(parserStdOut, "");
    // StdErr should include an error message
    EXPECT_NE(parserStdErr, "");
  }

  const int PARSING_SUCCESS = 0;
  const int PARSING_ERROR = 1;
};

TEST_F(SyntaxParserTest, AcceptsEmptyProgram) { EXPECT_ACCEPT(""); }

TEST_F(SyntaxParserTest, RejectsEmptyList) {
  EXPECT_REJECT(";", 1, "';'", "end of file");
}

TEST_F(SyntaxParserTest, AcceptsEmptyFunction) {
  EXPECT_ACCEPT("foo -> inteiro := [];");
}

TEST_F(SyntaxParserTest, AcceptsFunctionWithParameters) {
  EXPECT_ACCEPT("foo -> inteiro com bar := inteiro := [];");
}

TEST_F(SyntaxParserTest, AcceptsFunctionWithParametersAndOptionalCom) {
  EXPECT_ACCEPT("foo -> inteiro bar := inteiro := [];");
}

TEST_F(SyntaxParserTest, RejectsFunctionWithNoArrow) {
  EXPECT_REJECT("foo inteiro := [];", 1, "TK_INTEIRO", "TK_SETA");
}

TEST_F(SyntaxParserTest, AcceptsVarDeclaration) {
  EXPECT_ACCEPT("var x := inteiro;");
}

TEST_F(SyntaxParserTest, RejectsVariableInitializationOnCommandList) {
  EXPECT_REJECT("var x := inteiro com 2;", 1, "TK_COM", "';' or ','");
}

TEST_F(SyntaxParserTest, AcceptsMultipleCommands) {
  EXPECT_ACCEPT("var x := inteiro,\n"
                "var y := decimal,\n"
                "z -> inteiro := []\n"
                ";");
}

TEST_F(SyntaxParserTest, AcceptsSimpleProgram) {
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

TEST_F(SyntaxParserTest, RejectsVariableInitializationWithCommand) {
  EXPECT_REJECT("foo -> inteiro := [\n"
                "var bar := decimal com (3+1)\n"
                "];\n");
}

TEST_F(SyntaxParserTest, AcceptsComplexExpression) {
  const int foo = 2 - +-+-2;
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
