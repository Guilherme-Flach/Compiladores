#include "asd.h"
#include "iloc_functions.h"
#include "stack_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yyparse(void);
extern int yylex_destroy(void);

asd_tree_t *arvore = NULL;

int main(int argc, char **argv) {
  int ret = yyparse();
  // asd_print_graphviz(arvore);
  if(arvore != NULL){
    print_operation_list(arvore->code);
    destroy_operation_list(arvore->code);
    asd_free(arvore);
  }

  yylex_destroy();
  return ret;
}
