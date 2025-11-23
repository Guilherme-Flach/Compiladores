#!/bin/bash
COMMAND=$1
STAGE=$2
OPTION=$3

set -e

STAGE_FOLDER="etapa-$STAGE"
TARGZ_FILE=etapa$STAGE.tgz

case $COMMAND in
  run)
  cd $STAGE_FOLDER
  make
  ./etapa$STAGE
  ;;

  test)
  cd $STAGE_FOLDER
  make test
  bash -c "./tests/test"
  ;;

  visualize)
  cd $STAGE_FOLDER
  make
  rm -f ../test_output.txt
  ./etapa$STAGE < ../test_input.txt > ../test_output.txt 
  xdot ../test_output.txt
  ;;


  clean)
  cd $STAGE_FOLDER
  make clean
  ;;

  export)
  # Create workdir
  rm -rf .tmp
  mkdir .tmp

  # Export relevant files
  cp $STAGE_FOLDER/Makefile .tmp
  cp $STAGE_FOLDER/main.c .tmp
  cp $STAGE_FOLDER/parser.y .tmp
  cp $STAGE_FOLDER/scanner.l .tmp
  cp $STAGE_FOLDER/asd.c .tmp
  cp $STAGE_FOLDER/asd.h .tmp
  cp $STAGE_FOLDER/semantic_checks.c .tmp
  cp $STAGE_FOLDER/semantic_checks.h .tmp
  cp $STAGE_FOLDER/stack_functions.c .tmp
  cp $STAGE_FOLDER/stack_functions.h .tmp
  cp $STAGE_FOLDER/errors.h .tmp
  cp $STAGE_FOLDER/iloc_functions.c .tmp
  cp $STAGE_FOLDER/iloc_functions.h .tmp
  
  cd .tmp
  touch $TARGZ_FILE
  tar cvzf $TARGZ_FILE --exclude=$TARGZ_FILE . 
  # Copy final submission file
  cp $TARGZ_FILE ../submissions/$TARGZ_FILE

  # Test if our export is OK
  rm -rf testDir
  mkdir testDir
  cp $TARGZ_FILE testDir
  cd testDir
  tar -xzf $TARGZ_FILE 
  if [ "$OPTION" == "--run" ];  then
    make
    ./etapa$STAGE
  fi
  ;;

  make-tests)
TEST_DIR="avaliacoes/e${STAGE}_all/E${STAGE}"
CSV_FILE="avaliacoes/e${STAGE}_all/e${STAGE}_CSV_LOG/e${STAGE}_expected.csv"
OUTPUT_FILE="generated_tests.cpp"

if [[ ! -d "$TEST_DIR" ]]; then
    echo "Missing tests directory: $TEST_DIR"
    exit 1
fi

if [[ ! -f "$CSV_FILE" ]]; then
    echo "Missing CSV file: $CSV_FILE"
    exit 1
fi

declare -A EXPECTED_STATUS
declare -A EXPECTED_CODE

# Read CSV: name,status,exit_code (skip blank lines and lines starting with #)
while IFS=',' read -r name status code; do
    [[ -z "${name// }" ]] && continue
    EXPECTED_STATUS["$name"]="$status"
    EXPECTED_CODE["$name"]="$code"
done < <(grep -v -E '^\s*($|#)' "$CSV_FILE" || true)

# Start writing output C++ file
cat > "$OUTPUT_FILE" <<'EOF'
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

EOF

# Loop through test files
for filepath in "$TEST_DIR"/*; do
    [[ -f "$filepath" ]] || continue
    filename=$(basename "$filepath")

    if [[ -z "${EXPECTED_CODE[$filename]:-}" ]]; then
        echo "Warning: No CSV entry for test '$filename', skipping."
        continue
    fi

    expected_status="${EXPECTED_STATUS[$filename]}"
    expected_code="${EXPECTED_CODE[$filename]}"
    
    cat >> "$OUTPUT_FILE" <<EOF

TEST_F(SemanticAnalysisTest, $filename) {
    // Expected: status=${expected_status}, exit_code=${expected_code}
    std::string input = R"GTEST_DELIM(
EOF
        # append the file content literally
        # use sed to print as-is (we already normalized CRs)
        sed -e 's/$/\n/' "$filepath" >> "$OUTPUT_FILE"

        # close the raw literal and add assertion
        cat >> "$OUTPUT_FILE" <<EOF
)GTEST_DELIM";

    EXPECT_RESULT(input, ${expected_code});
}

EOF

done

cat >> "$OUTPUT_FILE" <<"EOF"

} // namespace semantic_analysis_tests

EOF

echo "Generated $OUTPUT_FILE"
echo "Adjust PROGRAM in the script or define PROGRAM macro before compiling if needed."
  ;;

  *)
  echo "Unkown command `$COMMAND`! Available commands are
  run stageNumber
  test stageNumber
  export stageNumber
  visualize stageNumber"
  exit 1
  ;;
esac
