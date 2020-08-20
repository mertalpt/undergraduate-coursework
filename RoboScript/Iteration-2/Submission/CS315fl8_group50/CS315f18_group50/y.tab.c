/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20170709

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 4 "CS315f18_group50.yacc"
#include <stdio.h>
#line 23 "y.tab.c"

#if ! defined(YYSTYPE) && ! defined(YYSTYPE_IS_DECLARED)
/* Default: YYSTYPE is the semantic value type. */
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define FNC_BEG 257
#define FNC_END 258
#define VAR 259
#define FOR 260
#define WHILE 261
#define WHEN 262
#define OTHERWISE 263
#define FNC_DEF 264
#define LIST_INIT 265
#define IGNORE 266
#define MOVE 267
#define TURN 268
#define GRAB 269
#define RELEASE 270
#define RDSENS 271
#define RCDATA 272
#define SDDATA 273
#define PRINT 274
#define SCAN 275
#define WAIT 276
#define TIME 277
#define MAP_OP 278
#define ASSIGN_OP 279
#define INV_OP 280
#define ADD_OP 281
#define SUB_OP 282
#define MULT_OP 283
#define DIV_OP 284
#define MOD_OP 285
#define EQ_OP 286
#define INEQ_OP 287
#define LT_OP 288
#define GT_OP 289
#define LTE_OP 290
#define GTE_OP 291
#define BAND_OP 292
#define LAND_OP 293
#define BOR_OP 294
#define LOR_OP 295
#define XOR_OP 296
#define INC_BY 297
#define DEC_BY 298
#define MULT_BY 299
#define DIV_BY 300
#define INC 301
#define DEC 302
#define INT_LITERAL 303
#define FLOAT_LITERAL 304
#define STRING_LITERAL 305
#define ID 306
#define COMMA 307
#define LP 308
#define RP 309
#define LC 310
#define RC 311
#define LSQ 312
#define RSQ 313
#define SEMI_COL 314
#define COLON 315
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    1,    1,    1,    3,    3,    2,    2,
    5,    5,    7,    7,    8,    8,   10,   10,   10,   10,
   10,   11,   12,   15,   15,   15,   18,   19,   19,    4,
   20,   20,   21,   21,   22,   22,   23,   23,   24,   24,
   25,   25,   25,   27,   27,   28,   28,   29,    6,   30,
   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
   14,   14,   41,   42,   42,   42,   42,   42,   42,   42,
   42,   42,   42,   42,   13,   13,   13,   44,   44,   44,
   44,   44,   45,   45,   17,   17,   16,   16,   16,   43,
   43,   26,    9,   46,   46,   47,   47,   48,   48,   49,
   49,   50,   50,   51,   51,   51,   52,   52,   52,   52,
   52,   53,   53,   53,   54,   54,   54,   54,   55,   55,
};
static const YYINT yylen[] = {                            2,
    1,    4,    4,    4,    4,    1,    3,    2,    2,    1,
    1,    1,   11,    1,    7,   11,    1,    1,    1,    1,
    1,    7,   13,    2,    4,    4,    4,    1,    1,    0,
    3,    1,    1,    1,    1,    1,    3,    1,    1,    1,
    1,    1,    1,    3,    1,    1,    1,    8,    6,    8,
    8,    7,    7,    9,   11,   13,    8,    8,    8,    8,
    1,    1,    9,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    2,    3,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    4,    1,    1,    3,    1,    3,    1,    3,    1,
    3,    1,    3,    1,    3,    3,    1,    3,    3,    3,
    3,    3,    3,    1,    1,    3,    3,    3,    1,    2,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    1,    6,    0,    0,    0,   10,    0,    0,
    9,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    2,    5,    0,   11,   12,   14,   17,   18,   19,   20,
   21,   91,   64,   65,   66,   67,   68,   69,   70,   71,
   72,   73,   74,   61,   62,    0,    3,    4,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   79,
   80,   81,   82,   83,   84,    0,   76,    0,   34,   33,
    0,    0,    0,    0,    0,    0,   28,   29,    0,    0,
  119,   89,   88,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   86,   85,    0,    0,    7,
    0,   77,   75,    0,    0,   49,   25,   26,    0,  120,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,   35,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   92,   40,    0,   39,    0,    0,   31,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  116,  117,  118,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   37,    0,
   27,    0,    0,   22,    0,    0,    0,    0,   52,   53,
    0,    0,    0,    0,   41,    0,    0,   43,    0,    0,
    0,   48,    0,    0,    0,   50,   51,    0,    0,    0,
   57,   58,   59,   60,   47,    0,   46,    0,    0,    0,
    0,   54,    0,    0,    0,   63,    0,    0,    0,    0,
    0,    0,   44,    0,   13,    0,   16,    0,   55,    0,
    0,    0,   23,    0,   56,    0,    0,   15,
};
static const YYINT yydgoto[] = {                          2,
    3,    6,   31,   89,   33,    8,   34,   35,  127,   36,
   37,   38,   39,   40,   41,  101,  128,  132,  102,   90,
   91,  174,  175,  176,  266,  103,  267,  268,   14,   43,
   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,
   54,   55,   56,   86,   87,  104,  105,  106,  107,  108,
  109,  110,  111,  112,  113,
};
static const YYINT yysindex[] = {                      -237,
 -234,    0,    0,    0, -268, -245, -192,    0, -229, -214,
    0, -214, -209, -205, -212, -198, -193, -154, -183, -152,
 -150, -128, -125, -117, -115, -111,  -88,  -79,  -73, -276,
    0,    0,  -97,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -106,    0,    0, -145, -214,
  -22,  -44, -176, -176,  -41,  -39,  -36,  -35,  -32,  -31,
  -29,  -28,  -27,  -26,  -23, -215,    6, -214,   45,    0,
    0,    0,    0,    0,    0, -215,    0,   10,    0,    0,
    9,    8, -184,   11,   36,   14,    0,    0,   11,   12,
    0,    0,    0,   30,   59,   60,   61,   67, -275, -206,
 -207, -241,    0,   49, -153, -153,   51,   52, -153, -153,
 -153, -153,   54, -153,   56,    0,    0,   68, -153,    0,
   70,    0,    0, -145,  105,    0,    0,    0,  -69,    0,
   74, -176, -176, -176, -176, -176, -176, -176, -176, -176,
 -176, -176, -176, -176, -176, -176, -176,   80,    0,    0,
   83,   85,  119,  120,   90,   94,   96,   91,  127,  101,
  133,    0,    0,  106,    0,  103,  116,    0,  117,  123,
 -214,   59,   60,   61,   67, -275, -206, -206, -207, -207,
 -207, -207, -241, -241,    0,    0,    0, -214,  154,  166,
  126,  132,  175, -153, -153,  176,  138,  181,  152, -153,
  186,  153, -145,  -69,  157,  167,  169,  178,  187,  185,
  189,  188,  191,  193,  198, -257,  200, -257,    0,  204,
    0,  205,  172,    0,  254,  255,  210,  211,    0,    0,
 -257,  243,  244,  214,    0,   11,  215,    0,  216,  217,
 -257,    0,  -69,  218,  219,    0,    0,  221,  223,  224,
    0,    0,    0,    0,    0,  220,    0,  225,  226, -214,
  271,    0, -257, -257, -257,    0,  227,  228,  230,  229,
  234,  233,    0, -214,    0, -176,    0, -257,    0,  232,
  235,  236,    0,  237,    0, -214,  238,    0,
};
static const YYINT yyrindex[] = {                       536,
  288,    0,    0,    0,    0,    0,    0,    0,    0,  536,
    0,  536,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -76,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  239,    0,
 -247,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    1, -180,    0,
    0,    0,    0,    0,    0,    0,    0,  241,    0,    0,
    0,    0,    0,  -76,    0,    0,    0,    0, -147,    0,
    0,    0,    0, -200,  -55,  179,  168,  156,  128, -216,
  -81, -112,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  239,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  242,    0,    0,    0,    0,    0,    0,
    0,  202,  196,  190,  162,  134,   93,  122,   13,   42,
   53,   82,  -43,    2,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  239,    0,    0,    0,  240,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  245,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -178,    0,    0,    0,    0,
  239,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  246,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
static const YYINT yygindex[] = {                         0,
    0,    0,   -9,    4,    0,  546, -188,  282,  -57,    0,
    0,    0,  494,    0,    0,  -98,  -78,  464,    0,  424,
  347,  -87,  351,    0, -201,  -10,  287,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  421,  422,  420,  423,
  425,   38,    7,   66,  -54,
};
#define YYTABLESIZE 571
static const YYINT yytable[] = {                         42,
    8,   42,   57,    4,    7,  100,  114,  133,  245,  217,
  147,  148,   10,   32,  137,   58,  160,  160,    5,    1,
  160,  160,  160,  160,  247,  160,  250,  161,  162,    5,
  160,  165,  166,  167,  168,   76,  170,    9,   77,  258,
  180,  155,  156,  157,   15,   16,   17,   18,  246,   42,
   92,   42,   19,   20,   21,   22,   23,   24,   25,   26,
   27,   28,   29,   24,   96,   12,   24,   42,  130,  104,
  104,  281,  282,  153,  154,  104,  104,  104,  104,  104,
  131,  149,  150,  151,  152,   13,  292,   97,   98,  126,
   99,   30,  104,   61,  104,   96,  104,  104,   59,   78,
  195,  196,  197,   96,   60,  160,  160,  217,   93,   62,
   93,  160,   93,   93,   63,  233,  223,  224,   97,   98,
  126,   99,   78,   78,   78,   78,   97,   98,   42,   99,
   42,   65,  173,   87,   87,   87,   87,   87,   87,   87,
   87,   87,   87,   87,   87,   87,   87,   87,   87,   97,
   98,  159,   99,   64,  269,  189,  190,  191,  192,   87,
   88,   87,   66,   87,   67,   87,   87,   87,  114,  114,
   42,  215,   79,  114,  114,  114,  114,  114,  114,  114,
  114,  114,  114,  114,  187,  188,   68,   42,  216,   69,
   80,   81,   82,   83,   84,   85,  114,   70,  114,   71,
  114,  114,   90,   72,  107,  107,  107,  107,  107,  107,
  107,  107,  107,  107,  107,  248,   78,  248,  193,  194,
   90,   90,   90,   90,   90,   90,   73,  107,  291,  107,
  248,  107,  107,   97,   98,   74,   99,  112,  112,   94,
  248,   75,  112,  112,  112,  112,  112,  112,  112,  112,
  112,  112,  112,   94,  265,   94,   93,   94,   94,   42,
  278,   94,  248,  248,  248,  112,  115,  112,  116,  112,
  112,  117,  118,   42,  290,  119,  120,  248,  121,  122,
  123,  124,  113,  113,  125,   42,  297,  113,  113,  113,
  113,  113,  113,  113,  113,  113,  113,  113,  108,  108,
  108,  108,  108,  108,  108,  108,  108,  108,  108,  131,
  113,    8,  113,  129,  113,  113,  134,  135,  136,  140,
  141,  108,   76,  108,  142,  108,  108,  110,  110,  110,
  110,  110,  110,  110,  110,  110,  110,  110,  109,  109,
  109,  109,  109,  109,  109,  109,  109,  109,  109,  139,
  110,  143,  110,  144,  110,  110,  145,  158,  146,  163,
  164,  109,  169,  109,  171,  109,  109,  111,  111,  111,
  111,  111,  111,  111,  111,  111,  111,  111,  105,  105,
  172,  177,  179,  181,  105,  105,  105,  105,  105,  198,
  111,  199,  111,  200,  111,  111,  201,  202,  203,  206,
  204,  105,  205,  105,  207,  105,  105,  106,  106,  208,
  209,  211,  210,  106,  106,  106,  106,  106,  212,  102,
  102,  102,  102,  102,  213,  103,  103,  103,  103,  103,
  106,  218,  106,  220,  106,  106,  102,  214,  102,  221,
  102,  102,  103,  219,  103,  226,  103,  103,  100,  100,
  100,  100,  222,  225,  101,  101,  101,  101,  227,  228,
   98,   98,   98,  230,  100,  231,  100,  234,  100,  100,
  101,   96,  101,   96,  101,  101,   98,  235,   98,  236,
   98,   98,   99,   99,   99,  237,  253,   96,   97,   96,
   97,   96,   96,  239,  238,  241,   95,  240,   99,  242,
   99,  243,   99,   99,   97,  244,   97,  249,   97,   97,
   95,  251,   95,  252,   95,   95,  254,  255,  256,  257,
  259,  260,  261,  262,  263,  264,  275,  270,  271,  272,
  273,  274,  279,  276,  277,   30,  284,  286,  285,  287,
  288,  289,  293,  294,  295,   30,  296,   30,  298,   32,
   38,   11,  280,   11,   45,   95,  138,  178,   15,  232,
  229,  283,  182,  184,  183,    0,    0,  185,    0,    0,
  186,
};
static const YYINT yycheck[] = {                         10,
    0,   12,   12,    0,    1,   63,   64,   86,  266,  198,
  286,  287,  258,   10,   93,   12,  115,  116,  264,  257,
  119,  120,  121,  122,  226,  124,  228,  115,  116,  264,
  129,  119,  120,  121,  122,  312,  124,  306,  315,  241,
  139,  283,  284,  285,  259,  260,  261,  262,  306,   60,
   60,   62,  267,  268,  269,  270,  271,  272,  273,  274,
  275,  276,  277,  311,  280,  258,  314,   78,   78,  286,
  287,  273,  274,  281,  282,  292,  293,  294,  295,  296,
  265,  288,  289,  290,  291,  315,  288,  303,  304,  305,
  306,  306,  309,  306,  311,  280,  313,  314,  308,  280,
  155,  156,  157,  280,  310,  204,  205,  296,  309,  308,
  311,  210,  313,  314,  308,  214,  204,  205,  303,  304,
  305,  306,  303,  304,  305,  306,  303,  304,  307,  306,
  309,  315,  129,  281,  282,  283,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  303,
  304,  305,  306,  308,  253,  149,  150,  151,  152,  307,
  306,  309,  315,  311,  315,  313,  314,  315,  281,  282,
  181,  181,  279,  286,  287,  288,  289,  290,  291,  292,
  293,  294,  295,  296,  147,  148,  315,  198,  198,  315,
  297,  298,  299,  300,  301,  302,  309,  315,  311,  315,
  313,  314,  279,  315,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  295,  296,  226,  314,  228,  153,  154,
  297,  298,  299,  300,  301,  302,  315,  309,  286,  311,
  241,  313,  314,  303,  304,  315,  306,  281,  282,  295,
  251,  315,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  295,  296,  309,  251,  311,  279,  313,  314,  270,
  270,  306,  273,  274,  275,  309,  308,  311,  308,  313,
  314,  308,  308,  284,  284,  308,  308,  288,  308,  308,
  308,  308,  281,  282,  308,  296,  296,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  265,
  309,  311,  311,  308,  313,  314,  307,  309,  311,  306,
  309,  309,  312,  311,  295,  313,  314,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  314,
  309,  293,  311,  294,  313,  314,  296,  309,  292,  309,
  309,  309,  309,  311,  309,  313,  314,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  286,  287,
  313,  312,  278,  310,  292,  293,  294,  295,  296,  310,
  309,  309,  311,  309,  313,  314,  278,  278,  309,  309,
  307,  309,  307,  311,  278,  313,  314,  286,  287,  309,
  278,  309,  307,  292,  293,  294,  295,  296,  303,  292,
  293,  294,  295,  296,  308,  292,  293,  294,  295,  296,
  309,  278,  311,  308,  313,  314,  309,  315,  311,  308,
  313,  314,  309,  278,  311,  308,  313,  314,  293,  294,
  295,  296,  278,  278,  293,  294,  295,  296,  278,  308,
  293,  294,  295,  278,  309,  313,  311,  311,  313,  314,
  309,  293,  311,  295,  313,  314,  309,  311,  311,  311,
  313,  314,  293,  294,  295,  308,  315,  309,  293,  311,
  295,  313,  314,  309,  308,  308,  295,  309,  309,  309,
  311,  309,  313,  314,  309,  308,  311,  308,  313,  314,
  309,  308,  311,  309,  313,  314,  263,  263,  309,  309,
  278,  278,  309,  309,  309,  309,  307,  310,  310,  309,
  308,  308,  262,  309,  309,    0,  310,  308,  311,  311,
  307,  309,  311,  309,  309,  258,  310,  309,  311,  309,
  309,    6,  271,  314,  309,   62,   93,  134,  314,  213,
  210,  275,  142,  144,  143,   -1,   -1,  145,   -1,   -1,
  146,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 315
#define YYUNDFTOKEN 373
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"FNC_BEG","FNC_END","VAR","FOR",
"WHILE","WHEN","OTHERWISE","FNC_DEF","LIST_INIT","IGNORE","MOVE","TURN","GRAB",
"RELEASE","RDSENS","RCDATA","SDDATA","PRINT","SCAN","WAIT","TIME","MAP_OP",
"ASSIGN_OP","INV_OP","ADD_OP","SUB_OP","MULT_OP","DIV_OP","MOD_OP","EQ_OP",
"INEQ_OP","LT_OP","GT_OP","LTE_OP","GTE_OP","BAND_OP","LAND_OP","BOR_OP",
"LOR_OP","XOR_OP","INC_BY","DEC_BY","MULT_BY","DIV_BY","INC","DEC",
"INT_LITERAL","FLOAT_LITERAL","STRING_LITERAL","ID","COMMA","LP","RP","LC","RC",
"LSQ","RSQ","SEMI_COL","COLON",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : start",
"start : program",
"program : FNC_BEG func_block FNC_END stmts",
"program : FNC_BEG empty FNC_END stmts",
"program : FNC_BEG empty FNC_END empty",
"program : FNC_BEG func_block FNC_END empty",
"program : empty",
"stmts : stmt SEMI_COL stmts",
"stmts : stmt SEMI_COL",
"func_block : func_block func_def",
"func_block : func_def",
"stmt : matched",
"stmt : unmatched",
"matched : WHEN LP conditional_exp RP LC stmts RC OTHERWISE LC stmts RC",
"matched : non_when",
"unmatched : WHEN LP conditional_exp RP LC stmts RC",
"unmatched : WHEN LP conditional_exp RP LC matched RC OTHERWISE LC unmatched RC",
"non_when : while_loop",
"non_when : for_loop",
"non_when : assignment",
"non_when : func_call",
"non_when : var_declaration",
"while_loop : WHILE LP conditional_exp RP LC stmts RC",
"for_loop : FOR LP assignment SEMI_COL operand COLON operand COLON operand RP LC stmts RC",
"var_declaration : VAR ID",
"var_declaration : VAR ID ASSIGN_OP assignable",
"var_declaration : VAR ID ASSIGN_OP empty_list",
"empty_list : LIST_INIT LSQ INT_LITERAL RSQ",
"numeric_literal : INT_LITERAL",
"numeric_literal : FLOAT_LITERAL",
"empty :",
"ne_params : ID COMMA ne_params",
"ne_params : ID",
"func_params : ne_params",
"func_params : empty",
"in_arg : operand",
"in_arg : STRING_LITERAL",
"in_args : in_arg COMMA in_args",
"in_args : in_arg",
"func_in_list : in_args",
"func_in_list : empty",
"out_arg : IGNORE",
"out_arg : ID",
"out_arg : list_access",
"out_args : out_arg COMMA out_args",
"out_args : out_arg",
"func_out_list : out_args",
"func_out_list : empty",
"func_map : COLON LP func_params RP MAP_OP LP func_params RP",
"func_def : FNC_DEF ID func_map LC stmts RC",
"prim_move : MOVE COLON LP in_arg RP MAP_OP LP RP",
"prim_turn : TURN COLON LP in_arg RP MAP_OP LP RP",
"prim_grab : GRAB COLON LP RP MAP_OP LP RP",
"prim_release : RELEASE COLON LP RP MAP_OP LP RP",
"prim_rdsens : RDSENS COLON LP in_arg RP MAP_OP LP out_arg RP",
"prim_sddata : SDDATA COLON LP in_arg COMMA in_arg RP MAP_OP LP out_arg RP",
"prim_rcdata : RCDATA COLON LP in_arg COMMA in_arg RP MAP_OP LP out_arg COMMA out_arg RP",
"prim_print : PRINT COLON LP in_arg RP MAP_OP LP RP",
"prim_scan : SCAN COLON LP RP MAP_OP LP out_arg RP",
"prim_wait : WAIT COLON LP in_arg RP MAP_OP LP RP",
"prim_time : TIME COLON LP RP MAP_OP LP out_arg RP",
"func_call : user_func",
"func_call : prim_func",
"user_func : ID COLON LP func_in_list RP MAP_OP LP func_out_list RP",
"prim_func : prim_move",
"prim_func : prim_turn",
"prim_func : prim_grab",
"prim_func : prim_release",
"prim_func : prim_rdsens",
"prim_func : prim_sddata",
"prim_func : prim_rcdata",
"prim_func : prim_print",
"prim_func : prim_scan",
"prim_func : prim_wait",
"prim_func : prim_time",
"assignment : left_hand_side assign_op assignable",
"assignment : left_hand_side crement_op",
"assignment : left_hand_side ASSIGN_OP empty_list",
"assign_op : ASSIGN_OP",
"assign_op : INC_BY",
"assign_op : DEC_BY",
"assign_op : MULT_BY",
"assign_op : DIV_BY",
"crement_op : INC",
"crement_op : DEC",
"assignable : conditional_exp",
"assignable : STRING_LITERAL",
"operand : ID",
"operand : list_access",
"operand : numeric_literal",
"left_hand_side : ID",
"left_hand_side : list_access",
"list_access : ID LSQ assignable RSQ",
"conditional_exp : conditional_or_exp",
"conditional_or_exp : conditional_and_exp",
"conditional_or_exp : conditional_or_exp LOR_OP conditional_and_exp",
"conditional_and_exp : inclusive_or",
"conditional_and_exp : conditional_and_exp LAND_OP inclusive_or",
"inclusive_or : xor_exp",
"inclusive_or : inclusive_or BOR_OP xor_exp",
"xor_exp : inclusive_and_exp",
"xor_exp : xor_exp XOR_OP inclusive_and_exp",
"inclusive_and_exp : equal_exp",
"inclusive_and_exp : inclusive_and_exp BAND_OP equal_exp",
"equal_exp : relational_exp",
"equal_exp : equal_exp EQ_OP relational_exp",
"equal_exp : equal_exp INEQ_OP relational_exp",
"relational_exp : sum_exp",
"relational_exp : relational_exp LT_OP sum_exp",
"relational_exp : relational_exp LTE_OP sum_exp",
"relational_exp : relational_exp GT_OP sum_exp",
"relational_exp : relational_exp GTE_OP sum_exp",
"sum_exp : sum_exp ADD_OP multiply_exp",
"sum_exp : sum_exp SUB_OP multiply_exp",
"sum_exp : multiply_exp",
"multiply_exp : negate_exp",
"multiply_exp : multiply_exp MULT_OP negate_exp",
"multiply_exp : multiply_exp DIV_OP negate_exp",
"multiply_exp : multiply_exp MOD_OP negate_exp",
"negate_exp : operand",
"negate_exp : INV_OP ID",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 189 "CS315f18_group50.yacc"
#include "lex.yy.c"
int lineno = 1;

int main()
{
    yyparse();
    return 0;
}

int yyerror(char *s){fprintf(stderr, "Line: %d, %s \n", lineno, s);}
#line 581 "y.tab.c"

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yym = 0;
    yyn = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        yychar = YYLEX;
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);

    switch (yyn)
    {
case 1:
#line 12 "CS315f18_group50.yacc"
	{printf("The program is accepted.\n");}
break;
#line 784 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            yychar = YYLEX;
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
