#ifndef SEMANTIC_ANALYSIS_TESTS_H
#define SEMANTIC_ANALYSIS_TESTS_H

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

#define SUCCESS 0
#define ERR_UNDECLARED 10      // 2.2
#define ERR_DECLARED 11        // 2.2
#define ERR_VARIABLE 20        // 2.3
#define ERR_FUNCTION 21        // 2.3
#define ERR_WRONG_TYPE 30      // 2.4
#define ERR_MISSING_ARGS 40    // 2.5
#define ERR_EXCESS_ARGS 41     // 2.5
#define ERR_WRONG_TYPE_ARGS 42 // 2.5

namespace semantic_analysis_tests {

// The fixture for testing class Foo.
class SemanticAnalysisTest : public testing::Test {
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  SemanticAnalysisTest() {};

  ~SemanticAnalysisTest();

  int SemanticAnalysis(std::string input);

  void EXPECT_RESULT(std::string input, int exit_code);
};

} // namespace semantic_analysis_tests
#endif