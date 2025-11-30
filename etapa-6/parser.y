/* 00343210 - Eduarda Tessari Pereira
   00342108 - Guilherme Ismael Flach   */

 %code requires {
    #include "asd.h"
    #include <stdlib.h>
    #include <string.h>
    #include "stack_functions.h"
    #include "semantic_checks.h"
    #include "errors.h"

    typedef enum { LITERAL, IDENTIFIER } lex_value_type;

    typedef struct {
        int line_number;
        lex_value_type token_type;
        char *token_value;
    } valor_lexico;

    void debug_node(asd_tree_t* node);

 }

%code {

    void debug_node(asd_tree_t* node) {
        printf("Node: %p\n", node);
        printf("Code: %p\n",node->code);
        print_operation_list(node->code, stack);
    }
}

%union {
  asd_tree_t *tree;
  valor_lexico lex_value;
  SYMBOL_TYPE symbol_type;
}

%type <symbol_type> tipo
%type <tree> programa lista_elementos lista_elementos_opcional elemento definicao_funcao cabecalho_f declaracao_funcao corpo_f
%type <tree> lista_opcional_param tk_com_opcional lista_param lista_param_opcional param declaracao_variavel 
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
extern stack_node_t *stack;

symbol_table_entry* current_function = NULL;

int global_offset_counter = 0;
int local_offset_counter = 0;

#define WORD_SIZE 4

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
    %empty 
    { 
        $$ = NULL;
    }
    | inicializa_programa lista_elementos ';'
    {   
        arvore = $2;
        $$ = $2;

        if($2 != NULL){
            iloc_operation_list_t *program_code = $2->code;
            $2->code = generate_program_startup_shutdown(program_code);
        }

    }
;

inicializa_programa:
    %empty
    {
        stack = push_symbol_table(NULL, make_symbol_table_node());
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
            $$->type = $1->type;
        }
        else{
            // Propaga o próximo elemento caso o primeiro seja podado
            $$ = $2;
            if($2 != NULL)
            {
                $$->type = $2->type; 
            }
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

        stack = pop_symbol_table(stack);
        /*Copia o codigo do corpo*/
        $$->code = $2->code;
    }
;


/*O cabeçalho consiste no token TK_ID seguido do token TK_SETA seguido ou do token
TK_DECIMAL ou do token TK_INTEIRO, seguido por uma lista opcional de parâmetros seguido do
token TK_ATRIB. */
cabecalho_f:
    declaracao_funcao abre_escopo_funcao lista_opcional_param TK_ATRIB
    {
        $$ = $1;        
    }
;

declaracao_funcao: 
    TK_ID TK_SETA tipo
    {
        //Verificacao: checa se a funcao ja foi declarada no escopo atual
        if (find_symbol(stack, $1.token_value, S_FUNCTION) != NULL) {
            fprintf(stderr, "Erro semântico: dupla declaração do símbolo '%s' na linha %d.\n", $1.token_value, $1.line_number);
            exit(ERR_DECLARED);
        }
        
        //Insercao
        symbol_table_entry *new_func = make_symbol_table_entry(
            S_FUNCTION,       
            $3,
            $1.token_value    
        );
        ts_insert_local(stack, new_func);

        current_function = new_func;

        
        /*Para funções, deve-se utilizar seu identificador (o nome da função).*/
        $$ = asd_new($1.token_value);
        $$->type = $3;  
        free($1.token_value);
    }
;


abre_escopo_funcao:
    %empty { 
        stack = push_symbol_table(stack, make_symbol_table_node());

        //Entrada em escopo de funcao reseta o contador do offset local
        local_offset_counter = 0;
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
        //Verificacao
        if (find_symbol(stack, $1.token_value, S_IDENTIFIER) != NULL) {
            fprintf(stderr, "Erro semântico: dupla declaração do parâmetro '%s' na linha %d.\n", $1.token_value, $1.line_number);
            exit(ERR_DECLARED); 
        }

        //Insercao
        symbol_table_entry *new_param = make_symbol_table_entry(
            S_IDENTIFIER, 
            $3,          
            $1.token_value
        );
        
        //Enderecamento
        new_param->offset = local_offset_counter;
        new_param->storage_type = VAR_LOCAL;
        local_offset_counter += WORD_SIZE;
        
        ts_insert_local(stack, new_param);

        argument* new_arg = make_argument($1.token_value, $3);

        append_argument_to_symbol(current_function, new_arg);

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
        if (find_symbol(stack, $2.token_value, S_IDENTIFIER) != NULL) {
            fprintf(stderr, "Erro semântico: dupla declaração do símbolo '%s' na linha %d.\n", $2.token_value, $2.line_number);
            exit(ERR_DECLARED);
        }

        symbol_table_entry *new_var = make_symbol_table_entry(
            S_IDENTIFIER, 
            $4,          
            $2.token_value
        );

        //Enderecamento
        new_var->offset = global_offset_counter;
        new_var->storage_type = VAR_GLOBAL;
        global_offset_counter += WORD_SIZE;

        ts_insert_local(stack, new_var);

        free($2.token_value); //libera o ID
        $$ = NULL; //poda
    }
;

tipo:
    TK_DECIMAL {$$ = S_FLOAT;} 
    |TK_INTEIRO {$$ = S_INTEGER;}
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

abre_bloco:
    '[' { stack = push_symbol_table(stack, make_symbol_table_node()); }
;

fecha_bloco:
    ']' { stack = pop_symbol_table(stack); }
;

bloco_comandos:
    abre_bloco sequencia_bloco_comando_opcional fecha_bloco
    {
        $$ = $2;
        if ($2 != NULL)
            $$->type = $2->type;
    
        //Propagacao codigo do bloco
        $$->code = $2->code;
    }
;

sequencia_bloco_comando_opcional:
    %empty {$$ = NULL;}
    | sequencia_comandos {$$ = $1;}
;

sequencia_comandos:
    comando_simples
    {
        $$ = $1; 
         if ($1 != NULL) {
            $$->type = $1->type;
        }
    }
    | comando_simples sequencia_comandos
    {
        if ($1 == NULL){
            if ($2 != NULL) {
                $$ = $2;  //se o primeiro comando foi podado
                $$->type = $2->type;
            } else {
                $$ = NULL;
            }
        }
        else{
            $$ = $1;
            $$->type = $1->type;
            if ($2 != NULL){
                asd_add_child($$, $2);

                //Concatenacao ILOC
                $$->code = concat_operation_lists($1->code, $2->code);
            }
            else{
                //Propaga o codigo de $1
                $$->code = $1->code;
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
        //Verificacao
        if (find_symbol(stack, $2.token_value, S_IDENTIFIER) != NULL) {
            fprintf(stderr, "Erro semântico: dupla declaração da variável '%s' na linha %d.\n", $2.token_value, $2.line_number);
            exit(ERR_DECLARED); 
        }
        
        //Insercao
        symbol_table_entry *new_var = make_symbol_table_entry(
            S_IDENTIFIER, 
            $4,          
            $2.token_value
        );

        //Enderecamento
        local_offset_counter += WORD_SIZE;
        new_var->offset = -local_offset_counter;
        new_var->storage_type = VAR_LOCAL;        

        ts_insert_local(stack, new_var);
        
        /*Se a variável nao tiver incialização ($inicializacao_opcional == NULL),
         não deve aparecer na AST. */
        if ($5 == NULL) {
            $$ = NULL;
            free($2.token_value);

        } else {
            /*"Rouba" o nodo do lexema TK_COM e coloca o identificador do filho nele. */
            $$ = $5;

            // Verificação de compatibilidade de tipos
            if (!tipo_compativel($5->type,$4)) {
                fprintf(stderr, 
                    "Erro semântico: tipo da inicialização incompatível com a declaração de '%s' na linha %d.\n", 
                    $2.token_value, $2.line_number);
                exit(ERR_WRONG_TYPE);
            }

            $$->type = $4; 

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
        $$->type = $2->type;
    }
;

literal:
    TK_LI_INTEIRO
    {
        /*Literais utilizam o próprio lexema.*/
        $$ = asd_new($1.token_value);
        $$->type = S_INTEGER; 
        free($1.token_value);
    }
    | TK_LI_DECIMAL
    {
        /*Literais utilizam o próprio lexema.*/
        $$ = asd_new($1.token_value);
        $$->type = S_FLOAT; 
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
        symbol_table_entry *id_entry = find_symbol(stack, $1.token_value, S_IDENTIFIER);

        //Verificacao nao declarado
        if (id_entry == NULL) {
            //Verificacao uso incorreto
            symbol_table_entry *function_entry = find_symbol(stack, $1.token_value, S_FUNCTION);
            if (function_entry != NULL) {
                fprintf(stderr, "Erro semântico: função '%s' usada como variável para atribuição na linha %d.\n", $1.token_value, $1.line_number);
                exit(ERR_FUNCTION);
            }

            fprintf(stderr, "Erro semântico: símbolo '%s' não declarado na linha %d.\n", $1.token_value, $1.line_number);
            exit(ERR_UNDECLARED);
        }


        //Obtencao dos tipos 
        SYMBOL_TYPE id_type = id_entry->type;   
        SYMBOL_TYPE expr_type = $3->type;

        //Verificacao de compatibilidade
        if (id_type != expr_type) {
             fprintf(stderr, "Erro semântico: atribuição com tipos incompatíveis para '%s' na linha %d.\n", $1.token_value, $1.line_number);
             exit(ERR_WRONG_TYPE); 
        }
        
        /*O comando de atribuição deve ter pelo menos dois filhos, um que é o iden-
        tificador e outro que é o valor da expressão.*/

        //Nó pai com o lexema do operador de atribuição
        $$ = asd_new(":=");

        //Anotacao de tipo
        $$->type = id_type;

        //Cria o nó para o ID e o anexa ao nó pai
        asd_tree_t* id_node = asd_new($1.token_value);
        asd_add_child($$, id_node);

        //Anexa o nó da expressao ao nó pai
        asd_add_child($$, $3);

        //Geracao de codigo ILOC
        char *source_reg = $3->result_reg; //registrador que contem o resultado final da expressao
        $$->code = generate_store_variable(id_entry, $3->code, source_reg);

        free(source_reg);
        $3->result_reg = NULL;
        free($1.token_value); //libera a string do ID
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
        //Verificacao
        symbol_table_entry *function_entry = find_symbol(stack, $1.token_value, S_FUNCTION);
                
        if (function_entry == NULL) {
            //Verificacao de natureza (iItem 2.3)
            //Se a entrada for um identificador e não uma função, erro.
            symbol_table_entry *id_entry = find_symbol(stack, $1.token_value, S_IDENTIFIER);
            if (id_entry != NULL) {
                fprintf(stderr, "Erro semântico: variável '%s' usada como função na linha %d.\n", $1.token_value, $1.line_number);
                exit(ERR_VARIABLE);
            }

            fprintf(stderr, "Erro semântico: símbolo '%s' não declarado na linha %d.\n", $1.token_value, $1.line_number);
            exit(ERR_UNDECLARED);
        }

        
        
        //Verificacao de argumentos
        check_function_arguments($1.token_value, $3, function_entry->arguments, $1.line_number);
        
        /*O comando chamada de função tem pelo menos um filho, que
        é a primeira expressão na lista de seus argumentos.*/

        //Cria rótulo da funcao
        char label_buffer[128];
        snprintf(label_buffer, sizeof(label_buffer), "call %s", $1.token_value);

        $$ = asd_new(label_buffer); //Cria o nó pai
        $$->type = function_entry->type; 

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
        $$->type = $1->type;
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
        //Verificacao tipo retorno
        if (!tipo_compativel($2->type, $4)) {
            fprintf(stderr, "Erro semântico: tipo de retorno incompatível com a declaração da função na linha %d.\n", yylineno);
            exit(ERR_WRONG_TYPE); 
        }

        if (!tipo_compativel(current_function->type, $2->type)) {
            fprintf(stderr, "Erro semântico: tipo de retorno incompatível com a declaração da função na linha %d.\n", yylineno);
            exit(ERR_WRONG_TYPE);  
        }

        //Coleta dos componentes 
        iloc_operation_list_t *code_expr = $2->code;
        char *reg_expr = $2->result_reg;

        /*O comando return tem um filho, que é uma expressão.*/
        $$ = asd_new("retorna");
        $$->type = $2->type;
        asd_add_child($$, $2);

        //Geracao codigo ILOC
        $$->code = generate_return_code(code_expr, reg_expr);

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
        //Verificacao da expressao de teste
        if (!tipo_compativel($3->type, S_INTEGER) && !tipo_compativel($3->type, S_FLOAT)) {
             fprintf(stderr, "Erro semântico: expressão de teste condicional inválida na linha %d.\n", yylineno);
             exit(ERR_WRONG_TYPE); 
        }
        
        //Verificacao de blocos (compatibilidade entre bloco_comandos ($5) e senao_opcional ($6))

        SYMBOL_TYPE if_type, else_type;

        if ($5 != NULL) {
            if_type = $5->type;
        } else {
            if_type = S_INTEGER;
        }

        if ($6 != NULL) {
            else_type = $6->type;
        } else {
            else_type = S_INTEGER;
        }
        
        //Se o bloco else existir, os tipos dos blocos devem ser compatíveis.
        if ($6 != NULL) {
            if (if_type != else_type) {
                fprintf(stderr, "Erro semântico: tipos dos blocos 'if' e 'else' incompatíveis na linha %d.\n", yylineno);
                exit(ERR_WRONG_TYPE); 
            }
        }

        //Coleta componentes de teste 
        iloc_operation_list_t *code_test = $3->code;
        char *reg_test = $3->result_reg;
        
        //Coleta o codigo do bloco if
        iloc_operation_list_t *code_if = NULL;
        if($5 != NULL){
            code_if = $5->code;
        }
        
        //Coleta o codigo do bloco else (senao_opcional/$6)
        iloc_operation_list_t *code_else = NULL;
        if ($6 != NULL) {
            code_else = $6->code;
        }

        /*O comando if com else opcional deve ter pelo menos três filhos,
        um para a expressão, outro para o primeiro comando quando verdade,
        e o último – opcional – para o segundocomando quando falso.*/

        $$ = asd_new("se");
        $$->type = $3->type;
        asd_add_child($$, $3); //Primeiro filho: condicao (expressao)

        if ($5 != NULL) {
            asd_add_child($$, $5); //Segundo filho: bloco true (bloco_comandos)
        }
        
        if ($6 != NULL) {
            asd_add_child($$, $6); //Terceiro filho (opcional): bloco else (senao_opcional)
        }

        //Geracao codigo ILOC
        $$->code = generate_if_else_code(code_test, reg_test, code_if, code_else);
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
        //Verificacao da expressao de teste
        if (!tipo_compativel($3->type,S_INTEGER) && tipo_compativel($3->type, S_FLOAT)) {
             fprintf(stderr, "Erro semântico: expressão de teste de 'enquanto' inválida na linha %d.\n", yylineno);
             exit(ERR_WRONG_TYPE); 
        }

        //Coleta componentes de teste para a condicao
        iloc_operation_list_t *code_test = $3->code;
        char *reg_test = $3->result_reg; 

        //Coleta componente do corpo (bloco de comandos/$5)
        iloc_operation_list_t *code_body = NULL;
        if ($5 != NULL){
            code_body = $5->code;
        }

        /*O comando while deve ter pelo menos dois filhos, um para expressão e outro
        para o primeiro comando do laço.*/
        $$ = asd_new("enquanto");
        $$->type = $3->type;
        asd_add_child($$, $3); //Primeiro filho: condicao (expressao)

        if ($5 != NULL) {
            asd_add_child($$, $5); //Segundo filho: bloco_comandos   
        }

        //Geracao codigo ILOC
        $$->code = generate_while_code(code_test, reg_test, code_body);
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

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_OR,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p6 {$$ = $1;}
;

expressao_p6:
    expressao_p6 '&' expressao_p5
    {
        $$ = asd_new("&");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_AND,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p5 {$$ = $1;}
;

expressao_p5:
    expressao_p5 TK_OC_EQ expressao_p4
    {
        $$ = asd_new("==");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_EQ,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p5 TK_OC_NE expressao_p4
    {
        $$ = asd_new("!=");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_NE,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p4 {$$ = $1;}
;

expressao_p4:
    expressao_p4 '<' expressao_p3
    {
        $$ = asd_new("<");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_LT,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p4 '>' expressao_p3
    {
        $$ = asd_new(">");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_GT,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p4 TK_OC_LE expressao_p3
    {
        $$ = asd_new("<=");
        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_LE,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p4 TK_OC_GE expressao_p3
    {
        $$ = asd_new(">=");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_CMP_GE,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p3 {$$ = $1;}
;

expressao_p3:
    expressao_p3 '+' expressao_p2
    {
        $$ = asd_new("+");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;
        
        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_ADD,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p3 '-' expressao_p2
    {
        $$ = asd_new("-");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_SUB,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg) 
        );
    }
    | expressao_p2  {$$ = $1;}
;

expressao_p2:
    expressao_p2 '*' expressao_p1
    {
        $$ = asd_new("*");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_MULT,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p2 '/' expressao_p1
    {
        $$ = asd_new("/");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);

        //Geracao codigo ILOC
        $$->code = generate_binary_operation(
            ILOC_DIV,
            $1->code,
            $1->result_reg,
            $3->code,
            $3->result_reg,
            &($$->result_reg) 
        );
    }
    | expressao_p2 '%' expressao_p1
    {
        $$ = asd_new("%");

        //Verificacao de tipo
        SYMBOL_TYPE result_type = infer_type($1->type, $3->type);
        
        //Inferencia
        $$->type = result_type;

        asd_add_child($$, $1);
        asd_add_child($$, $3);
    }
    | expressao_p1  {$$ = $1;}
;

expressao_p1:
    '+' expressao_p1
    {
        $$ = asd_new("+");
        $$->type = $2->type;
        asd_add_child($$, $2);

        //Geracao codigo ILOC
        $$->code = generate_unary_operation(
            X86_MOVL,
            $2->code,
            $2->result_reg,
            &($$->result_reg)
        );
    }
    | '-' expressao_p1
    {
        $$ = asd_new("-");
        $$->type = $2->type;
        asd_add_child($$, $2);

        //Geracao codigo ILOC
        $$->code = generate_unary_operation(
            ILOC_RSUBI, 
            $2->code,
            $2->result_reg,
            &($$->result_reg)
        );
    }
    | '!' expressao_p1
    {
        $$ = asd_new("!");
        $$->type = $2->type;
        asd_add_child($$, $2);

        //Geracao codigo ILOC
        $$->code = generate_unary_operation(
            ILOC_XORI, 
            $2->code,
            $2->result_reg,
            &($$->result_reg)
        );
    }
    | expressao_p0 {$$ = $1;}
;


expressao_p0:
    TK_ID
    {
        //Verificacao: checa se a funcao ja foi declarada no escopo atual e global
        symbol_table_entry *id_entry = find_symbol(stack, $1.token_value, S_IDENTIFIER);
        
        if (id_entry == NULL) {
            //Verificacao de natureza (item 2.3)
            symbol_table_entry *function_entry = find_symbol(stack, $1.token_value, S_FUNCTION);
            if (function_entry != NULL) {
                fprintf(stderr, "Erro semântico: função '%s' usada como variável na linha %d.\n", $1.token_value, $1.line_number);
                exit(ERR_FUNCTION); 
            }

            fprintf(stderr, "Erro semântico: símbolo '%s' não declarado na linha %d.\n", $1.token_value, $1.line_number);
            exit(ERR_UNDECLARED); 
        }

        $$ = asd_new($1.token_value);

        //Anotacao de tipo
        $$->type = id_entry->type;

        $$->code = generate_load_variable(id_entry, &($$->result_reg));

        free($1.token_value);

    }
    | TK_LI_INTEIRO
    {
        $$ = asd_new($1.token_value);

        //Anotacao de tipo
        $$->type = S_INTEGER;

        /*!Colocar em uma funcao auxiliar depois!*/

        //Gerecao de registrador temporario e operacao
        char *temp_result = generate_temp_register();
        iloc_operation_t *loadI_op = make_operation(ILOC_LOADI);

        //Operandos: fonte = literal, destino = novo registrador
        loadI_op->first_operand = strdup($1.token_value);
        loadI_op->second_operand = strdup(temp_result);

        //Criacao e anexo lista ILOC
        $$->code = make_operation_list_node();
        add_operation_to_list($$->code, loadI_op);

        //Propagacao (informa ao no pai onde o resultado foi colocado)
        $$->result_reg = temp_result;

        free($1.token_value);
    }
    | TK_LI_DECIMAL
    {
        $$ = asd_new($1.token_value);

        //Anotacao de tipo
        $$->type = S_FLOAT;

        free($1.token_value);
    }
    | chamada_funcao {$$ = $1;}
    | '(' expressao ')' {$$ = $2;}
;

%%

void yyerror (const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", yylineno, s);
}