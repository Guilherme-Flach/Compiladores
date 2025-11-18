#include "semantic_analysis_tests.h"

extern "C" {
int yylex_destroy(void);
int yy_scan_string(const char *);
int yyparse();
}

namespace semantic_analysis_tests {
asd_tree_t *arvore = NULL;
stack_node_t *stack = NULL;

int SemanticAnalysisTest::SemanticAnalysis(std::string input) {
  yy_scan_string(input.c_str());
  yyparse();
  exit(0);
};

void SemanticAnalysisTest::EXPECT_RESULT(std::string input, int exit_code) {
  EXPECT_EXIT(SemanticAnalysis(input), ::testing::ExitedWithCode(exit_code),
              ".*");
};

SemanticAnalysisTest::~SemanticAnalysisTest() {
  yylex_destroy();
  asd_free(arvore);
}

TEST_F(SemanticAnalysisTest, qwe00) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  a := 2

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe01) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  a()

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe02) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  var b := inteiro

  a(b)

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe03) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  var b := inteiro

  b := a

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe04) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  b := 2

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe05) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  var c := inteiro

  se (c) [

     b := 2

  ]

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe06) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  var c := inteiro

  se (c) [

  ]senao[

     b := 2

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe07) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  var c := inteiro

  enquanto (c) [

     b := 2

  ]

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe08) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  [

     b := 2

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe09) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
var b := inteiro,

f -> inteiro := [

  [[[[

     b := 2

  ]]]]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe10) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  [

    var b := inteiro

  ]

  b := 2

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe11) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  var c := inteiro

  se (c) [

     var b := inteiro

  ]

  b := 2

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe12) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  var c := inteiro

  se (c) []senao[

     var b := inteiro

  ]

  b := 2

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe13) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  var c := inteiro

  enquanto (c) [

     var b := inteiro

  ]

  b := 2

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe14) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  [

    [

      var b := inteiro

    ]

    b := 2

  ]

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe15) {
  // Expected: status=ERR_UNDECLARED, exit_code=10
  std::string input = R"GTEST_DELIM(
//ERR_UNDECLARED

f -> inteiro := [

  [

   [

    [

     [

      var b := inteiro

     ]

    ]

   ]

   b := 2

  ]

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 10);
}

TEST_F(SemanticAnalysisTest, qwe16) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

var b := inteiro,

f -> inteiro := [],

var b := inteiro;
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe17) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  var b := inteiro

  var b := inteiro  

];

)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe18) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  var c := inteiro

  se (c) [

    var b := inteiro

    var b := inteiro

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe19) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  var c := inteiro

  se (c) []senao[

    var b := inteiro

    var b := inteiro

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe20) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  var c := inteiro

  enquanto (c) [

    var b := inteiro

    var b := inteiro

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe21) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  [

    var b := inteiro

    var b := inteiro

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe22) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [

  [[[[

    var b := inteiro

    []

    var b := inteiro

  ]]]]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe23) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro com a := inteiro, b := inteiro, a := inteiro := [];

)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe24) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro com a := inteiro, b := inteiro := [

  var b := inteiro

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe25) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro com f := inteiro := [];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe26) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro := [

  var f := inteiro

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe27) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [],

f -> inteiro := [];
)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe28) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro := [],

f -> inteiro com a := inteiro := [];

)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe29) {
  // Expected: status=ERR_DECLARED, exit_code=11
  std::string input = R"GTEST_DELIM(
//ERR_DECLARED

f -> inteiro com a := inteiro := [],

f -> inteiro := [];

)GTEST_DELIM";

  EXPECT_RESULT(input, 11);
}

TEST_F(SemanticAnalysisTest, qwe30) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro := [],

g -> inteiro := [

  f()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe31) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro := [

  f()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe32) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro com a := inteiro := [],

g -> inteiro := [

  var b := inteiro

  f(b)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe33) {
  // Expected: status=0, exit_code=0
  std::string input = R"GTEST_DELIM(
f -> inteiro com a := inteiro := [

  f(a)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 0);
}

TEST_F(SemanticAnalysisTest, qwe34) {
  // Expected: status=ERR_VARIABLE, exit_code=20
  std::string input = R"GTEST_DELIM(
//ERR_VARIABLE

var b := inteiro,

f -> inteiro := [

  b()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 20);
}

TEST_F(SemanticAnalysisTest, qwe35) {
  // Expected: status=ERR_VARIABLE, exit_code=20
  std::string input = R"GTEST_DELIM(
//ERR_VARIABLE

f -> inteiro := [

  var b := inteiro

  b()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 20);
}

TEST_F(SemanticAnalysisTest, qwe36) {
  // Expected: status=ERR_VARIABLE, exit_code=20
  std::string input = R"GTEST_DELIM(
//ERR_VARIABLE

var b := inteiro,

f -> inteiro := [

  var a := inteiro

  a := b()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 20);
}

TEST_F(SemanticAnalysisTest, qwe37) {
  // Expected: status=ERR_VARIABLE, exit_code=20
  std::string input = R"GTEST_DELIM(
//ERR_VARIABLE

f -> inteiro := [

  var b := inteiro

  var a := inteiro

  a := b()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 20);
}

TEST_F(SemanticAnalysisTest, qwe38) {
  // Expected: status=ERR_FUNCTION, exit_code=21
  std::string input = R"GTEST_DELIM(
//ERR_FUNCTION

f -> inteiro := [

  f := 2

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 21);
}

TEST_F(SemanticAnalysisTest, qwe39) {
  // Expected: status=ERR_FUNCTION, exit_code=21
  std::string input = R"GTEST_DELIM(
//ERR_FUNCTION

g -> inteiro := [],

f -> inteiro := [

  g := 2

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 21);
}

TEST_F(SemanticAnalysisTest, qwe40) {
  // Expected: status=ERR_FUNCTION, exit_code=21
  std::string input = R"GTEST_DELIM(
//ERR_FUNCTION

g -> inteiro := [],

f -> inteiro := [

  var a := inteiro

  a := g

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 21);
}

TEST_F(SemanticAnalysisTest, qwe41) {
  // Expected: status=ERR_FUNCTION, exit_code=21
  std::string input = R"GTEST_DELIM(
//ERR_FUNCTION

h -> inteiro com a := inteiro := [],

g -> inteiro := [],

f -> inteiro := [

  var a := inteiro

  a := h(g)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 21);
}

TEST_F(SemanticAnalysisTest, qwe42) {
  // Expected: status=ERR_MISSING_ARGS, exit_code=40
  std::string input = R"GTEST_DELIM(
//ERR_MISSING_ARGS

h -> inteiro com a := inteiro := [],

f -> inteiro := [

  h()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 40);
}

TEST_F(SemanticAnalysisTest, qwe43) {
  // Expected: status=ERR_MISSING_ARGS, exit_code=40
  std::string input = R"GTEST_DELIM(
//ERR_MISSING_ARGS

h -> inteiro com a := inteiro, b := inteiro := [],

f -> inteiro := [

  h()

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 40);
}

TEST_F(SemanticAnalysisTest, qwe44) {
  // Expected: status=ERR_EXCESS_ARGS, exit_code=41
  std::string input = R"GTEST_DELIM(
//ERR_EXCESS_ARGS

f -> inteiro := [

  var a := inteiro

  f(a)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 41);
}

TEST_F(SemanticAnalysisTest, qwe45) {
  // Expected: status=ERR_EXCESS_ARGS, exit_code=41
  std::string input = R"GTEST_DELIM(
//ERR_EXCESS_ARGS

f -> inteiro := [

  var a := inteiro

  f(a, a)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 41);
}

TEST_F(SemanticAnalysisTest, qwe46) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := inteiro := [],

f -> inteiro := [

  var b := decimal

  h(b)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe47) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var b := inteiro

  h(b)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe48) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal, b := inteiro := [],

f -> inteiro := [

  var x := decimal

  var y := decimal

  h(x, y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe49) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x+y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe50) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x*y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe51) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x-y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe52) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x/y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe53) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x%y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe54) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  h(-x)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe55) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  h(+x)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe56) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  h(!x)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe57) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x < y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe58) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x > y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe59) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x <= y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe60) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x >= y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe61) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x & y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe62) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x | y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe63) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x == y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe64) {
  // Expected: status=ERR_WRONG_TYPE_ARGS, exit_code=42
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE_ARGS

h -> inteiro com a := decimal := [],

f -> inteiro := [

  var x := inteiro

  var y := inteiro

  h(x != y)

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 42);
}

TEST_F(SemanticAnalysisTest, qwe65) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  retorna a := decimal

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe66) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  retorna a+a := decimal

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe67) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  retorna 2 := decimal

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe68) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  retorna 2.0 := inteiro

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe69) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  retorna 2.0 := decimal

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe70) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  a := 2.0

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe71) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := b+b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe72) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := a+b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe73) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := a*b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe74) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := a<b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe75) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := a==b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe76) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := +b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe77) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var a := inteiro

  var b := decimal

  a := a*2/3+4|a+b

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe78) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var c := inteiro

  se (c) [

    var a := inteiro

    enquanto(a) []

  ]senao[

    var b := decimal

    enquanto(b) []

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe79) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var c := inteiro

  se (c) [

    var a := inteiro

    enquanto(a) []

  ]senao[

    var b := decimal

    se(b) []

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe80) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var c := inteiro

  se (c) [

    var a := inteiro

    enquanto(a) []

  ]senao[

    var b := decimal

    se(b) [ b := 2.0 ] senao [ b := 2.1 ]

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe81) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

f -> inteiro := [

  var c := inteiro

  se (c) [

    var a := inteiro

    a := 2

  ]senao[

    var b := decimal

    b := 2.0

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

TEST_F(SemanticAnalysisTest, qwe82) {
  // Expected: status=ERR_WRONG_TYPE, exit_code=30
  std::string input = R"GTEST_DELIM(
//ERR_WRONG_TYPE

g -> decimal := [],

f -> inteiro := [

  var c := inteiro

  se (c) [

    g()

  ]senao[

    f()

  ]

];
)GTEST_DELIM";

  EXPECT_RESULT(input, 30);
}

} // namespace semantic_analysis_tests
