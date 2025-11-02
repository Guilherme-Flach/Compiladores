/* 00343210 - Eduarda Tessari Pereira
   00342108 - Guilherme Ismael Flach   */

 %code requires {
    #include "asd.h"
    #include <stdlib.h>
    #include <string.h>
    #include "stack_functions.h"

    #define ERR_UNDECLARED 10 //2.2
    #define ERR_DECLARED 11 //2.2
    #define ERR_VARIABLE 20 //2.3
    #define ERR_FUNCTION 21 //2.3
    #define ERR_WRONG_TYPE 30 //2.4
    #define ERR_MISSING_ARGS 40 //2.5
    #define ERR_EXCESS_ARGS 41 //2.5
    #define ERR_WRONG_TYPE_ARGS 42 //2.5

    // ETAPA 3:
    typedef enum { LITERAL, IDENTIFIER } lex_value_type;

    typedef struct {
        int line_number;
        lex_value_type token_type;
        char *token_value;
    } valor_lexico;

    void debug_node(asd_tree_t* node);

 }

%code {
    // ETAPA 3:
    void debug_node(asd_tree_t* node) {
        asd_print_graphviz(node);
        exit(0);
    }
}

%union {
  asd_tree_t *tree;
  valor_lexico lex_value;
}

%type <tree> programa lista_elementos lista_elementos_opcional elemento definicao_funcao cabecalho_f corpo_f
%type <tree> lista_opcional_param tk_com_opcional lista_param lista_param_opcional param declaracao_variavel tipo
%type <tree> comando_simples bloco_comandos sequencia_bloco_comando_opcional sequencia_comandos
%type <tree> comando_declaracao_variavel inicializacao_opcional literal comando_atribuicao chamada_funcao argumento_opcional lista_argumentos lista_argumentos_opcional
%type <tree> comando_retorno fluxo_controle condicional senao_opcional iterativa
%type <tree> expressao expressao_p7 expressao_p6 expressao_p5 expressao_p4 expressao_p3 expressao_p2 expressao_p1 expressao_p0

%token <lex_value> TK_ID TK_LI_INTEIRO TK_LI_DECIMAL

%{
#include <stdio.h>
extern int yylex(void);
extern int yylineno;
void yyerror (char const *mensagem);

extern asd_tree_t *arvore;
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
%token TK_ER

%%

programa:
    %empty {$$ = NULL;}
    | lista_elementos ';'
    {
        //Raiz da arvore
        arvore = $1;
        $$ = $1;

    }

;

lista_elementos:
    elemento lista_elementos_opcional
    {
    /*Listas de funções, onde cada função tem dois filhos, um que é o seu primeiro comando
    e outro que é a próxima função;*/
        if ($1 != NULL){

            //Se o elemento é uma função válida (não podada)
            if ($2 != NULL) {
                 //Adiciona a próxima função como filho da função atual
                asd_add_child($1, $2);
            }
            // A lista de funções é ligada no nó pai da lista, não na função em si.
            $$ = $1;
        }
        else{
            // Propaga o próximo elemento caso o primeiro seja podado
            $$ = $2;
        }
    }
;

lista_elementos_opcional:
    ',' lista_elementos { $$ = $2;};
    | %empty { $$ = NULL;}
;

elemento:
    declaracao_variavel {$$ = $1;}
    | definicao_funcao   {$$ = $1;}
;

/*Uma funcao possui um cabeçalho e um corpo.*/
definicao_funcao:
    cabecalho_f corpo_f
    {
        /*Usa o cabeçalho da função (lexema do identificador)*/
        $$ = $1;

        /*pode estar NULL se corpo_f for vazio*/
        if ($2 != NULL){

            /*Aponta para o primeiro comando.*/
            asd_add_child($$, $2);
        }
        //debug_node($$);
    }
;

/*O cabeçalho consiste no token TK_ID seguido do token TK_SETA seguido ou do token
TK_DECIMAL ou do token TK_INTEIRO, seguido por uma lista opcional de parâmetros seguido do
token TK_ATRIB. */
cabecalho_f:
    TK_ID TK_SETA tipo lista_opcional_param TK_ATRIB
    {
        /*Para funções, deve-se utilizar seu identificador (o nome da função).*/
        $$ = asd_new($1.token_value);
        free($1.token_value);
    }
;

/*A lista de parâmetros, quando
presente, consiste no token opcional TK_COM se-
guido de uma lista, separada por vírgula, de parâmetros.*/

lista_opcional_param:
    %empty  {$$ = NULL;}
    | tk_com_opcional lista_param {$$ = $2;} //propaga a lista de parâmetros
;

tk_com_opcional:
    %empty {$$ = NULL;} //poda
    | TK_COM {$$ = NULL;} //poda
;

lista_param:
    param lista_param_opcional
;

lista_param_opcional:
    ',' lista_param {$$ = $2;}
    |%empty {$$ = NULL;}
;

/*. Cada parâmetro consiste no token TK_ID
seguido do token TK_ATRIB seguido ou do to-
ken TK_INTEIRO ou do token TK_DECIMAL. */
param:
    TK_ID TK_ATRIB tipo
    {
        $$ = NULL; //poda
        free($1.token_value); //libera o ID do parâmetro
    }
;


/*Esta declaração é idêntica ao comando simples de declaração de variável
(veja abaixo), sendo que a única e importante dife-
rença é que esse elemento não pode receber valo-
res de inicialização.*/

declaracao_variavel:
    TK_VAR TK_ID TK_ATRIB tipo
    {
        free($2.token_value); //libera o ID
        $$ = NULL; //poda
    }
;

tipo:
    TK_DECIMAL {$$ = NULL;} //poda
    |TK_INTEIRO {$$ = NULL;}
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
    bloco_comandos { $$ = $1; }
;

bloco_comandos:
    '[' sequencia_bloco_comando_opcional ']' {$$ = $2;}
;

sequencia_bloco_comando_opcional:
    %empty {$$ = NULL;}
    | sequencia_comandos {$$ = $1;}
;

sequencia_comandos:
    comando_simples {$$ = $1;}
    | comando_simples sequencia_comandos
    {
        if ($1 == NULL){
            $$ = $2;  //se o primeiro comando foi podado
        }
        else{
            $$ = $1;
            if ($2 != NULL){
                asd_add_child($$, $2);
            }
        }
    }
;

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
    {
        /*Se a variável nao tiver incialização ($inicializacao_opcional == NULL),
         não deve aparecer na AST. */
        if ($5 == NULL) {
            $$ = NULL;
            free($2.token_value);

        } else {
            /*"Rouba" o nodo do lexema TK_COM e coloca o identificador do filho nele. */
            $$ = $5;

            /* Cria um nodo novo com a label do lexema de TK_ID.
             Esse passo poderia ser feito em uma produção do tipo
             ```c
             identificador:
                TK_ID { $$ = asd_new($1.token_value); free($1.token_value);}
             ```
            Mas achamos que assim fica um pouco mais semântico.*/
            asd_tree_t* id_node = asd_new($2.token_value);
            asd_add_child($$, id_node);
            free($2.token_value);

        }
    }
;


inicializacao_opcional:
    %empty { $$ = NULL; }
    | TK_COM literal
    {
        /*Declaração de variável com incialização, o nome deve ser o lexema do token TK_COM.*/
        $$ = asd_new("com");
        asd_add_child($$, $2);
    }
;

literal:
    TK_LI_INTEIRO
    {
        /*Literais utilizam o próprio lexema.*/
        $$ = asd_new($1.token_value);
        free($1.token_value);
    }
    | TK_LI_DECIMAL
    {
        /*Literais utilizam o próprio lexema.*/
        $$ = asd_new($1.token_value);
        free($1.token_value);
    }
;


/*Comando de Atribuição: O comando de atribui-
ção consiste em um token TK_ID, seguido do to-
ken TK_ATRIB e enfim seguido por uma expres-
são.*/

comando_atribuicao:
    TK_ID TK_ATRIB expressao
    {
        /*O comando de atribuição deve ter pelo menos dois filhos, um que é o iden-
        tificador e outro que é o valor da expressão.*/

        //Nó pai com o lexema do operador de atribuição
        $$ = asd_new(":=");

        //Cria o nó para o ID e o anexa ao nó pai
        asd_tree_t* id_node = asd_new($1.token_value);
        asd_add_child($$, id_node);
        free($1.token_value); //libera a string do ID

        //Anexa o nó da expressao ao nó pai
        asd_add_child($$, $3);
    }
;

/*Chamada de Função: Uma chamada de função
consiste no token TK_ID, seguida de argumentos
entre parênteses, sendo que cada argumento é se-
parado do outro por vírgula. Um argumento é
uma expressão. Uma chamada de função pode
existir sem argumentos.*/

chamada_funcao:
    TK_ID '('argumento_opcional')'
    {
        /*O comando chamada de função tem pelo menos um filho, que
        é a primeira expressão na lista de seus argumentos.*/

        //Cria rótulo da funcao
        char label_buffer[128];
        snprintf(label_buffer, sizeof(label_buffer), "call %s", $1.token_value);

        $$ = asd_new(label_buffer); //Cria o nó pai

        if ($3 != NULL) {
            asd_add_child($$, $3); //Anexa a raiz da lista de argumentos (se não for vazia)
        }

        free($1.token_value); //Libera a string do ID

    }
;

argumento_opcional:
    %empty {$$ = NULL;}
    |lista_argumentos

lista_argumentos:
    expressao lista_argumentos_opcional
    {
        // $1 é o argumento atual, $2 é o próximo argumento (se não for vazio)
        if ($2 != NULL) {
            asd_add_child($1, $2); //Liga o argumento atual ao próximo argumento
        }
        $$ = $1; //Propaga a raiz da lista (o primeiro argumento)
    }
;

lista_argumentos_opcional:
    ',' lista_argumentos {$$ = $2;} //propaga a lista seguinte
    |%empty {$$ = NULL;}
;

/*Comando de Retorno: Trata-se do token
TK_RETORNA seguido de uma expressão, se-
guido do token TK_ATRIB e terminado ou pelo
token TK_DECIMAL ou pelo token TK_INTEIRO.*/

comando_retorno:
    TK_RETORNA expressao TK_ATRIB tipo
    {
        /*O comando return tem um filho, que é uma expressão.*/
        $$ = asd_new("retorna");
        asd_add_child($$, $2);
    }
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
    {
        /*O comando if com else opcional deve ter pelo menos três filhos,
        um para a expressão, outro para o primeiro comando quando verdade,
        e o último – opcional – para o segundocomando quando falso.*/

        $$ = asd_new("se");
        asd_add_child($$, $3); //Primeiro filho: condição (expressao)
        asd_add_child($$, $5); //Segundo filho: bloco true (bloco_comandos)

        if ($6 != NULL) {
            asd_add_child($$, $6); //Terceiro filho (opcional): bloco else (senao_opcional)
        }
    }
;

senao_opcional:
    %empty {$$ = NULL;}
    | TK_SENAO bloco_comandos
    {
        //O nó "senao" é o bloco de comandos ($2)
        $$ = $2;
    }
;

iterativa:
    TK_ENQUANTO '(' expressao ')' bloco_comandos
    {
        /*O comando while deve ter pelo menos dois filhos, um para expressão e outro
        para o primeiro comando do laço.*/

        $$ = asd_new("enquanto");
        asd_add_child($$, $3); //Primeiro filho: condicao (expressao)
        asd_add_child($$, $5); //Segundo filho: bloco_comandos
    }
;

/* Precedencia definida utilizando expressao_pX, onde X, em ordem decrescente,
indica qual expressao sera avaliada primeiro (ex: expressao_p0 possui precedencia
maior que expressao_p1, expressao_p1 possui precedencia maior que expressao_p0)*/

expressao:
    expressao_p7 {$$ = $1;}
;

expressao_p7:
    expressao_p7 '|' expressao_p6
    {
        $$ = asd_new("|");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p6 {$$ = $1;}
;

expressao_p6:
    expressao_p6 '&' expressao_p5
    {
        $$ = asd_new("&");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p5 {$$ = $1;}
;

expressao_p5:
    expressao_p5 TK_OC_EQ expressao_p4
    {
        $$ = asd_new("==");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p5 TK_OC_NE expressao_p4
    {
        $$ = asd_new("!=");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p4 {$$ = $1;}
;

expressao_p4:
    expressao_p4 '<' expressao_p3
    {
        $$ = asd_new("<");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p4 '>' expressao_p3
    {
        $$ = asd_new(">");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p4 TK_OC_LE expressao_p3
    {
        $$ = asd_new("<=");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p4 TK_OC_GE expressao_p3
    {
        $$ = asd_new(">=");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p3 {$$ = $1;}
;

expressao_p3:
    expressao_p3 '+' expressao_p2
    {
        $$ = asd_new("+");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p3 '-' expressao_p2
    {
        $$ = asd_new("-");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p2  {$$ = $1;}
;

expressao_p2:
    expressao_p2 '*' expressao_p1
    {
        $$ = asd_new("*");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p2 '/' expressao_p1
    {
        $$ = asd_new("/");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p2 '%' expressao_p1
    {
        $$ = asd_new("%");
        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p1  {$$ = $1;}
;

expressao_p1:
    '+' expressao_p1
    {
        $$ = asd_new("+");
        asd_add_child($$, $2);
    }
    | '-' expressao_p1
    {
        $$ = asd_new("-");
        asd_add_child($$, $2);
    }
    | '!' expressao_p1
    {
        $$ = asd_new("!");
        asd_add_child($$, $2);
    }
    | expressao_p0 {$$ = $1;}
;


expressao_p0:
    TK_ID
    {
        $$ = asd_new($1.token_value);
        free($1.token_value);

    }
    | TK_LI_INTEIRO
    {
        $$ = asd_new($1.token_value);
        free($1.token_value);
    }
    | TK_LI_DECIMAL
    {
        $$ = asd_new($1.token_value);
        free($1.token_value);
    }
    | chamada_funcao {$$ = $1;}
    | '(' expressao ')' {$$ = $2;}
;

%%

void yyerror (const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", yylineno, s);
}