
DIGIT [0-9]
NONZERO_DIG [1-9]
NUMBER [1-9][0-9]+
LETTER [a-zA-Z]
ID_CHAR [_a-zA-Z]
NL           \n

%x ML_COMMENT

%%
%{
    int nesting_level = 0;
%}

"</"                BEGIN(ML_COMMENT); ++nesting_level;
"<//".*

<ML_COMMENT>"<//"
<ML_COMMENT>[^</>\n]*
<ML_COMMENT>"\n"    { extern int lineno; ++lineno; }
<ML_COMMENT>"</"    ++nesting_level;
<ML_COMMENT>"/>"    if (--nesting_level == 0) BEGIN(INITIAL);
<ML_COMMENT>[</>]

"start_funcs" return(FNC_BEG);
"end_funcs" return(FNC_END);
"var" return(VAR);
"for" return(FOR);
"while" return(WHILE);
"when" return(WHEN);
"otherwise" return(OTHERWISE);
"func" return(FNC_DEF);
"list" return(LIST_INIT);
"ignore" return(IGNORE);

"move" return(MOVE);
"turn" return(TURN);
"grab" return(GRAB);
"release" return(RELEASE);
"readSensor" return(RDSENS);
"sendData" return(SDDATA);
"receiveData" return(RCDATA);
"print" return(PRINT);
"scan" return(SCAN);
"wait" return(WAIT);
"time" return(TIME);

"->" return(MAP_OP);
":=" return(ASSIGN_OP);
"~" return(INV_OP);
"+" return(ADD_OP);
"-" return(SUB_OP);
"*" return(MULT_OP);
"/" return(DIV_OP);
"%" return(MOD_OP);
"=" return(EQ_OP);
"~=" return(INEQ_OP);
"<" return(LT_OP);
">" return(GT_OP);
"<=" return(LTE_OP);
">=" return(GTE_OP);
"&" return(BAND_OP);
"&&" return(LAND_OP);
"|" return(BOR_OP);
"||" return(LOR_OP);
"^" return(XOR_OP);
"+=" return(INC_BY);
"-=" return(DEC_BY);
"*=" return(MULT_BY);
"/=" return(DIV_BY);
"++" return(INC);
"--" return(DEC);

[+-]?({NUMBER}|{DIGIT}) return(INT_LITERAL);
[+-]?({NUMBER}|{DIGIT})"."({DIGIT}+) return(FLOAT_LITERAL);
(\"([^\"\\\n]|\\\"|\\n|\\t|\\\\)*\")|('([^'\\\n]|\\'|\\n|\\t|\\\\)*') return(STRING_LITERAL);
{ID_CHAR}({DIGIT}|{ID_CHAR})* return(ID);

"," return(COMMA);
"(" return(LP);
")" return(RP);
"{" return(LC);
"}" return(RC);
"[" return(LSQ);
"]" return(RSQ);
";" return(SEMI_COL);
":" return(COLON);

{NL}         { extern int lineno; ++lineno; }
.            ;
%%

int yywrap() {return 1;}
