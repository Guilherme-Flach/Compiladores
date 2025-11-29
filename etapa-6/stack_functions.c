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
  list->arg = arg;
  list->next_arg = NULL;

  return list;
}

void destroy_argument_list(argument_list_t *list) {
  if (list != NULL) {
    destroy_argument_list(list->next_arg);
    destroy_argument(list->arg);
    free(list);
  }
}

symbol_table_t *make_symbol_table_node() {
  symbol_table_t *table = malloc(sizeof(symbol_table_t));

  table->symbol = NULL;
  table->next_symbol = NULL;

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
    cur = cur->next_arg;
  }

  argument_list_t *new_node = make_argument_list_node(arg);

  // Check for first insertion;
  if (prev == NULL) {
    symbol->arguments = new_node;
  } else {
    prev->next_arg = new_node;
  }
}

void destroy_symbol_table_entry(symbol_table_entry *symbol) {
  if (symbol != NULL) {
    destroy_argument_list(symbol->arguments);
    free(symbol->value);
    free(symbol);
  }
}

void append_symbol_to_table(symbol_table_t *table, symbol_table_entry *symbol) {
  symbol_table_t *end = table;

  // Search for end of table (empty space)
  while (end->next_symbol != NULL) {
    end = end->next_symbol;
  }

  end->symbol = symbol;
  end->next_symbol = make_symbol_table_node();
}

void destroy_symbol_table(symbol_table_t *table) {
  if (table != NULL) {
    destroy_symbol_table(table->next_symbol);
    destroy_symbol_table_entry(table->symbol);
    free(table);
  }
}

stack_node_t *push_symbol_table(stack_node_t *stack, symbol_table_t *table) {
  stack_node_t *new_node = malloc(sizeof(stack_node_t));
  new_node->table_below = stack;
  new_node->table_contents = table;

  return new_node;
}

stack_node_t *pop_symbol_table(stack_node_t *stack) {
  // check if stack is empty
  if (stack == NULL) {
    printf("Tried to pop empty stack!\n");
    exit(1);
  }

  symbol_table_t *cur = stack->table_contents;
  destroy_symbol_table(cur);

  // Relocate stack
  stack_node_t *new_stack = stack->table_below;

  // Free top node
  free(stack);

  return new_stack;
}

symbol_table_entry *find_symbol(stack_node_t *stack, char *value,
                                SYMBOL_NATURE nature) {
  // Go up the stack until we reach the top
  while (stack != NULL) {
    // Look into the table
    symbol_table_t *table_node = stack->table_contents;
    while (table_node->symbol != NULL) {
      symbol_table_entry *symbol = table_node->symbol;
      if (strcmp(symbol->value, value) == 0 && symbol->nature == nature) {
        return symbol;
      }

      table_node = table_node->next_symbol;
    }
    stack = stack->table_below;
  }

  return NULL;
}

void ts_insert_local(stack_node_t *stack, symbol_table_entry *symbol) {

  // Access the symbol table that is at the top of the stack
  symbol_table_t *current_table = stack->table_contents;

  append_symbol_to_table(current_table, symbol);
}
