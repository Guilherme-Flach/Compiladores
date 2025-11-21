#include "semantic_checks.h"

void check_function_arguments(const char *func_id,
                              asd_tree_t *provided_args_ast,
                              argument_list_t *expected_params,
                              int line_number) {

  asd_tree_t *provided_arg = provided_args_ast;
  argument_list_t *expected_param = expected_params;

  int arg_index = 0;

  // Percorre as listas simultaneamente para contagem e checagem de tipos
  while (provided_arg != NULL && expected_param != NULL) {
    arg_index++;

    // Verificacao de tipo: checa a compatibilidade entre o tipo anotado na AST
    // e o tipo esperado na TS
    if (!tipo_compativel(provided_arg->type, expected_param->arg->type)) {
      fprintf(stderr,
              "Erro semântico: tipo do argumento %d é incompatível na chamada "
              "de '%s' na linha %d.\n",
              arg_index, func_id, line_number);
      exit(ERR_WRONG_TYPE_ARGS);
    }

    // Move para o proximo argumento nas duas listas
    if (provided_arg->number_of_children > 0) {
      provided_arg = provided_arg->children[0];
    } else {
      provided_arg = NULL;
    }
    expected_param = expected_param->next_arg;
  }

  // Verificacao de contagem argumentos faltando
  if (provided_arg == NULL && expected_param != NULL) {
    fprintf(
        stderr,
        "Erro semântico: faltam argumentos na chamada de '%s' na linha %d.\n",
        func_id, line_number);
    exit(ERR_MISSING_ARGS);
  }

  // Verificacao de contagem argumentos em excesso
  if (provided_arg != NULL && expected_param == NULL) {
    fprintf(stderr,
            "Erro semântico: argumentos em excesso na chamada de '%s' na linha "
            "%d.\n",
            func_id, line_number);
    exit(ERR_EXCESS_ARGS);
  }
}

SYMBOL_TYPE infer_type(SYMBOL_TYPE type1, SYMBOL_TYPE type2) {
  if (type1 == S_INTEGER && type2 == S_INTEGER) {
    return S_INTEGER; // int + int = int
  }
  if (type1 == S_FLOAT && type2 == S_FLOAT) {
    return S_FLOAT; // float + float = float
  }

  fprintf(stderr,
          "Erro semântico: tipos incompatíveis na expressão (float e int).");
  exit(ERR_WRONG_TYPE);
}

int tipo_compativel(SYMBOL_TYPE a, SYMBOL_TYPE b) {
  return (a == S_INTEGER && b == S_INTEGER) || (a == S_FLOAT && b == S_FLOAT);
}