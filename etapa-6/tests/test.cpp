// #include "../src/main.c"

#include "gtest/gtest.h"
#include <cstddef>
#include <gtest/gtest.h>

extern "C" {
int yylex_destroy(void);
int yy_scan_string(const char *);
int yyparse();

#include "../asd.h"
}

asd_tree_t *arvore = NULL;
