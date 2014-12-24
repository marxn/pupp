/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "puppy.y"

    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
    #include <list>
    #include "memorymgr.h"
    #include "constval.h"
    #include "expression.h"
    #include "variable.h"
    #include "node.h"
    #include "statement.h"
    #include "container.h"
    
    using namespace std;
    
    int yyerror(char *);
    int yylex(void);
    ContainerNode * final_node;


/* Line 268 of yacc.c  */
#line 92 "puppy.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OR = 258,
     AND = 259,
     NOT = 260,
     NOT_EQUAL_OP = 261,
     LE_OP = 262,
     GE_OP = 263,
     EQUAL_OP = 264,
     UMINUS = 265,
     INTEGER = 266,
     FLOAT = 267,
     BOOLEAN = 268,
     STRING = 269,
     IDENTIFIER = 270,
     TYPE_INTEGER = 271,
     TYPE_FLOAT = 272,
     TYPE_STRING = 273,
     TYPE_BOOLEAN = 274,
     TYPE_SET = 275,
     DEF = 276,
     IF = 277,
     ELSE = 278,
     WHILE = 279,
     BREAK = 280,
     CONTINUE = 281,
     FOR = 282,
     FOREACH = 283,
     IN = 284,
     DO = 285,
     AS = 286,
     PRINT = 287,
     SLEEP = 288,
     NL = 289,
     PI = 290
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 22 "puppy.y"

	DataType                      puppy_datatype;
	IntegerValue                * puppy_const_integer;
	FloatValue                  * puppy_const_float;
	BooleanValue                * puppy_const_boolean;
	StringValue                 * puppy_const_string;
	ConstValue                  * puppy_const;
	string                      * puppy_variable;
	Expression                  * puppy_expr;
	RelationExpression          * puppy_relexpr;
	ArithmeticExpression        * puppy_arithexpr;
	LogicalExpression           * puppy_logicalexpr;
	KVExpression                * puppy_kvexpr;
	SetExpression               * puppy_setexpr;
	OffsetExpression            * puppy_offsetexpr;
        Identifier                  * puppy_ident;
	Node                        * puppy_node;
	list<Node*>                 * puppy_nodelist;
	list<Identifier*>           * puppy_identlist;
	list<Expression*>           * puppy_exprlist;
	StatementNode               * puppy_statement;



/* Line 293 of yacc.c  */
#line 188 "puppy.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 200 "puppy.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   443

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNRULES -- Number of states.  */
#define YYNSTATES  145

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   290

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      44,    45,    15,    13,    48,    14,    49,    16,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    43,
       7,     6,     8,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     9,    10,    11,    12,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    13,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    39,    50,    63,    65,
      67,    69,    74,    75,    84,    88,    90,    92,    94,    96,
      98,   100,   102,   107,   111,   113,   116,   121,   125,   127,
     128,   131,   133,   135,   137,   139,   141,   143,   145,   149,
     153,   157,   161,   165,   169,   173,   177,   181,   185,   189,
     193,   197,   200,   206,   211,   213,   215,   217,   219,   221,
     223,   225,   227,   229
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    54,    -1,    54,    55,    -1,    55,    -1,
      56,    43,    -1,    60,    -1,    62,    -1,    63,    -1,    64,
      -1,    65,    -1,    67,    -1,    69,    -1,    72,    -1,    70,
      -1,    31,    44,    82,    45,    46,    54,    47,    -1,    34,
      56,    43,    82,    43,    56,    37,    46,    54,    47,    -1,
      35,     7,    22,    48,    22,     8,    36,    82,    37,    46,
      54,    47,    -1,    57,    -1,    58,    -1,    59,    -1,    30,
      46,    54,    47,    -1,    -1,    29,    44,    82,    45,    46,
      54,    47,    61,    -1,    22,     6,    82,    -1,    32,    -1,
      33,    -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,
      27,    -1,    28,    68,    38,    66,    -1,    68,    48,    22,
      -1,    22,    -1,    40,    82,    -1,    73,    49,    22,    71,
      -1,    71,    48,    82,    -1,    82,    -1,    -1,    39,    71,
      -1,    22,    -1,    41,    -1,    42,    -1,    18,    -1,    19,
      -1,    21,    -1,    20,    -1,    46,    71,    47,    -1,    82,
      13,    82,    -1,    82,    14,    82,    -1,    82,    15,    82,
      -1,    82,    16,    82,    -1,    82,     8,    82,    -1,    82,
       7,    82,    -1,    82,    12,    82,    -1,    82,     9,    82,
      -1,    82,    11,    82,    -1,    82,    10,    82,    -1,    82,
       4,    82,    -1,    82,     3,    82,    -1,     5,    82,    -1,
       7,    82,    48,    82,     8,    -1,    73,    50,    82,    51,
      -1,    74,    -1,    75,    -1,    73,    -1,    77,    -1,    78,
      -1,    79,    -1,    80,    -1,    76,    -1,    81,    -1,    44,
      82,    45,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    85,    85,    93,    98,   106,   110,   114,   121,   125,
     129,   133,   137,   141,   145,   152,   164,   183,   200,   204,
     208,   215,   220,   226,   238,   248,   255,   262,   266,   270,
     274,   278,   285,   293,   298,   306,   315,   323,   328,   334,
     340,   349,   355,   359,   365,   369,   373,   377,   384,   391,
     395,   399,   403,   410,   414,   418,   422,   426,   430,   437,
     441,   445,   452,   459,   467,   471,   475,   479,   483,   487,
     491,   495,   499,   503
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OR", "AND", "NOT", "'='", "'<'", "'>'",
  "NOT_EQUAL_OP", "LE_OP", "GE_OP", "EQUAL_OP", "'+'", "'-'", "'*'", "'/'",
  "UMINUS", "INTEGER", "FLOAT", "BOOLEAN", "STRING", "IDENTIFIER",
  "TYPE_INTEGER", "TYPE_FLOAT", "TYPE_STRING", "TYPE_BOOLEAN", "TYPE_SET",
  "DEF", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "FOR", "FOREACH",
  "IN", "DO", "AS", "PRINT", "SLEEP", "NL", "PI", "';'", "'('", "')'",
  "'{'", "'}'", "','", "'.'", "'['", "']'", "$accept", "final_block",
  "node_list", "program_node", "simple_node", "while_loop", "for_loop",
  "foreach_loop", "loop_node", "optional_else_list", "branch_node",
  "assign_statement", "break_statement", "continue_statement",
  "def_data_type", "vardefstatement", "identifier_list", "sleep_statement",
  "object_statement", "expr_list", "print_statement", "variable",
  "symbolic_constant", "const_value", "set_expr", "arith_expr", "rel_expr",
  "logical_expr", "kvexpr", "offset_expr", "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,    61,    60,    62,   261,
     262,   263,   264,    43,    45,    42,    47,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,    59,    40,    41,   123,   125,    44,    46,
      91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    54,    54,    55,    55,    55,    56,    56,
      56,    56,    56,    56,    56,    57,    58,    59,    60,    60,
      60,    61,    61,    62,    63,    64,    65,    66,    66,    66,
      66,    66,    67,    68,    68,    69,    70,    71,    71,    71,
      72,    73,    74,    74,    75,    75,    75,    75,    76,    77,
      77,    77,    77,    78,    78,    78,    78,    78,    78,    79,
      79,    79,    80,    81,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     7,    10,    12,     1,     1,
       1,     4,     0,     8,     3,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     2,     4,     3,     1,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     5,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    41,     0,     0,     0,    25,    26,     0,     0,    39,
       0,     0,     2,     4,     0,    18,    19,    20,     6,     7,
       8,     9,    10,    11,    12,    14,    13,     0,     0,    34,
       0,     0,     0,     0,     0,     0,     0,    44,    45,    47,
      46,    41,    42,    43,     0,    39,    40,    66,    64,    65,
      71,    67,    68,    69,    70,    72,    38,    35,     1,     3,
       5,     0,    24,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,     0,
      73,    48,    37,     0,    60,    59,    54,    53,    56,    58,
      57,    55,    49,    50,    51,    52,    36,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,    62,    22,    15,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
      16,     0,    21,     0,    17
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    11,    12,    13,    14,    15,    16,    17,    18,   133,
      19,    20,    21,    22,    93,    23,    30,    24,    25,    46,
      26,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -86
static const yytype_int16 yypact[] =
{
     346,     4,   -16,   -21,   -20,   -86,   -86,   -19,    20,   183,
     183,    11,   346,   -86,   -15,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -18,   183,   -86,
     -33,   183,   183,    -9,    15,   183,   183,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   183,   183,     5,     2,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   380,   380,   -86,   -86,
     -86,    33,   380,    39,    35,   141,   155,   183,     6,   417,
      87,   169,   -31,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,    12,    13,   233,    38,   183,
     -86,   -86,   380,    32,   407,   417,   427,   427,   427,   427,
     427,   427,     3,     3,   -86,   -86,     5,   346,   346,   -19,
      73,   394,   -86,   246,   266,    45,    48,   189,    55,   -86,
      40,   183,    41,   -86,   346,   247,   346,   286,    42,   306,
     -86,   346,   -86,   326,   -86
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -86,   -86,   -85,   -11,    -5,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -37,
     -86,     0,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,
      -6
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      27,    59,    33,     1,    57,    63,    29,    27,    72,     2,
      28,    58,    27,     5,     6,    64,   101,    73,    85,    86,
       9,    10,    62,    31,    32,    65,    66,    34,    60,    69,
      70,    61,   123,   124,    67,    75,    76,    68,    71,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,   137,
     116,   139,    74,    73,    98,    87,   143,    94,   117,   118,
     120,    97,    88,    89,    90,    91,    92,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   126,   130,   122,   131,   132,   134,   136,   141,     0,
      75,    76,     0,   121,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,    59,    59,   125,     0,     0,    27,    27,    27,
       0,   107,     0,    27,    27,   135,    59,     0,    59,     0,
       0,     0,    59,     0,    27,    99,    27,    27,     0,    27,
       0,    27,     0,    27,    75,    76,     0,     0,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    75,    76,
       0,     0,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    75,    76,     0,     0,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    95,     0,    35,     0,
      36,     0,     0,     0,    35,     0,    -1,     0,     0,     0,
      96,    37,    38,    39,    40,    41,     0,    37,    38,    39,
      40,    41,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    43,     0,    44,     0,    45,
      42,    43,     0,    44,     0,    45,    75,    76,     0,     0,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      75,    76,     0,     0,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     2,     3,   119,     4,     5,     6,
       7,     8,     0,     0,   138,     9,    10,     0,     1,     0,
       0,     0,     0,   128,     2,     3,     0,     4,     5,     6,
       7,     8,     0,     0,     0,     9,    10,     0,     1,     0,
       0,     0,     0,   129,     2,     3,     0,     4,     5,     6,
       7,     8,     0,     0,     0,     9,    10,     0,     1,     0,
       0,     0,     0,   140,     2,     3,     0,     4,     5,     6,
       7,     8,     0,     0,     0,     9,    10,     0,     1,     0,
       0,     0,     0,   142,     2,     3,     0,     4,     5,     6,
       7,     8,     0,     0,     0,     9,    10,     0,     1,     0,
       0,     0,     0,   144,     2,     3,     0,     4,     5,     6,
       7,     8,     0,    75,    76,     9,    10,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    75,    76,     0,
       0,    77,   127,    79,    80,    81,    82,    83,    84,    85,
      86,    76,     0,     0,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    86
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-86))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-1))

static const yytype_int16 yycheck[] =
{
       0,    12,     7,    22,    10,    38,    22,     7,    45,    28,
       6,     0,    12,    32,    33,    48,    47,    48,    15,    16,
      39,    40,    28,    44,    44,    31,    32,     7,    43,    35,
      36,    49,   117,   118,    43,     3,     4,    22,    44,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,   134,
      87,   136,    50,    48,    48,    22,   141,    22,    46,    46,
      22,    67,    23,    24,    25,    26,    27,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     8,    37,    51,    36,    30,    46,    46,    46,    -1,
       3,     4,    -1,    99,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   119,    -1,    -1,   117,   118,   119,
      -1,   127,    -1,   123,   124,   131,   137,    -1,   139,    -1,
      -1,    -1,   143,    -1,   134,    48,   136,   137,    -1,   139,
      -1,   141,    -1,   143,     3,     4,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,     3,     4,
      -1,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     3,     4,    -1,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    45,    -1,     5,    -1,
       7,    -1,    -1,    -1,     5,    -1,     7,    -1,    -1,    -1,
      45,    18,    19,    20,    21,    22,    -1,    18,    19,    20,
      21,    22,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,
      41,    42,    -1,    44,    -1,    46,     3,     4,    -1,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
       3,     4,    -1,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    28,    29,    43,    31,    32,    33,
      34,    35,    -1,    -1,    37,    39,    40,    -1,    22,    -1,
      -1,    -1,    -1,    47,    28,    29,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    39,    40,    -1,    22,    -1,
      -1,    -1,    -1,    47,    28,    29,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    39,    40,    -1,    22,    -1,
      -1,    -1,    -1,    47,    28,    29,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    39,    40,    -1,    22,    -1,
      -1,    -1,    -1,    47,    28,    29,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    39,    40,    -1,    22,    -1,
      -1,    -1,    -1,    47,    28,    29,    -1,    31,    32,    33,
      34,    35,    -1,     3,     4,    39,    40,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     3,     4,    -1,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,     4,    -1,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    22,    28,    29,    31,    32,    33,    34,    35,    39,
      40,    53,    54,    55,    56,    57,    58,    59,    60,    62,
      63,    64,    65,    67,    69,    70,    72,    73,     6,    22,
      68,    44,    44,    56,     7,     5,     7,    18,    19,    20,
      21,    22,    41,    42,    44,    46,    71,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    82,     0,    55,
      43,    49,    82,    38,    48,    82,    82,    43,    22,    82,
      82,    82,    71,    48,    50,     3,     4,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    22,    23,    24,
      25,    26,    27,    66,    22,    45,    45,    82,    48,    48,
      45,    47,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    71,    46,    46,    43,
      22,    82,    51,    54,    54,    56,     8,     8,    47,    47,
      37,    36,    30,    61,    46,    82,    46,    54,    37,    54,
      47,    46,    47,    54,    47
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 86 "puppy.y"
    {
			final_node = new ContainerNode;
			final_node->SetNodeList((yyvsp[(1) - (1)].puppy_nodelist));
		}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 94 "puppy.y"
    {
			(yyvsp[(1) - (2)].puppy_nodelist)->push_back((yyvsp[(2) - (2)].puppy_node));
			(yyval.puppy_nodelist) = (yyvsp[(1) - (2)].puppy_nodelist);
		}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 99 "puppy.y"
    {
			(yyval.puppy_nodelist) = new list<Node*>;
			(yyval.puppy_nodelist)->push_back((yyvsp[(1) - (1)].puppy_node));
		}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 107 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (2)].puppy_node);
		}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 111 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_node);
		}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 115 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_node);
		}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 122 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 126 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 130 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 134 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 138 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 142 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 146 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_statement);
		}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 153 "puppy.y"
    {
                        WhileLoopNode * node = new WhileLoopNode;

                        node->SetCondition((yyvsp[(3) - (7)].puppy_expr));
                        node->SetNodeList((yyvsp[(6) - (7)].puppy_nodelist));

                        (yyval.puppy_node) = static_cast<Node*>(node);
                }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 165 "puppy.y"
    {
			list<Node*> * n1 = new list<Node*>;
			n1->push_back((yyvsp[(2) - (10)].puppy_node));

			list<Node*> * n2 = new list<Node*>;
                        n2->push_back((yyvsp[(6) - (10)].puppy_node));

			ForLoopNode * node = new ForLoopNode;
			node->SetPreLoopStatement(n1);
			node->SetCondition((yyvsp[(4) - (10)].puppy_expr));
			node->SetPerOnceStatement(n2);
			node->SetNodeList((yyvsp[(9) - (10)].puppy_nodelist));

			(yyval.puppy_node) = static_cast<Node*>(node);
                }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 184 "puppy.y"
    {
			ForeachLoopNode * node = new ForeachLoopNode;

			node->SetPreLoopStatement(new list<Node*>);
			node->SetPerOnceStatement(new list<Node*>);

			node->SetCondition(new ConstValueExpression(new BooleanValue(true)));
			node->SetKV((yyvsp[(3) - (12)].puppy_ident)->GetName(), (yyvsp[(5) - (12)].puppy_ident)->GetName());
			node->SetCollectionExpr(static_cast<SetExpression*>((yyvsp[(8) - (12)].puppy_expr)));
                        node->SetNodeList((yyvsp[(11) - (12)].puppy_nodelist));

                        (yyval.puppy_node) = static_cast<Node*>(node);
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 201 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_node);
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 205 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_node);
		}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 209 "puppy.y"
    {
			(yyval.puppy_node) = (yyvsp[(1) - (1)].puppy_node);
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 216 "puppy.y"
    {
			(yyval.puppy_nodelist) = (yyvsp[(3) - (4)].puppy_nodelist);
		}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 220 "puppy.y"
    {
			(yyval.puppy_nodelist) = NULL;
		}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 227 "puppy.y"
    {
			BranchNode * node = new BranchNode;
			
			node->SetCondition((yyvsp[(3) - (8)].puppy_expr));
			node->SetNodeList((yyvsp[(6) - (8)].puppy_nodelist));
			node->SetElseNodeList((yyvsp[(8) - (8)].puppy_nodelist));
			(yyval.puppy_node) = (Node *)node;
		}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 239 "puppy.y"
    {
			AssignStatement * stmt = new AssignStatement;
			stmt->SetVariableName((yyvsp[(1) - (3)].puppy_ident)->GetName());
			stmt->SetExpression((yyvsp[(3) - (3)].puppy_expr));
			(yyval.puppy_statement) = stmt;
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 249 "puppy.y"
    {
			(yyval.puppy_statement) = new BreakStatement;
		}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 256 "puppy.y"
    {
			(yyval.puppy_statement) = new ContinueStatement;
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 263 "puppy.y"
    {
			(yyval.puppy_datatype) = Integer;
		}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 267 "puppy.y"
    {
			(yyval.puppy_datatype) = Float;
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 271 "puppy.y"
    {
			(yyval.puppy_datatype) = String;
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 275 "puppy.y"
    {
			(yyval.puppy_datatype) = Boolean;
		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 279 "puppy.y"
    {
			(yyval.puppy_datatype) = Set;
		}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 286 "puppy.y"
    {
			VarDefinitionStatement * stmt = new VarDefinitionStatement((yyvsp[(2) - (4)].puppy_identlist), (yyvsp[(4) - (4)].puppy_datatype));
			(yyval.puppy_statement) = stmt;
		}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 294 "puppy.y"
    {
			(yyvsp[(1) - (3)].puppy_identlist)->push_back((yyvsp[(3) - (3)].puppy_ident));
			(yyval.puppy_identlist) = (yyvsp[(1) - (3)].puppy_identlist);
		}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 299 "puppy.y"
    {
			(yyval.puppy_identlist) = new list<Identifier*>;
			(yyval.puppy_identlist)->push_back((yyvsp[(1) - (1)].puppy_ident));
		}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 307 "puppy.y"
    {
			SleepStatement * stmt = new SleepStatement;
			stmt->SetExpression((yyvsp[(2) - (2)].puppy_expr));
			(yyval.puppy_statement) = stmt;
		}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 316 "puppy.y"
    {
			ObjectStatement * stmt = new ObjectStatement((yyvsp[(1) - (4)].puppy_variable), (yyvsp[(3) - (4)].puppy_ident), (yyvsp[(4) - (4)].puppy_exprlist));
			(yyval.puppy_statement) = static_cast<StatementNode*>(stmt);
		}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 324 "puppy.y"
    {
			(yyvsp[(1) - (3)].puppy_exprlist)->push_back((yyvsp[(3) - (3)].puppy_expr));
			(yyval.puppy_exprlist) = (yyvsp[(1) - (3)].puppy_exprlist);
		}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 329 "puppy.y"
    {
			(yyval.puppy_exprlist) = new list<Expression*>;
			(yyval.puppy_exprlist)->push_back((yyvsp[(1) - (1)].puppy_expr));
		}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 334 "puppy.y"
    {
			(yyval.puppy_exprlist) = new list<Expression*>;
		}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 341 "puppy.y"
    {
			PrintStatement * stmt = new PrintStatement;
			stmt->SetExpressionList((yyvsp[(2) - (2)].puppy_exprlist));
			(yyval.puppy_statement) = stmt;
		}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 350 "puppy.y"
    {
			(yyval.puppy_variable) = new string((yyvsp[(1) - (1)].puppy_ident)->GetName());
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 356 "puppy.y"
    {
			(yyval.puppy_const) = new StringValue("\n");
		}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 360 "puppy.y"
    {
			(yyval.puppy_const) = new FloatValue(3.1415926);
		}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 366 "puppy.y"
    {
			(yyval.puppy_const) = (yyvsp[(1) - (1)].puppy_const_integer);
		}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 370 "puppy.y"
    {
			(yyval.puppy_const) = (yyvsp[(1) - (1)].puppy_const_float);
		}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 374 "puppy.y"
    {
			(yyval.puppy_const) = (yyvsp[(1) - (1)].puppy_const_string);
		}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 378 "puppy.y"
    {
			(yyval.puppy_const) = (yyvsp[(1) - (1)].puppy_const_boolean);
		}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 385 "puppy.y"
    {
			(yyval.puppy_setexpr) = new SetExpression((yyvsp[(2) - (3)].puppy_exprlist));
		}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 392 "puppy.y"
    {
                        (yyval.puppy_arithexpr) = static_cast<ArithmeticExpression*>(new PlusExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 396 "puppy.y"
    {
                        (yyval.puppy_arithexpr) = static_cast<ArithmeticExpression*>(new SubtractExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 400 "puppy.y"
    {
                        (yyval.puppy_arithexpr) = static_cast<ArithmeticExpression*>(new MultiplicationExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 404 "puppy.y"
    {
                        (yyval.puppy_arithexpr) = static_cast<ArithmeticExpression*>(new DivisionExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 411 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new GTExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 415 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new LTExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 419 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new EQExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 423 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new NEQExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 427 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new GEExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 431 "puppy.y"
    {
                        (yyval.puppy_relexpr) = static_cast<RelationExpression*>(new LEExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 438 "puppy.y"
    {
                        (yyval.puppy_logicalexpr) = static_cast<LogicalExpression*>(new ANDExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 442 "puppy.y"
    {
                        (yyval.puppy_logicalexpr) = static_cast<LogicalExpression*>(new ORExpression((yyvsp[(1) - (3)].puppy_expr), (yyvsp[(3) - (3)].puppy_expr)));
                }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 446 "puppy.y"
    {
                        (yyval.puppy_logicalexpr) = static_cast<LogicalExpression*>(new NOTExpression((yyvsp[(2) - (2)].puppy_expr)));
                }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 453 "puppy.y"
    {
			(yyval.puppy_kvexpr) = static_cast<KVExpression*>(new KVExpression((yyvsp[(2) - (5)].puppy_expr),(yyvsp[(4) - (5)].puppy_expr)));
		}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 460 "puppy.y"
    {
			VarExpression * var = new VarExpression((yyvsp[(1) - (4)].puppy_variable));
			(yyval.puppy_offsetexpr) = static_cast<OffsetExpression*>(new OffsetExpression(var,(yyvsp[(3) - (4)].puppy_expr)));
		}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 468 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>(new ConstValueExpression((yyvsp[(1) - (1)].puppy_const)));
		}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 472 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>(new ConstValueExpression((yyvsp[(1) - (1)].puppy_const)));
		}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 476 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>(new VarExpression((yyvsp[(1) - (1)].puppy_variable)));
		}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 480 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_arithexpr));
		}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 484 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_relexpr));
		}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 488 "puppy.y"
    {
                        (yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_logicalexpr));
                }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 492 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_kvexpr));
		}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 496 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_setexpr));
		}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 500 "puppy.y"
    {
			(yyval.puppy_expr) = static_cast<Expression*>((yyvsp[(1) - (1)].puppy_offsetexpr));
		}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 504 "puppy.y"
    {
			(yyval.puppy_expr) = (yyvsp[(2) - (3)].puppy_expr); 
		}
    break;



/* Line 1806 of yacc.c  */
#line 2314 "puppy.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 508 "puppy.y"


int yyerror(char *s)
{
    fprintf(stderr, "puppy grammar error: %s\n", s);
    return 0;
}

Node * parse()
{
    yyparse();
	
    return (Node*)final_node;
}

