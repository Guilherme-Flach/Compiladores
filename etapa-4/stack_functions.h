#ifndef STACK_FUNCTIONS_HEADER
#define STACK_FUNCTIONS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { S_LITERAL, S_IDENTIFIER, S_FUNCTION } SYMBOL_NATURE;
typedef enum { S_INTEGER, S_FLOAT } SYMBOL_TYPE;

typedef struct {
  char *name;
  SYMBOL_TYPE type;
} argument;

typedef struct argument_list {
  struct argument_list *next_arg;
  argument *arg;
} argument_list_t;

typedef struct {
  SYMBOL_NATURE nature;
  SYMBOL_TYPE type;
  argument_list_t *arguments;
  char *value;
} symbol_table_entry;

typedef struct symbol_table {
  struct symbol_table *next_symbol;
  symbol_table_entry *symbol;
} symbol_table_t;

typedef struct stack_node {
  struct stack_node *table_below;
  symbol_table_t *table_contents;
} stack_node_t;

argument *make_argument(char *name, SYMBOL_TYPE type);
void destroy_argument(argument *arg);

void append_argument_to_symbol(symbol_table_entry *symbol, argument *arg);
argument_list_t *make_argument_list_node(argument *arg);
void destroy_argument_list(argument_list_t *list);

symbol_table_entry *make_symbol_table_entry(SYMBOL_NATURE nature,
                                            SYMBOL_TYPE type, char *value);

void destroy_symbol_table_entry(symbol_table_entry *symbol);

symbol_table_t *make_symbol_table_node();

void append_symbol_to_table(symbol_table_t *table, symbol_table_entry *symbol);
void destroy_symbol_table(symbol_table_t *table);

stack_node_t *push_symbol_table(stack_node_t *stack, symbol_table_t *table);
stack_node_t *pop_symbol_table(stack_node_t *stack);

// Look for a symbol on the currently available tables, if not found, returns
// NULL
symbol_table_entry *find_symbol(char *value, stack_node_t *stack);

#endif