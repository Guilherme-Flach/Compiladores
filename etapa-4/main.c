#include "asd.h"
#include "stack_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yyparse(void);
extern int yylex_destroy(void);

asd_tree_t *arvore = NULL;
stack_node_t *stack = NULL;

int main(int argc, char **argv) {
  int ret = yyparse();
  asd_print_graphviz(arvore);
  asd_free(arvore);
  yylex_destroy();
  return ret;
}
