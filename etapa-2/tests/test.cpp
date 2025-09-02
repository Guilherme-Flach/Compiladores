// #include "../src/main.c"

#include <gtest/gtest.h>

extern "C" {
   int yylex_destroy(void);
   int yy_scan_string(const char*);
   int yyparse();
}

// The fixture for testing class Foo.
class CompilerTest : public testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  CompilerTest() {
     // You can do set-up work for each test here.
  }

  ~CompilerTest() override {
      yylex_destroy();
  }


  // Class members declared here can be used by all tests in the test suite
  // for CompilerTest.

   protected:  
      int SyntaxParse(std::string input) {
         yy_scan_string(input.c_str());
         int ret = yyparse();
         return ret;
      }

      const int PARSING_SUCCESS = 0;
      const int PARSING_ERROR = 1;
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(CompilerTest, CorrectlyParsesEmpty) {
  const std::string input = "this/package/testdata/myinputfile.dat";
  
  int ret = SyntaxParse(input);

  EXPECT_EQ(ret, PARSING_SUCCESS);
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}