#include "stack_functions.h"
#include <string.h>

argument *make_argument(char *name, SYMBOL_TYPE type) {
  argument *arg = malloc(sizeof(argument));

  arg->name = strdup(name);
  arg->type = type;
  return arg;
}

void destroy_argument(argument *arg) {
  free(arg->name);
  free(arg);
}

argument_list_t *make_argument_list_node(argument *arg) {
  argument_list_t *list = malloc(sizeof(argument_list_t));
  list->current = arg;
  list->next = NULL;

  return list;
}

void destroy_argument_list(argument_list_t *list) {
  if (list != NULL) {
    destroy_argument_list(list->next);
    destroy_argument(list->current);
    free(list);
  }
}

symbol_table_t *make_symbol_table_node(symbol_table_entry *symbol) {
  symbol_table_t *table = malloc(sizeof(symbol_table_t));

  table->current = symbol;
  table->next = NULL;

  return table;
}

symbol_table_entry *make_symbol_table_entry(SYMBOL_NATURE nature,
                                            SYMBOL_TYPE type, char *value) {
  symbol_table_entry *symbol = malloc(sizeof(symbol_table_entry));

  symbol->nature = nature;
  symbol->type = type;
  symbol->value = strdup(value);
  symbol->arguments = NULL;

  return symbol;
}

void append_argument_to_symbol(symbol_table_entry *symbol, argument *arg) {
  argument_list_t *cur = symbol->arguments;
  argument_list_t *prev = NULL;

  // Find list end
  while (cur != NULL) {
    prev = cur;
    cur = cur->next;
  }

  argument_list_t *new_node = make_argument_list_node(arg);

  // Check for first insertion;
  if (prev == NULL) {
    symbol->arguments = new_node;
  } else {
    prev->next = new_node;
  }
}

void destroy_symbol_table_entry(symbol_table_entry *symbol) {
  destroy_argument_list(symbol->arguments);
  free(symbol->value);
  free(symbol);
}

symbol_table_t *append_symbol_to_table(symbol_table_t *table,
                                       symbol_table_entry *symbol) {
  symbol_table_t *cur = table;
  symbol_table_t *prev = NULL;

  // Search for end of table
  while (cur != NULL) {
    prev = cur;
    cur = cur->next;
  }

  symbol_table_t *new_node = make_symbol_table_node(symbol);

  // Check for first insertion
  if (prev == NULL) {
    table = new_node;
  } else {
    prev->next = new_node;
  }

  return table;
}

void destroy_symbol_table(symbol_table_t *table) {
  if (table != NULL) {
    destroy_symbol_table(table->next);
    destroy_symbol_table_entry(table->current);
    free(table);
  }
}

stack_node_t *push_symbol_table(stack_node_t *stack, symbol_table_t *table) {
  stack_node_t *new_node = malloc(sizeof(stack_node_t));
  new_node->below = stack;
  new_node->current = table;

  return new_node;
}

stack_node_t *pop_symbol_table(stack_node_t *stack) {
  // check if stack is empty
  if (stack == NULL) {
    printf("Tried to pop empty stack!\n");
    exit(1);
  }

  symbol_table_t *cur = stack->current;
  destroy_symbol_table(cur);

  // Relocate stack
  stack_node_t *new_stack = stack->below;

  // Free top node
  free(stack);

  return new_stack;
}

symbol_table_entry *find_symbol(char *value, stack_node_t *stack) {
  // Go up the stack until we reach the top
  while (stack != NULL) {
    // Look into the table
    symbol_table_t *table_node = stack->current;
    while (table_node != NULL) {
      symbol_table_entry *symbol = table_node->current;
      if (strcmp(symbol->value, value) == 0) {
        return symbol;
      }

      table_node = table_node->next;
    }
    stack = stack->below;
  }

  return NULL;
}
