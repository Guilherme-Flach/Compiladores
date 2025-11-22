#ifndef ILOC_FUNCTIONS_HEADER
#define ILOC_FUNCTIONS_HEADER

#include <stdlib.h>
#include "stack_functions.h"

typedef enum { OPT_NORMAL, OPT_CONTROL } OPERATION_TYPE;
typedef enum {
  // Normal operations
  ILOC_NOP,
  ILOC_ADD,
  ILOC_SUB,
  ILOC_MULT,
  ILOC_DIV,
  ILOC_ADDI,
  ILOC_SUBI,
  ILOC_RSUBI,
  ILOC_MULTI,
  ILOC_DIVI,
  ILOC_RDIVI,
  ILOC_LSHIFT,
  ILOC_LSHIFTI,
  ILOC_RSHIFT,
  ILOC_RSHIFTI,
  ILOC_AND,
  ILOC_ANDI,
  ILOC_OR,
  ILOC_ORI,
  ILOC_XOR,
  ILOC_XORI,
  ILOC_LOADI,
  ILOC_LOAD,
  ILOC_LOADAI,
  ILOC_LOADA0,
  ILOC_CLOAD,
  ILOC_CLOADAI,
  ILOC_CLOADA0,
  ILOC_STORE,
  ILOC_STOREAI,
  ILOC_STOREAO,
  ILOC_CSTORE,
  ILOC_CSTOREAI,
  ILOC_CSTOREAO,
  ILOC_I2I,
  ILOC_C2C,
  ILOC_C2I,
  ILOC_I2C,
  // Control flow
  ILOC_JUMPI,
  ILOC_JUMP,
  ILOC_CBR,
  ILOC_CMP_LT,
  ILOC_CMP_LE,
  ILOC_CMP_EQ,
  ILOC_CMP_GE,
  ILOC_CMP_GT,
  ILOC_CMP_NE
} OPERATION_CODE;

typedef struct {
  OPERATION_TYPE type;
  OPERATION_CODE op_code;
  char *first_operand;
  char *second_operand;
  char *third_operand;

} iloc_operation_t;

typedef struct iloc_operation_list {
  struct iloc_operation_list *next;
  iloc_operation_t *current_operation;

} iloc_operation_list_t;

OPERATION_TYPE get_operation_type(OPERATION_CODE op_code);

iloc_operation_t *make_operation(OPERATION_CODE op_code);

// Lists here have their last node point to an empty (but allocated!) node. This
// makes them easier to use inside the parser, but slightly trickier to handle
// in the code here (similar to the symbol tables).
iloc_operation_list_t *make_operation_list_node();

void add_operation_to_list(iloc_operation_list_t *list,
                           iloc_operation_t *operation);

// Makes the end of the first list point to the start of the second and returns
// a pointer to the start of the concatenated list (covers the cases when the
// first list may be empty)
iloc_operation_list_t *concat_operation_lists(iloc_operation_list_t *first,
                                              iloc_operation_list_t *second);

void destroy_operand(char *operand);

void destroy_operation(iloc_operation_t *operation);

void destroy_operation_list(iloc_operation_list_t *list);

void print_operation_by_code(OPERATION_CODE op_code, char *op1, char *op2,
                             char *op3);

void print_operation(iloc_operation_t *operation);

void print_operation_list(iloc_operation_list_t *list);

char *generate_temp_register();
char *generate_label();
iloc_operation_list_t *generate_load_variable(symbol_table_entry *entry, char **result_reg_ptr);
iloc_operation_list_t *generate_store_variable(symbol_table_entry *entry, iloc_operation_list_t *expr_code, char *source_reg);

iloc_operation_list_t *generate_binary_operation(OPERATION_CODE op_code,
  iloc_operation_list_t *code_left,
  char *reg_left,
  iloc_operation_list_t *code_right,
  char *reg_right,
  char **result_reg_ptr);

iloc_operation_list_t *generate_unary_operation(OPERATION_CODE op_code,
  iloc_operation_list_t *code_expr,
  char *reg_expr,
  char **result_reg_ptr);

iloc_operation_list_t *generate_while_code(iloc_operation_list_t *code_test, char *reg_test, iloc_operation_list_t *code_body);
iloc_operation_list_t *generate_if_else_code(iloc_operation_list_t *code_test, char *reg_test, iloc_operation_list_t *code_if, iloc_operation_list_t *code_else);
iloc_operation_list_t *generate_return_code(iloc_operation_list_t *code_expr, char *reg_expr);
iloc_operation_list_t *generate_program_startup_shutdown(iloc_operation_list_t *program_code);

#endif