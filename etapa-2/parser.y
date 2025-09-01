%{
#include <stdio.h> // Include this for stderr
extern int yylex(void);
extern int yylineno; // Add this line for yylineno
void yyerror (char const *mensagem);
%}

%define parse.error verbose
%token TK_TIPO
%token TK_VAR
%token TK_SENAO
%token TK_DECIMAL
%token TK_SE
%token TK_INTEIRO
%token TK_ATRIB
%token TK_RETORNA
%token TK_SETA
%token TK_ENQUANTO
%token TK_COM
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_ID
%token TK_LI_INTEIRO
%token TK_LI_DECIMAL
%token TK_ER

%%

programa: 
    lista_elementos_opcional ';'
;

lista_elementos_opcional:
    /*vazio*/
    | lista_elementos
;

lista_elementos:
    elemento
    | lista_elementos ',' elemento
;

elemento:
    declaracao_variavel
    |definicao_funcao
;

/*Ela possui um cabeçalho e um corpo.*/
definicao_funcao:
    cabecalho_f corpo_f
;

/*O cabeçalho consiste no token TK_ID seguido do token TK_SETA seguido ou do token
TK_DECIMAL ou do token TK_INTEIRO, seguido por uma lista opcional de parâmetros seguido do
token TK_ATRIB. */
cabecalho_f:
    TK_ID TK_SETA tipo lista_opcional_param TK_ATRIB
;

/*A lista de parâmetros, quando
presente, consiste no token opcional TK_COM se-
guido de uma lista, separada por vírgula, de parâmetros.*/

lista_opcional_param:
    /*vazio*/
    |TK_COM lista_param
;

lista_param:
    param
    |lista_param ',' param
;


/*. Cada parâmetro consiste no token TK_ID
seguido do token TK_ATRIB seguido ou do to-
ken TK_INTEIRO ou do token TK_DECIMAL. */
param:
    TK_ID TK_ATRIB tipo
;


/*Esta declaração é idên-
tica ao comando simples de declaração de variável
(veja abaixo), sendo que a única e importante dife-
rença é que esse elemento não pode receber valo-
res de inicialização.*/

declaracao_variavel:
    TK_VAR TK_ID TK_ATRIB tipo
;

tipo:
    TK_DECIMAL
    |TK_INTEIRO
;

/*Os comandos simples da linguagem podem ser:
bloco de comandos, declaração de variável, co-
mando de atribuição, chamada de função, co-
mando de retorno, e construções de fluxo de con-
trole.*/

comando_simples:
    bloco_comandos
    |comando_declaracao_variavel
    |comando_atribuicao
    |chamada_funcao
    |comando_retorno
    |fluxo_controle
;


/*Bloco de Comandos: Definido entre colchetes, e
consiste em uma sequência, possivelmente vazia,
de comandos simples. Um bloco de comandos
é considerado como um comando único simples
e pode ser utilizado em qualquer construção que
aceite um comando simples.*/
corpo_f:
    bloco_comandos
;

bloco_comandos:
    '[' sequencia_bloco_comando_opcional ']'
;

sequencia_bloco_comando_opcional:
    /*vazio*/
    |sequencia_comandos
;

sequencia_comandos:
    comando_simples
    |sequencia_comandos comando_simples




/*Declaração de Variável: Consiste no token
TK_VAR seguido do token TK_ID, que é por sua
vez seguido do token TK_ATRIB e enfim seguido
do tipo. O tipo pode ser ou o token TK_DECIMAL
ou o token TK_INTEIRO. Uma variável pode ser
opcionalmente inicializada caso sua declaração
seja seguida do token TK_COM e de um literal. Um
literal pode ser ou o token TK_LI_INTEIRO ou o
token TK_LI_DECIMAL.*/

comando_declaracao_variavel:
    TK_VAR TK_ID TK_ATRIB tipo inicializacao_opcional
;

inicializacao_opcional:
    /* vazio */
    | TK_COM literal
;

literal:
    TK_LI_INTEIRO
    | TK_LI_DECIMAL
;


/*Comando de Atribuição: O comando de atribui-
ção consiste em um token TK_ID, seguido do to-
ken TK_ATRIB e enfim seguido por uma expres-
são.*/

comando_atribuicao:
    TK_ID TK_ATRIB expressao
;

/*Chamada de Função: Uma chamada de função
consiste no token TK_ID, seguida de argumentos
entre parênteses, sendo que cada argumento é se-
parado do outro por vírgula. Um argumento é
uma expressão. Uma chamada de função pode
existir sem argumentos.*/

chamada_funcao:
    TK_ID '('argumento_opcional')'
;

argumento_opcional:
    /*vazio*/
    |lista_argumentos

lista_argumentos:
    expressao
    |lista_argumentos ',' expressao
;


/*Comando de Retorno: Trata-se do token
TK_RETORNA seguido de uma expressão, se-
guido do token TK_ATRIB e terminado ou pelo
token TK_DECIMAL ou pelo token TK_INTEIRO.*/

comando_retorno:
    TK_RETORNA expressao TK_ATRIB tipo
;

/*Comandos de Controle de Fluxo: A lingua-
gem possui uma construção condicional e uma
construção iterativa para controle estruturado de
fluxo. A condicional consiste no token TK_SE se-
guido de uma expressão entre parênteses e en-
tão por um bloco de comandos obrigatório. Após
este bloco, podemos opcionalmente ter o token
TK_SENAO que, quando aparece, é seguido obri-
gatoriamente por um bloco de comandos. Temos
apenas uma construção de repetição que é o to-
ken TK_ENQUANTO seguido de uma expressão en-
tre parênteses e de um bloco de comandos.*/

fluxo_controle:
    condicional
    | iterativa
;

condicional:
    TK_SE '(' expressao ')' bloco_comandos senao_opcional
;

senao_opcional:
    /* vazio */
    | TK_SENAO bloco_comandos
;

iterativa:
    TK_ENQUANTO '(' expressao ')' bloco_comandos
;

expressao:
    expressao_p7
;

expressao_p7:
    expressao_p7 '|' expressao_p6
    | expressao_p6
;

expressao_p6:
    expressao_p6 '&' expressao_p5
    | expressao_p5
;

expressao_p5:
    expressao_p5 TK_OC_EQ expressao_p4
    | expressao_p5 TK_OC_NE expressao_p4
    | expressao_p4
;

expressao_p4:
    expressao_p4 '<' expressao_p3
    | expressao_p4 '>' expressao_p3
    | expressao_p4 TK_OC_LE expressao_p3
    | expressao_p4 TK_OC_GE expressao_p3
    | expressao_p3
;

expressao_p3:
    expressao_p3 '+' expressao_p2
    | expressao_p3 '-' expressao_p2
    | expressao_p2
;

expressao_p2:
    expressao_p2 '*' expressao_p1
    | expressao_p2 '/' expressao_p1
    | expressao_p2 '%' expressao_p1
    | expressao_p1
;

expressao_p1:
    '+' expressao_p0
    | '-' expressao_p0
    | '!' expressao_p0
    | expressao_p0
;


expressao_p0:
    TK_ID
    | TK_LI_INTEIRO
    | TK_LI_DECIMAL
    | chamada_funcao
    | '(' expressao ')'
;

%%

void yyerror (const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", yylineno, s);
}