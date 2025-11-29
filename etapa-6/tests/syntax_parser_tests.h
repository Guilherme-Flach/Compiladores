#ifndef SYNTAX_PARSER_TESTS_H
#define SYNTAX_PARSER_TESTS_H

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

namespace syntax_parser_tests {

// The fixture for testing class Foo.
class SyntaxParserTest : public testing::Test {
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  SyntaxParserTest() {
    // You can do set-up work for each test here.
  }

  ~SyntaxParserTest() override;

  // Class members declared here can be used by all tests in the test suite
  // for CompilerTest.

protected:
  int SyntaxParse(std::string input);

  void EXPECT_ACCEPT(std::string input);

  const int PARSING_SUCCESS = 0;
  const int PARSING_ERROR = 1;
};

} // namespace syntax_parser_tests
#endif