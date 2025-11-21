#include "iloc_functions.h"
#include "stack_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Variaveis para contagem de temporarios e rotulos
static int temp_register_counter = 0;
static int label_counter = 0;

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

//Gera um novo nome de registrador temporario 
char *generate_temp_register() {
  temp_register_counter++;
  char buffer[16];
  
  //Formato iloc (r + numero do contador)
  snprintf(buffer, sizeof(buffer), "r%d", temp_register_counter);
  return strdup(buffer); //Retorna copia alocada
}

//Gera um novo nome de rotulo
char *generate_label() {
  label_counter++;
  char buffer[16];

  //Formato iloc (L + numero do contador)
  snprintf(buffer, sizeof(buffer), "L%d", label_counter);
  return strdup(buffer); //Retorna uma copia alocada
}

//loadAI
iloc_operation_list_t *generate_load_variable(symbol_table_entry *entry, char **result_reg_ptr) {
  char base_reg[5];
  char offset_str[16];
  
  //Define a base (rbss ou rfp)
  if (entry->storage_type == VAR_LOCAL) {
      strncpy(base_reg, "rfp", sizeof(base_reg));
  } else {
      strncpy(base_reg, "rbss", sizeof(base_reg));
  }

  snprintf(offset_str, sizeof(offset_str), "%d", entry->offset);

  //Gera novo registrador
  char *temp_result = generate_temp_register();
  *result_reg_ptr = temp_result;

  //Operacao loadAI
  iloc_operation_t *loadAI_op = make_operation(ILOC_LOADAI);
  loadAI_op->first_operand = strdup(base_reg);
  loadAI_op->second_operand = strdup(offset_str);
  loadAI_op->third_operand = strdup(temp_result); 

  iloc_operation_list_t *code = make_operation_list_node();
  add_operation_to_list(code, loadAI_op);

  return code;
}

//storeAI
iloc_operation_list_t *generate_store_variable(symbol_table_entry *entry, iloc_operation_list_t *expr_code, char *source_reg) {
  char base_reg[5];
  char offset_str[16];

  //Determina a base (rfp ou rbss)
  if (entry->storage_type == VAR_LOCAL) {
      strncpy(base_reg, "rfp", sizeof(base_reg));
  } else {
      strncpy(base_reg, "rbss", sizeof(base_reg));
  }
  snprintf(offset_str, sizeof(offset_str), "%d", entry->offset);

  //Operacao storeAI
  iloc_operation_t *storeAI_op = make_operation(ILOC_STOREAI);
  storeAI_op->first_operand = strdup(source_reg); //Fonte:resultado da expressao
  storeAI_op->second_operand = strdup(base_reg);  //Base:rfp/rbss
  storeAI_op->third_operand = strdup(offset_str); //Offset

  //Anexa a operacao storeai ao codigo da expressao
  if (expr_code == NULL) {
      expr_code = make_operation_list_node();
  }
  add_operation_to_list(expr_code, storeAI_op);

  return expr_code;
}


//Operadores binarios (ADD, SUB, CMP_LT, etc...)
iloc_operation_list_t *generate_binary_operation(OPERATION_CODE op_code,
  iloc_operation_list_t *code_left,
  char *reg_left,
  iloc_operation_list_t *code_right,
  char *reg_right,
  char **result_reg_ptr) {

  iloc_operation_list_t *code = concat_operation_lists(code_left, code_right);

  //Criacao registrador temporario para resultado
  char *temp_result = generate_temp_register();
  *result_reg_ptr = temp_result; 

  //Operacao ILOC
  iloc_operation_t *op = make_operation(op_code);
  op->first_operand = strdup(reg_left);
  op->second_operand = strdup(reg_right);
  op->third_operand = strdup(temp_result);

  add_operation_to_list(code, op);


  free(reg_left);
  free(reg_right);

  return code;
}

//Operadores unarios
iloc_operation_list_t *generate_unary_operation(OPERATION_CODE op_code,
iloc_operation_list_t *code_expr,
char *reg_expr,
char **result_reg_ptr) {

  //Registrador temporario
  char *temp_result = generate_temp_register();
  *result_reg_ptr = temp_result;

  //Operacao ILOC
  iloc_operation_t *op = make_operation(op_code);

  //Operação unaria 
  switch (op_code) {
    case ILOC_RSUBI: //Para o unario '-' (0 - expr)
      op->first_operand = strdup("0"); 
      op->second_operand = strdup(reg_expr);
      op->third_operand = strdup(temp_result);
      break;
    case ILOC_I2I: //Para o unario '+' (como se fosse um nop)
      op->first_operand = strdup(reg_expr);
      op->second_operand = strdup(temp_result);
      break;
    case ILOC_XORI: //Para o unario '!' (not logico: 1 xor expr)
      op->first_operand = strdup(reg_expr);
      op->second_operand = strdup("1"); 
      op->third_operand = strdup(temp_result);
      break;
    default:
      break;
  }

  add_operation_to_list(code_expr, op);
  free(reg_expr);

  return code_expr;
}