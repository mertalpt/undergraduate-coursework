%option noyywrap

DIGIT [0-9]
NONZERO_DIG [1-9]
NUMBER [1-9][0-9]+
LETTER [a-zA-Z]
ID_CHAR [_a-zA-Z]

%x ML_COMMENT

%%
%{
    int nesting_level = 0;
%}

"</"                BEGIN(ML_COMMENT); ++nesting_level;
"<//".*

<ML_COMMENT>"<//"
<ML_COMMENT>[^</>]*
<ML_COMMENT>"</"    ++nesting_level;
<ML_COMMENT>"/>"    if (--nesting_level == 0) BEGIN(INITIAL);
<ML_COMMENT>[</>]

"start_funcs" printf("FNC_BEG ");
"end_funcs" printf("FNC_END ");
"var" printf("VAR ");
"for" printf("FOR ");
"while" printf("WHILE ");
"when" printf("WHEN ");
"otherwise" printf("OTHERWISE ");
"func" printf("FNC_DEF ");
"list" printf("LIST_INIT ");
"ignore" printf("IGNORE ");

"move" printf("MOVE ");
"turn" printf("TURN ");
"grab" printf("GRAB ");
"release" printf ("RELEASE ");
"readSensor" printf("RDSENS ");
"sendData" printf("SDDATA ");
"receiveData" printf("RCDATA ");
"print" printf("PRINT ");
"scan" printf("SCAN ");
"wait" printf("WAIT ");
"time" printf("TIME ");

"->" printf("MAP_OP ");
":=" printf("ASSIGN_OP ");
"~" printf("INV_OP ");
"+" printf("ADD_OP ");
"-" printf("SUB_OP ");
"*" printf("MULT_OP ");
"/" printf("DIV_OP ");
"%" printf("MOD_OP ");
"=" printf("EQ_OP ");
"~=" printf("INEQ_OP ");
"<" printf("LT_OP ");
">" printf("GT_OP ");
"<=" printf("LTE_OP ");
">=" printf("GTE_OP ");
"&" printf("BAND_OP ");
"&&" printf("LAND_OP ");
"|" printf("BOR_OP ");
"||" printf("LOR_OP ");
"^" printf("XOR_OP ");
"+=" printf("INC_BY ");
"-=" printf("DEC_BY ");
"*=" printf("MULT_BY ");
"/=" printf("DIV_BY ");
"++" printf("INC ");
"--" printf("DEC ");

[+-]?({NUMBER}|{DIGIT}) printf("INT_LITERAL ");
[+-]?({NUMBER}|{DIGIT})"."({DIGIT}+) printf("FLOAT_LITERAL ");
(\"([^\"\\\n]|\\\"|\\n|\\t|\\\\)*\")|('([^'\\\n]|\\'|\\n|\\t|\\\\)*') printf("STRING_LITERAL ");
{ID_CHAR}({DIGIT}|{ID_CHAR})* printf("ID ");

"," printf("COMMA ");
"(" printf("LP ");
")" printf("RP ");
"{" printf("LC ");
"}" printf("RC ");
"[" printf("LSQ ");
"]" printf("RSQ ");
";" printf("SEMI_COL ");
":" printf("COLON ");
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
