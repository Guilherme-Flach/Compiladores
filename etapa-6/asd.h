#ifndef _ASD_H_
#define _ASD_H_

#include "stack_functions.h"
#include "iloc_functions.h" 

typedef struct asd_tree {
  char *label;
  int number_of_children;
  SYMBOL_TYPE type;
  struct asd_tree **children;
  iloc_operation_list_t *code;
  char *result_reg;
} asd_tree_t;

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label);

asd_tree_t *asd_new_with_type(const char *label, SYMBOL_TYPE node_type);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz(asd_tree_t *tree);
#endif //_ASD_H_
