#include "asd.h"
#include "iloc_functions.h"
#include "stack_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yyparse(void);
extern int yylex_destroy(void);

asd_tree_t *arvore = NULL;
stack_node_t *stack;

int main(int argc, char **argv) {
  int ret = yyparse();
  // asd_print_graphviz(arvore);

  // At the botom of the stack is the global context
  stack_node_t *stack_bottom = stack;
  while (stack_bottom != NULL && stack_bottom->table_below != NULL) {
    stack_bottom = stack_bottom->table_below;
  }

  if (arvore != NULL) {
    print_operation_list(arvore->code, stack_bottom);
    stack = pop_symbol_table(stack);

    destroy_operation_list(arvore->code);
    asd_free(arvore);
  }

  yylex_destroy();
  return ret;
}
