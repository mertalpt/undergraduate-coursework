/* parser.y */

%{
#include <stdio.h>
%}

%token FNC_BEG FNC_END VAR FOR WHILE WHEN OTHERWISE FNC_DEF LIST_INIT IGNORE MOVE TURN GRAB RELEASE RDSENS RCDATA SDDATA PRINT SCAN WAIT TIME MAP_OP ASSIGN_OP INV_OP ADD_OP SUB_OP MULT_OP DIV_OP MOD_OP EQ_OP INEQ_OP LT_OP GT_OP LTE_OP GTE_OP BAND_OP LAND_OP BOR_OP LOR_OP XOR_OP INC_BY DEC_BY MULT_BY DIV_BY INC DEC INT_LITERAL FLOAT_LITERAL STRING_LITERAL ID COMMA LP RP LC RC LSQ RSQ SEMI_COL COLON

%expect 1

%%

start : program {printf("The program is accepted.\n");}

program : FNC_BEG func_block FNC_END stmts
        | FNC_BEG empty FNC_END stmts
        | FNC_BEG empty FNC_END empty
        | FNC_BEG func_block FNC_END empty
        | empty

stmts : stmt SEMI_COL stmts
      | stmt SEMI_COL

func_block : func_block func_def
           | func_def

stmt : matched
     | unmatched

matched : WHEN LP conditional_exp RP LC stmts RC OTHERWISE LC stmts RC
        | non_when

unmatched : WHEN LP conditional_exp RP LC stmts RC
          | WHEN LP conditional_exp RP LC matched RC OTHERWISE LC unmatched RC

non_when : while_loop
         | for_loop
         | assignment
         | func_call
         | var_declaration

while_loop : WHILE LP conditional_exp RP LC stmts RC

for_loop : FOR LP assignment SEMI_COL operand COLON operand COLON operand RP LC stmts RC

var_declaration : VAR ID
                | VAR ID ASSIGN_OP assignable
                | VAR ID ASSIGN_OP empty_list

empty_list : LIST_INIT LSQ INT_LITERAL RSQ

numeric_literal : INT_LITERAL
                | FLOAT_LITERAL

empty :

ne_params : ID COMMA ne_params
          | ID

func_params : ne_params
            | empty

in_arg : operand
       | STRING_LITERAL

in_args : in_arg COMMA in_args
        | in_arg

func_in_list : in_args
             | empty

out_arg : IGNORE
        | ID
        | list_access

out_args : out_arg COMMA out_args
         | out_arg

func_out_list : out_args
              | empty

func_map : COLON LP func_params RP MAP_OP LP func_params RP

func_def : FNC_DEF ID func_map LC stmts RC

prim_move : MOVE COLON LP in_arg RP MAP_OP LP RP

prim_turn : TURN COLON LP in_arg RP MAP_OP LP RP

prim_grab : GRAB COLON LP RP MAP_OP LP RP

prim_release : RELEASE COLON LP RP MAP_OP LP RP

prim_rdsens : RDSENS COLON LP in_arg RP MAP_OP LP out_arg RP

prim_sddata : SDDATA COLON LP in_arg COMMA in_arg RP MAP_OP LP out_arg RP

prim_rcdata : RCDATA COLON LP in_arg COMMA in_arg RP MAP_OP LP out_arg COMMA out_arg RP

prim_print : PRINT COLON LP in_arg RP MAP_OP LP RP

prim_scan : SCAN COLON LP RP MAP_OP LP out_arg RP

prim_wait : WAIT COLON LP in_arg RP MAP_OP LP RP

prim_time : TIME COLON LP RP MAP_OP LP out_arg RP

func_call : user_func
          | prim_func

user_func : ID COLON LP func_in_list RP MAP_OP LP func_out_list RP

prim_func : prim_move
          | prim_turn
          | prim_grab
          | prim_release
          | prim_rdsens
          | prim_sddata
          | prim_rcdata
          | prim_print
          | prim_scan
          | prim_wait
          | prim_time

assignment : left_hand_side assign_op assignable
           | left_hand_side crement_op
           | left_hand_side ASSIGN_OP empty_list

assign_op : ASSIGN_OP
          | INC_BY
          | DEC_BY
          | MULT_BY
          | DIV_BY

crement_op : INC
           | DEC

assignable : conditional_exp
           | STRING_LITERAL

operand : ID
        | list_access
        | numeric_literal

left_hand_side : ID
               | list_access

list_access : ID LSQ assignable RSQ

conditional_exp : conditional_or_exp

conditional_or_exp : conditional_and_exp
                   | conditional_or_exp LOR_OP conditional_and_exp

conditional_and_exp : inclusive_or
                    | conditional_and_exp LAND_OP inclusive_or

inclusive_or : xor_exp
             | inclusive_or BOR_OP xor_exp

xor_exp : inclusive_and_exp
        | xor_exp XOR_OP inclusive_and_exp

inclusive_and_exp : equal_exp
                  | inclusive_and_exp BAND_OP equal_exp

equal_exp : relational_exp
          | equal_exp EQ_OP relational_exp
          | equal_exp INEQ_OP relational_exp

relational_exp : sum_exp
               | relational_exp LT_OP sum_exp
               | relational_exp LTE_OP sum_exp
               | relational_exp GT_OP sum_exp
               | relational_exp GTE_OP sum_exp

sum_exp : sum_exp ADD_OP multiply_exp
        | sum_exp SUB_OP multiply_exp
        | multiply_exp

multiply_exp : negate_exp
             | multiply_exp MULT_OP negate_exp
             | multiply_exp DIV_OP negate_exp
             | multiply_exp MOD_OP negate_exp

negate_exp : operand
           | INV_OP ID

%%
#include "lex.yy.c"
int lineno = 1;

int main()
{
    yyparse();
    return 0;
}

int yyerror(char *s){fprintf(stderr, "Line: %d, %s \n", lineno, s);}
