%option noyywrap

DIGIT [0-9]
NUMBER [1-9][0-9]+
LETTER [a-zA-Z]
FNC_BEG start_funcs

%%
"start_funcs" printf("FNC_BEG ");
"end_funcs" printf("FNC_END ");
"var" printf("VAR ");
"for" printf("FOR ");
"while" printf("WHILE ");
"when" printf("WHEN ");
"otherwise" printf("OTHERWISE ");
"func" printf("FNC_DEF ");
"list" printf("LIST_INIT ");

"print"|"scan"|"move"|"turn"|"grab"|"release"|"readSensor"|"sendData"|"receiveData"|"wait" printf("PRIM_FUNC ");

"</" printf("MLC_OPEN ");
"/>" printf("MLC_CLOSE ");
"<//" printf("SLC_START ");

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
[+-]?({NUMBER}|{DIGIT})?"."({DIGIT}+) printf("FLOAT_LITERAL ");
(\"([^\"\\\n]|\\\"|\\n|\\t|\\\\)*\")|('([^'\\\n]|\\'|\\n|\\t|\\\\)*') printf("STRING_LITERAL ");
{LETTER}({DIGIT}|{LETTER}|_)* printf("ID ");

[\t\ ];
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

