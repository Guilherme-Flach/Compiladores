#ifndef SEMANTIC_CHECKS_HEADER
#define SEMANTIC_CHECKS_HEADER

#include "asd.h"
#include "stack_functions.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>


void check_function_arguments(const char *func_id, asd_tree_t *provided_args_ast,  argument_list_t *expected_params, int line_number);
SYMBOL_TYPE infer_type(SYMBOL_TYPE type1, SYMBOL_TYPE type2);
int tipo_compativel(asd_node_t a, SYMBOL_TYPE b); 


#endif