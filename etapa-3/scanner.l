/* 00342108 - Eduarda Tessari Pereira
   00342108 - Guilherme Ismael Flach   */

%option noyywrap
%option yylineno

%{
#include "parser.tab.h"
%}

%%

"tipo"      { return TK_TIPO; }
"var"       { return TK_VAR; }
"senao"     { return TK_SENAO; }
"decimal"   { return TK_DECIMAL; }
"se"        { return TK_SE; }
"inteiro"   { return TK_INTEIRO; }
"retorna"   { return TK_RETORNA; }
"enquanto"  { return TK_ENQUANTO; }
"com"       { return TK_COM; }
"<="        { return TK_OC_LE; }
">="        { return TK_OC_GE; }
"=="        { return TK_OC_EQ; }
"!="        { return TK_OC_NE; }
":="        { return TK_ATRIB; }
"->"        { return TK_SETA; }

/* For the characters "! * % + - < > [ ] ( ) = , ; & |", (excluding spaces) simply return their ascii value. */
[\!\*/%\+\-<>\[\]\(\)=,;&\|] { return (int)yytext[0]; }

/* IDs can start with any letter or _ and may contain numbers after that. */
[a-z_][a-z_0-9]* { return TK_ID;}

/* Decimals may optionally omit the number before the . */
[0-9]*\.[0-9]+ {return TK_LI_DECIMAL;}

[0-9]+ {return TK_LI_INTEIRO;}

/* Ignore comments */
"//".*   ;

/* Ignore whitespaces */
[ \t\n]+ ;

/* Finally, if none of the rules apply, throw a LEX error. */
. { return TK_ER; }

%%

int get_line_number (void) {
  return yylineno;
}

