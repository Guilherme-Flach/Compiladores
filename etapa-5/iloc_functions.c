#include "iloc_functions.h"
#include <stdio.h>

OPERATION_TYPE get_operation_type(OPERATION_CODE op_code) {
  switch (op_code) {
  case ILOC_NOP:
  case ILOC_ADD:
  case ILOC_SUB:
  case ILOC_MULT:
  case ILOC_DIV:
  case ILOC_ADDI:
  case ILOC_SUBI:
  case ILOC_RSUBI:
  case ILOC_MULTI:
  case ILOC_DIVI:
  case ILOC_RDIVI:
  case ILOC_LSHIFT:
  case ILOC_LSHIFTI:
  case ILOC_RSHIFT:
  case ILOC_RSHIFTI:
  case ILOC_AND:
  case ILOC_ANDI:
  case ILOC_OR:
  case ILOC_ORI:
  case ILOC_XOR:
  case ILOC_XORI:
  case ILOC_LOADI:
  case ILOC_LOAD:
  case ILOC_LOADAI:
  case ILOC_LOADA0:
  case ILOC_CLOAD:
  case ILOC_CLOADAI:
  case ILOC_CLOADA0:
  case ILOC_STORE:
  case ILOC_STOREAI:
  case ILOC_STOREAO:
  case ILOC_CSTORE:
  case ILOC_CSTOREAI:
  case ILOC_CSTOREAO:
  case ILOC_I2I:
  case ILOC_C2C:
  case ILOC_C2I:
  case ILOC_I2C:
    return OPT_NORMAL;

  case ILOC_JUMPI:
  case ILOC_JUMP:
  case ILOC_CBR:
  case ILOC_CMP_LT:
  case ILOC_CMP_LE:
  case ILOC_CMP_EQ:
  case ILOC_CMP_GE:
  case ILOC_CMP_GT:
  case ILOC_CMP_NE:
  default:
    return OPT_CONTROL;
  }
}

void print_operation_by_code(OPERATION_CODE op_code, char *op1, char *op2,
                             char *op3) {
  switch (op_code) {
  case ILOC_NOP:
    printf("nop");
    break;
  case ILOC_ADD:
    printf("add %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_SUB:
    printf("sub %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_MULT:
    printf("mult %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_DIV:
    printf("div %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_ADDI:
    printf("addI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_SUBI:
    printf("subI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_RSUBI:
    printf("rsubI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_MULTI:
    printf("multI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_DIVI:
    printf("divI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_RDIVI:
    printf("rdivI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_LSHIFT:
    printf("lshift %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_LSHIFTI:
    printf("lshiftI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_RSHIFT:
    printf("rshift %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_RSHIFTI:
    printf("rshiftI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_AND:
    printf("and %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_ANDI:
    printf("andI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_OR:
    printf("or %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_ORI:
    printf("orI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_XOR:
    printf("xor %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_XORI:
    printf("xorI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_LOADI:
    printf("loadI %s => %s", op1, op2);
    break;
  case ILOC_LOAD:
    printf("load %s => %s", op1, op2);
    break;
  case ILOC_LOADAI:
    printf("loadAI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_LOADA0:
    printf("loadA0 %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_CLOAD:
    printf("cload %s => %s", op1, op2);
    break;
  case ILOC_CLOADAI:
    printf("cloadAI %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_CLOADA0:
    printf("cloadA0 %s, %s => %s", op1, op2, op3);
    break;
  case ILOC_STORE:
    printf("store %s => %s", op1, op2);
    break;
  case ILOC_STOREAI:
    printf("storeAI %s => %s, %s", op1, op2, op3);
    break;
  case ILOC_STOREAO:
    printf("storeAO %s => %s, %s", op1, op2, op3);
    break;
  case ILOC_CSTORE:
    printf("cstore %s => %s", op1, op2);
    break;
  case ILOC_CSTOREAI:
    printf("cstoreAI %s => %s, %s", op1, op2, op3);
    break;
  case ILOC_CSTOREAO:
    printf("cstoreAO %s => %s, %s", op1, op2, op3);
    break;
  case ILOC_I2I:
    printf("i2i %s => %s", op1, op2);
    break;
  case ILOC_C2C:
    printf("c2c %s => %s", op1, op2);
    break;
  case ILOC_C2I:
    printf("c2i %s => %s", op1, op2);
    break;
  case ILOC_I2C:
    printf("i2c %s => %s", op1, op2);
    break;
  case ILOC_JUMPI:
    printf("jumpI -> %s", op1);
    break;
  case ILOC_JUMP:
    printf("jump -> %s", op1);
    break;
  case ILOC_CBR:
    printf("cbr %s -> %s, %s", op1, op2, op3);
    break;
  case ILOC_CMP_LT:
    printf("cmp_LT %s, %s -> %s", op1, op2, op3);
    break;
  case ILOC_CMP_LE:
    printf("cmp_LE %s, %s -> %s", op1, op2, op3);
    break;
  case ILOC_CMP_EQ:
    printf("cmp_EQ %s, %s -> %s", op1, op2, op3);
    break;
  case ILOC_CMP_GE:
    printf("cmp_GE %s, %s -> %s", op1, op2, op3);
    break;
  case ILOC_CMP_GT:
    printf("cmp_GT %s, %s -> %s", op1, op2, op3);
    break;
  case ILOC_CMP_NE:
    printf("cmp_NE %s, %s -> %s", op1, op2, op3);
    break;
  };
}

iloc_operation_t *make_operation(OPERATION_CODE op_code) {
  OPERATION_TYPE op_type = get_operation_type(op_code);

  iloc_operation_t *new_op =
      (iloc_operation_t *)malloc(sizeof(iloc_operation_t));

  new_op->op_code = op_code;
  new_op->type = op_type;

  new_op->first_operand = NULL;
  new_op->second_operand = NULL;
  new_op->third_operand = NULL;

  return new_op;
}

iloc_operation_list_t *make_operation_list_node() {
  iloc_operation_list_t *node =
      (iloc_operation_list_t *)malloc(sizeof(iloc_operation_list_t));

  node->current_operation = NULL;
  node->next = NULL;

  return node;
}

void add_operation_to_list(iloc_operation_list_t *list,
                           iloc_operation_t *operation) {

  iloc_operation_list_t *end = list;

  // Search for end of list (empty space)
  while (end->next != NULL) {
    end = end->next;
  }

  end->current_operation = operation;
  end->next = make_operation_list_node();
}

void destroy_operand(char *operand) {
  if (operand != NULL) {
    free(operand);
  }
}

void destroy_operation(iloc_operation_t *operation) {
  if (operation != NULL) {
    destroy_operand(operation->first_operand);
    destroy_operand(operation->second_operand);
    destroy_operand(operation->third_operand);

    free(operation);
  }
}

void destroy_operation_list(iloc_operation_list_t *list) {
  if (list != NULL) {
    destroy_operation_list(list->next);
    destroy_operation(list->current_operation);
  }
}

iloc_operation_list_t *concat_operation_lists(iloc_operation_list_t *first,
                                              iloc_operation_list_t *second) {
  // Check if either list is empty. Since we are not using pointer to
  // pointers, we can't actually change their values. We destroy them to avoid
  // having empty nodes in the middle of our list.
  if (first->current_operation == NULL) {
    destroy_operation_list(first);
    // Note that second could still be empty here!
    return second;
  }

  if (second->current_operation == NULL) {
    destroy_operation_list(second);
    return first;
  }

  // Find the end of the first list (and the last node before in it, since
  // we'll need to trim the emtpy last node)
  iloc_operation_list_t *end = first;
  iloc_operation_list_t *prev = NULL;

  while (end->next != NULL) {
    prev = end;
    end = end->next;
  }
  prev->next = second;
  // Free detached node
  destroy_operation_list(end);

  return first;
}

void print_operation(iloc_operation_t *operation) {
  print_operation_by_code(operation->op_code, operation->first_operand,
                          operation->second_operand, operation->third_operand);
}
void print_operation_list(iloc_operation_list_t *list) {
  while (list->next != NULL) {
    print_operation(list->current_operation);
    list = list->next;
  }
}
