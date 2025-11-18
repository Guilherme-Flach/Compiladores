// #include "../src/main.c"

#include "syntax_parser_tests.h"

namespace syntax_parser_tests {

asd_tree_t *arvore = NULL;
stack_node_t *stack = NULL;

SyntaxParserTest::~SyntaxParserTest() {
  yylex_destroy();
  asd_free(arvore);
}
int SyntaxParserTest::SyntaxParse(std::string input) {
  yy_scan_string(input.c_str());
  int ret = yyparse();
  return ret;
}
void SyntaxParserTest::EXPECT_ACCEPT(std::string input) {
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

// TEST_F(SyntaxParserTest, AcceptsEmptyProgram) { EXPECT_ACCEPT(""); }

// TEST_F(SyntaxParserTest, AcceptsEmptyFunction) {
//   EXPECT_ACCEPT("foo -> inteiro := [];");
// }

// TEST_F(SyntaxParserTest, AcceptsFunctionWithParameters) {
//   EXPECT_ACCEPT("foo -> inteiro com bar := inteiro := [];");
// }

// TEST_F(SyntaxParserTest, AcceptsFunctionWithParametersAndOptionalCom) {
//   EXPECT_ACCEPT("foo -> inteiro bar := inteiro := [];");
// }

// TEST_F(SyntaxParserTest, AcceptsVarDeclaration) {
//   EXPECT_ACCEPT("var x := inteiro;");
// }

// TEST_F(SyntaxParserTest, AcceptsMultipleCommands) {
//   EXPECT_ACCEPT("var x := inteiro,\n"
//                 "var y := decimal,\n"
//                 "z -> inteiro := []\n"
//                 ";");
// }

// TEST_F(SyntaxParserTest, AcceptsSimpleProgram) {
//   // main->inteiro := [ var y := decimal com 3.14 retorna y := decimal ];

//   EXPECT_ACCEPT("main -> inteiro := [\n"
//                 "var y := decimal com 3.14\n"
//                 "retorna y := decimal\n"
//                 "];");
// }

// TEST_F(SyntaxParserTest, AcceptsComplexFunction) {
//   EXPECT_ACCEPT("foo -> decimal com\n"
//                 "  bar := decimal,\n"
//                 "  baz := inteiro,\n"
//                 "  qux := inteiro\n"
//                 ":= [\n"
//                 "  var fred := inteiro\n"
//                 "  fred := +(bar + baz - qux) / -(qux * bar)\n"
//                 "  barney := fred % bar\n"
//                 "  se (!(fred + barney > bar)) [\n"
//                 "    retorna fred := inteiro\n"
//                 "  ]\n"
//                 "  senao [\n"
//                 "    enquanto (fred > barney | fred >= bar & fred < qux &\n"
//                 "             fred <= baz) [\n"
//                 "      fred := fred + 1\n"
//                 "    ]\n"
//                 "    retorna foo(fred + 1, barney /2, qux * 3) := inteiro\n"
//                 "  ]\n"
//                 "]\n"
//                 ";");
// }

// TEST_F(SyntaxParserTest, AcceptsComplexExpression) {
//   EXPECT_ACCEPT("foo -> inteiro := [\n"
//                 "var bar := decimal\n"
//                 "bar := !(bar + foo)\n"
//                 "bar := 1++1\n"
//                 "bar := 1+-1\n"
//                 "bar := 1--1\n"
//                 "bar := 1*-1\n"
//                 "bar := 1*+1\n"
//                 "bar := 1/+1\n"
//                 "bar := 1/+1\n"
//                 "bar := 1/+1\n"
//                 "bar := 1+-+-1\n"
//                 "bar := !!1\n"
//                 "];\n");
// }

} // namespace syntax_parser_tests