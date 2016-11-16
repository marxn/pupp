/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
    #include <list>
    #include <string>

    #include "parser/constval.h"
    #include "parser/expression.h"
    #include "parser/variable.h"
    #include "parser/node.h"
    #include "parser/statement.h"
    #include "parser/container.h"
    #include "parser/loop.h"
    #include "parser/ifelse.h"
    #include "parser/function.h"
    #include "parser/transaction.h"

    using namespace std;
    
    static int yyerror(char *);
    static ContainerNode * final;
    static unsigned long anonymous_func_seq = 0;

    extern int yylex(void);
    extern unsigned long parse_line;

#line 94 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OR = 258,
    AND = 259,
    NOT = 260,
    INIT_OPER = 261,
    SELF_INCR = 262,
    SELF_DECR = 263,
    SELF_MUL = 264,
    SELF_DIV = 265,
    EQUAL_OP = 266,
    GE_OP = 267,
    LE_OP = 268,
    NOT_EQUAL_OP = 269,
    INTEGER = 270,
    DECIMAL = 271,
    BOOLEAN = 272,
    STRING = 273,
    IDENTIFIER = 274,
    TYPE_INTEGER = 275,
    TYPE_DECIMAL = 276,
    TYPE_STRING = 277,
    TYPE_BOOLEAN = 278,
    TYPE_MAP = 279,
    DEF = 280,
    FUNCTION = 281,
    USING = 282,
    RETURN = 283,
    IF = 284,
    ELSE = 285,
    WHILE = 286,
    BREAK = 287,
    CONTINUE = 288,
    FOR = 289,
    FOREACH = 290,
    IN = 291,
    PRINT = 292,
    SLEEP = 293,
    TRANSACTION = 294,
    ROLLBACK = 295,
    COMMIT = 296,
    NL = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 30 "parser.y" /* yacc.c:355  */

        DataType                      pupp_datatype;
        VariableType                * pupp_vartype;
        IntegerValue                * pupp_const_integer;
        DecimalValue                * pupp_const_decimal;
        BooleanValue                * pupp_const_boolean;
        StringValue                 * pupp_const_string;
        ConstValue                  * pupp_const;
        Expression                  * pupp_expr;
        BinaryExpression            * pupp_relexpr;
        BinaryExpression            * pupp_arithexpr;
        BinaryExpression            * pupp_logicalexpr;
        KVExpression                * pupp_kvexpr;
        SetExpression               * pupp_setexpr;
        VarExpression               * pupp_varexpr;
        FunctionExpression          * pupp_funcexpr;
        LambdaExpression            * pupp_lambda_expr;
        std::string                 * pupp_ident;
        Node                        * pupp_node;
        std::list<Node*>            * pupp_nodelist;
        std::list<std::string*>     * pupp_identlist;
        std::list<Expression*>      * pupp_exprlist;
        LValue                      * pupp_lvalue;
        StatementNode               * pupp_statement;
        FunctionNode                * pupp_function_node;
        FuncArgDef                  * pupp_function_argdef;
        std::list<FuncArgDef*>      * pupp_function_arg_list;
        ClosureVarDesc              * pupp_closure_var_desc;
        std::list<ClosureVarDesc*>  * pupp_closure_var_list;
        bool                          pupp_opt_function_key;

#line 209 "parser.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 224 "parser.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   417

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  219

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    59,     2,
      54,    55,    20,    18,    53,    19,     2,    21,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    58,    52,
      12,     7,    13,     2,    60,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    56,     2,    57,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,     2,    51,     2,     2,     2,     2,
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
       5,     6,     8,     9,    10,    11,    14,    15,    16,    17,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   118,   118,   127,   131,   138,   143,   151,   155,   159,
     163,   170,   174,   178,   182,   186,   190,   194,   198,   202,
     206,   213,   225,   238,   255,   259,   263,   270,   279,   283,
     290,   296,   318,   322,   329,   335,   344,   349,   355,   361,
     366,   372,   377,   385,   390,   398,   403,   408,   424,   431,
     435,   443,   450,   457,   464,   471,   481,   488,   495,   499,
     503,   507,   511,   515,   522,   527,   533,   550,   555,   561,
     570,   575,   583,   592,   601,   606,   612,   619,   629,   634,
     640,   646,   655,   663,   670,   677,   681,   686,   690,   696,
     700,   707,   714,   718,   722,   729,   733,   737,   741,   748,
     752,   756,   760,   764,   768,   775,   779,   783,   790,   797,
     804,   809,   817,   821,   825,   829,   833,   837,   841,   845,
     849,   853,   857
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OR", "AND", "NOT", "INIT_OPER", "'='",
  "SELF_INCR", "SELF_DECR", "SELF_MUL", "SELF_DIV", "'<'", "'>'",
  "EQUAL_OP", "GE_OP", "LE_OP", "NOT_EQUAL_OP", "'+'", "'-'", "'*'", "'/'",
  "INTEGER", "DECIMAL", "BOOLEAN", "STRING", "IDENTIFIER", "TYPE_INTEGER",
  "TYPE_DECIMAL", "TYPE_STRING", "TYPE_BOOLEAN", "TYPE_MAP", "DEF",
  "FUNCTION", "USING", "RETURN", "IF", "ELSE", "WHILE", "BREAK",
  "CONTINUE", "FOR", "FOREACH", "IN", "PRINT", "SLEEP", "TRANSACTION",
  "ROLLBACK", "COMMIT", "NL", "'{'", "'}'", "';'", "','", "'('", "')'",
  "'['", "']'", "':'", "'&'", "'@'", "$accept", "puppbean",
  "program_node_block", "program_node_list", "program_node",
  "statement_node", "while_loop", "for_loop", "foreach_loop", "loop_node",
  "transaction_node", "optional_else_block", "branch_node",
  "func_arg_type", "func_arg", "arg_list", "function_return_prototype",
  "closure_var_desc", "closure_var_list", "opt_copy_clause", "lambda_node",
  "lambda_expr", "lvalue", "assign_statement", "break_statement",
  "continue_statement", "data_type", "opt_dimension_def", "var_type",
  "vardefstatement", "identifier_list", "sleep_statement",
  "returnstatement", "opt_thread_num", "call_statement", "expr_list",
  "print_statement", "rollback_statement", "commit_statement",
  "symbolic_constant", "literal_value", "set_expr", "func_expr",
  "arith_expr", "rel_expr", "logical_expr", "kvexpr", "key_seeker",
  "var_expr", "expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,    61,   262,   263,
     264,   265,    60,    62,   266,   267,   268,   269,    43,    45,
      42,    47,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     123,   125,    59,    44,    40,    41,    91,    93,    58,    38,
      64
};
# endif

#define YYPACT_NINF -80

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-80)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     285,    17,     7,   116,   116,   116,   -80,   -80,   308,     3,
     116,   116,   -28,   -80,   -80,   -22,   -22,    43,   285,   -80,
      -4,   -80,   -80,   -80,   -80,   -80,   -80,    11,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   116,     0,
     116,   116,    19,   -80,   -80,   -80,   -80,   -80,   -80,     5,
     116,   -80,     6,   -80,   -80,   -80,    31,   -80,   -80,   -80,
     -80,   -20,   357,   243,   243,    37,    24,    -1,   357,   357,
      87,   -15,     6,   -41,   -20,    31,   -80,   -80,   -80,   116,
     116,   116,   116,   116,   116,   -80,   357,   116,    41,   386,
     201,   -80,   -80,    65,    93,   -80,    -2,    78,    -8,   116,
     116,   116,   -80,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   239,    90,   -80,   116,    76,
     116,   -80,    10,    65,    73,   357,   357,   357,   357,   357,
       9,   357,   -80,   -80,   -80,   -80,   -80,   -80,    80,   -80,
     116,    41,    74,   -15,   100,   -80,   -80,    20,    33,    61,
     181,   386,   396,   396,   396,   396,   396,   396,   105,   105,
     -80,   -80,   -80,   262,   -11,   -80,   220,   121,   357,   127,
     104,   133,   -80,   -80,   116,   -80,   376,   -80,   101,    41,
     -80,   102,   103,   -80,   -80,   -80,   -80,   -80,   -80,   308,
     145,   -80,   -80,   -80,    30,   154,   106,   -80,   -12,    41,
     104,   104,   117,   -80,   -80,   -80,   141,   -80,    69,   -80,
     -80,   -80,   116,   -80,   -12,   -80,   243,   -80,   -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    49,     0,     0,     0,     0,    56,    57,     0,     0,
      80,     0,     0,    82,    83,     0,     0,     0,     2,     6,
       0,    24,    25,    26,     8,    10,     9,     0,    11,    12,
      13,    14,    15,    17,    20,    16,    18,    19,     0,     0,
       0,     0,     0,    85,    87,    90,    89,   110,    84,    38,
      80,    48,   121,   112,   113,   118,   120,   114,   115,   116,
     117,   119,    73,     0,     0,     0,     0,    81,    79,    72,
       0,    38,     0,     0,     0,    76,     1,     5,     7,     0,
       0,     0,     0,     0,    80,    50,    69,     0,     0,   107,
       0,    86,    88,   110,     0,    37,     0,     0,     0,    80,
      80,    80,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    21,     0,     0,
       0,    71,     0,     0,    75,    51,    52,    53,    54,    55,
       0,    68,    58,    59,    60,    61,    62,    63,    65,    67,
       0,     0,     0,     0,    46,   122,    91,     0,     0,     0,
     106,   105,   100,    99,   101,   103,   104,   102,    95,    96,
      97,    98,     4,     0,     0,    31,     0,     0,    78,     0,
       0,     0,    77,   109,    80,    66,     0,    34,    32,     0,
      36,     0,    40,    93,    94,    92,     3,    28,    29,     0,
       0,    70,    27,    74,     0,   108,     0,    35,     0,     0,
       0,     0,     0,    64,    33,    41,     0,    43,     0,    39,
      47,    22,     0,    42,     0,    45,     0,    44,    23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -80,   -80,   -52,    47,   -13,    -5,   -80,   -80,   -80,   -80,
     -80,   -80,     4,   -10,    28,   -80,   -80,   -40,   -80,   -80,
     -80,   128,   -80,   -80,   -80,   -80,   -79,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -44,   -80,   -80,   -80,   -80,
     -80,   -80,   130,   -80,   -80,   -80,   -80,   148,   136,    -3
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    17,   116,    18,    19,    20,    21,    22,    23,    24,
      25,   165,    26,   177,    95,    96,   200,   207,   208,   182,
      51,    52,    27,    28,    29,    30,   178,   175,   139,    31,
     122,    32,    33,   172,    34,    67,    35,    36,    37,    53,
      54,    55,    56,    57,    58,    59,    60,   102,    61,    68
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      62,    63,    64,    65,    47,    77,    98,    87,    69,   138,
      40,   123,   117,   100,   205,    66,   124,    41,    79,    80,
      81,    82,    83,    38,    42,     4,    70,    43,    44,    45,
      46,    93,    71,    39,   101,    86,    84,    89,    90,   115,
     130,    91,    92,    76,    94,   120,    97,   206,    78,   146,
     119,   143,   120,   144,    48,   147,   148,   149,    88,    49,
      99,    50,   120,   169,    94,   170,   173,    84,   132,   133,
     134,   135,   136,   120,   137,   183,   125,   126,   127,   128,
     129,   103,   104,   120,   131,   100,   120,   203,   184,   118,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   187,   121,   120,   166,   185,   168,   192,   142,
     209,    40,   214,   141,   215,   113,   114,   164,    41,   167,
     194,   171,   179,   145,   181,    42,   174,   176,    43,    44,
      45,    46,    47,    72,    72,    73,    75,   190,   210,   211,
      77,    74,    74,   191,   115,   193,   198,   196,   202,    40,
     212,   199,   163,   204,   218,    48,    -1,   213,   188,   197,
      49,   180,    50,    42,   217,    85,    43,    44,    45,    46,
      47,     0,     0,     0,   201,   104,     0,     0,     0,     0,
       0,     0,   153,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    48,   103,   104,     0,     0,    49,   216,
      50,     0,     0,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   103,   104,     0,     0,     0,     0,     0,
       0,     0,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,     0,     0,     0,     0,   103,   104,     0,     0,
       0,     0,     0,     0,   140,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     1,     0,     0,     0,     0,
       0,     2,   189,     0,     3,     4,     0,     5,     6,     7,
       8,     9,     0,    10,    11,    12,    13,    14,     1,     0,
     162,     0,     0,   115,     2,    15,     0,     3,     4,    16,
       5,     6,     7,     8,     9,     0,    10,    11,    12,    13,
      14,     1,     0,   186,     0,     0,     0,     2,    15,     0,
       3,     4,    16,     5,     6,     7,     8,     9,     0,    10,
      11,    12,    13,    14,     1,     0,     0,     0,     0,     0,
       2,    15,     0,     3,     0,    16,     0,     6,     7,     0,
       0,     0,    10,    11,     0,    13,    14,     0,     0,     0,
     103,   104,     0,     0,    15,     0,     0,     0,    16,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   103,
     104,     0,     0,     0,     0,     0,     0,     0,   105,   195,
     107,   108,   109,   110,   111,   112,   113,   114,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,   113,   114
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     8,    26,    18,    50,     7,    11,    88,
       5,    26,    64,    54,    26,    12,    57,    12,     7,     8,
       9,    10,    11,     6,    19,    36,    54,    22,    23,    24,
      25,    26,    54,    26,    54,    38,    56,    40,    41,    50,
      84,    22,    23,     0,    59,    53,    49,    59,    52,    57,
      26,    53,    53,    55,    49,    99,   100,   101,    58,    54,
      54,    56,    53,    53,    59,    55,    57,    56,    27,    28,
      29,    30,    31,    53,    33,    55,    79,    80,    81,    82,
      83,     3,     4,    53,    87,    54,    53,    57,    55,    52,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   164,    26,    53,   118,    55,   120,   170,    26,
     199,     5,    53,    58,    55,    20,    21,    37,    12,    53,
     174,    58,    58,    55,    34,    19,    56,   140,    22,    23,
      24,    25,    26,    15,    16,    15,    16,    26,   200,   201,
     163,    15,    16,    26,    50,    22,    54,    56,    13,     5,
      43,    58,   115,    57,   216,    49,    12,    26,   164,   179,
      54,   143,    56,    19,   214,    27,    22,    23,    24,    25,
      26,    -1,    -1,    -1,   189,     4,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    49,     3,     4,    -1,    -1,    54,   212,
      56,    -1,    -1,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    26,    -1,    -1,    -1,    -1,
      -1,    32,    52,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    26,    -1,
      51,    -1,    -1,    50,    32,    56,    -1,    35,    36,    60,
      38,    39,    40,    41,    42,    -1,    44,    45,    46,    47,
      48,    26,    -1,    51,    -1,    -1,    -1,    32,    56,    -1,
      35,    36,    60,    38,    39,    40,    41,    42,    -1,    44,
      45,    46,    47,    48,    26,    -1,    -1,    -1,    -1,    -1,
      32,    56,    -1,    35,    -1,    60,    -1,    39,    40,    -1,
      -1,    -1,    44,    45,    -1,    47,    48,    -1,    -1,    -1,
       3,     4,    -1,    -1,    56,    -1,    -1,    -1,    60,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    26,    32,    35,    36,    38,    39,    40,    41,    42,
      44,    45,    46,    47,    48,    56,    60,    62,    64,    65,
      66,    67,    68,    69,    70,    71,    73,    83,    84,    85,
      86,    90,    92,    93,    95,    97,    98,    99,     6,    26,
       5,    12,    19,    22,    23,    24,    25,    26,    49,    54,
      56,    81,    82,   100,   101,   102,   103,   104,   105,   106,
     107,   109,   110,   110,   110,    66,    12,    96,   110,   110,
      54,    54,    82,   103,   109,   103,     0,    65,    52,     7,
       8,     9,    10,    11,    56,   108,   110,     7,    58,   110,
     110,    22,    23,    26,    59,    75,    76,   110,    96,    54,
      54,    54,   108,     3,     4,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    50,    63,    63,    52,    26,
      53,    26,    91,    26,    57,   110,   110,   110,   110,   110,
      96,   110,    27,    28,    29,    30,    31,    33,    87,    89,
      53,    58,    26,    53,    55,    55,    57,    96,    96,    96,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,    51,    64,    37,    72,   110,    53,   110,    53,
      55,    58,    94,    57,    56,    88,   110,    74,    87,    58,
      75,    34,    80,    55,    55,    55,    51,    63,    73,    52,
      26,    26,    63,    22,    96,    13,    56,    74,    54,    58,
      77,    66,    13,    57,    57,    26,    59,    78,    79,    87,
      63,    63,    43,    26,    53,    55,   110,    78,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    63,    64,    64,    65,    65,    65,
      65,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    68,    69,    70,    70,    70,    71,    72,    72,
      72,    73,    74,    74,    75,    75,    76,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    80,    81,    82,    83,
      83,    84,    84,    84,    84,    84,    85,    86,    87,    87,
      87,    87,    87,    87,    88,    88,    89,    90,    90,    90,
      91,    91,    92,    93,    94,    94,    95,    95,    96,    96,
      96,    97,    98,    99,   100,   101,   101,   101,   101,   101,
     101,   102,   103,   103,   103,   104,   104,   104,   104,   105,
     105,   105,   105,   105,   105,   106,   106,   106,   107,   108,
     109,   109,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     7,     9,     1,     1,     1,     5,     2,     2,
       0,     4,     1,     3,     3,     4,     3,     1,     0,     2,
       0,     1,     2,     1,     3,     4,     0,     6,     1,     1,
       2,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     2,     4,     4,     3,
       3,     1,     2,     2,     2,     0,     2,     4,     3,     1,
       0,     2,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     3,     4,     4,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     5,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
      yychar = yylex ();
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 119 "parser.y" /* yacc.c:1646  */
    {
                        final = new ContainerNode;
                        final->SetNodeList((yyvsp[0].pupp_nodelist));
                        final->ParentNode = NULL;
                }
#line 1512 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 128 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_nodelist) = (yyvsp[-1].pupp_nodelist);
                }
#line 1520 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 132 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_nodelist) = new list<Node*>;
                }
#line 1528 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 139 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[-1].pupp_nodelist)->push_back((yyvsp[0].pupp_node));
                        (yyval.pupp_nodelist) = (yyvsp[-1].pupp_nodelist);
                }
#line 1537 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 144 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_nodelist) = new list<Node*>;
                        (yyval.pupp_nodelist)->push_back((yyvsp[0].pupp_node));
                }
#line 1546 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 152 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[-1].pupp_node);
                }
#line 1554 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 156 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1562 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 160 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1570 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 164 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1578 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 171 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1586 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 175 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1594 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 179 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1602 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 183 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1610 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 187 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1618 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 191 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1626 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 195 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1634 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 199 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1642 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 203 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1650 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 207 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_statement);
                }
#line 1658 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 214 "parser.y" /* yacc.c:1646  */
    {
                        WhileLoopNode * node = new WhileLoopNode;

                        node->SetCondition((yyvsp[-1].pupp_expr));
                        node->SetNodeList((yyvsp[0].pupp_nodelist));

                        (yyval.pupp_node) = static_cast<Node*>(node);
                }
#line 1671 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 226 "parser.y" /* yacc.c:1646  */
    {
                        ForLoopNode * node = new ForLoopNode;
                        node->SetPreLoopStatement((yyvsp[-5].pupp_node));
                        node->SetCondition((yyvsp[-3].pupp_expr));
                        node->SetPerOnceStatement((yyvsp[-1].pupp_node));
                        node->SetNodeList((yyvsp[0].pupp_nodelist));

                        (yyval.pupp_node) = static_cast<Node*>(node);
                }
#line 1685 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 239 "parser.y" /* yacc.c:1646  */
    {
                        ForeachLoopNode * node = new ForeachLoopNode;

                        node->SetPreLoopStatement(NULL);
                        node->SetPerOnceStatement(NULL);

                        node->SetCondition(new ConstValueExpression(new BooleanValue(true)));
                        node->SetKV(*((yyvsp[-6].pupp_ident)), *((yyvsp[-4].pupp_ident)));
                        node->SetCollectionExpr(static_cast<SetExpression*>((yyvsp[-1].pupp_expr)));
                        node->SetNodeList((yyvsp[0].pupp_nodelist));

                        (yyval.pupp_node) = static_cast<Node*>(node);
                }
#line 1703 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 256 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1711 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 260 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1719 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 264 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_node) = (yyvsp[0].pupp_node);
                }
#line 1727 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 271 "parser.y" /* yacc.c:1646  */
    {
                        TransNode * node = new TransNode;
                        node->SetIdentList((yyvsp[-2].pupp_identlist));
                        node->SetNodeList((yyvsp[0].pupp_nodelist));
                        (yyval.pupp_node) = node;
                }
#line 1738 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 280 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_nodelist) = (yyvsp[0].pupp_nodelist);
                }
#line 1746 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 284 "parser.y" /* yacc.c:1646  */
    {
                        list<Node*> * elseiflist =  new list<Node*>;
                        elseiflist->push_back((yyvsp[0].pupp_node));
                        (yyval.pupp_nodelist) = elseiflist;
                }
#line 1756 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 290 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_nodelist) = NULL;
                }
#line 1764 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 297 "parser.y" /* yacc.c:1646  */
    {
                        BranchNode * node = new BranchNode;
                        
                        node->SetCondition((yyvsp[-2].pupp_expr));

                        ContainerNode * ifnode = new ContainerNode;
                        ifnode->SetNodeList((yyvsp[-1].pupp_nodelist));
                        node->SetIfNode(ifnode);

                        if((yyvsp[0].pupp_nodelist)!=NULL)
                        {
                                ContainerNode * elsenode = new ContainerNode;
                                elsenode->SetNodeList((yyvsp[0].pupp_nodelist));
                                node->SetElseNode(elsenode);
                        }

                        (yyval.pupp_node) = static_cast<Node*>(node);
                }
#line 1787 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 319 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_vartype) = new VariableType((yyvsp[0].pupp_datatype), (yyvsp[0].pupp_datatype));
                }
#line 1795 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 323 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_vartype) = new VariableType(Array, (yyvsp[-2].pupp_datatype));
                }
#line 1803 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 330 "parser.y" /* yacc.c:1646  */
    {
                        FuncArgDef * def = new FuncArgDef(*((yyvsp[-2].pupp_ident)), (yyvsp[0].pupp_vartype)->GetVarType(), false);
                        def->SetElementType((yyvsp[0].pupp_vartype)->GetElementType());
                        (yyval.pupp_function_argdef) = def;
                }
#line 1813 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 336 "parser.y" /* yacc.c:1646  */
    {
                        FuncArgDef * def = new FuncArgDef(*((yyvsp[-2].pupp_ident)), (yyvsp[0].pupp_vartype)->GetVarType(), true);
                        def->SetElementType((yyvsp[0].pupp_vartype)->GetElementType());
                        (yyval.pupp_function_argdef) = def;
                }
#line 1823 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 345 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[-2].pupp_function_arg_list)->push_back((yyvsp[0].pupp_function_argdef));
                        (yyval.pupp_function_arg_list) = (yyvsp[-2].pupp_function_arg_list);
                }
#line 1832 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 350 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_function_arg_list) = new list<FuncArgDef*>;
                        (yyval.pupp_function_arg_list)->push_back((yyvsp[0].pupp_function_argdef));
                }
#line 1841 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 355 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_function_arg_list) = new list<FuncArgDef*>;
                }
#line 1849 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 362 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = (yyvsp[0].pupp_datatype);
                }
#line 1857 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 366 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = Null; 
                }
#line 1865 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 373 "parser.y" /* yacc.c:1646  */
    {
                        ClosureVarDesc * desc = new ClosureVarDesc(*((yyvsp[0].pupp_ident)), false);
                        (yyval.pupp_closure_var_desc) = desc;
                }
#line 1874 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 378 "parser.y" /* yacc.c:1646  */
    {
                        ClosureVarDesc * desc = new ClosureVarDesc(*((yyvsp[0].pupp_ident)), true);
                        (yyval.pupp_closure_var_desc) = desc;
                }
#line 1883 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 386 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_closure_var_list) = new list<ClosureVarDesc*>;
                        (yyval.pupp_closure_var_list)->push_back((yyvsp[0].pupp_closure_var_desc));
                }
#line 1892 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 391 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[-2].pupp_closure_var_list)->push_back((yyvsp[0].pupp_closure_var_desc));
                        (yyval.pupp_closure_var_list) = (yyvsp[-2].pupp_closure_var_list);
                }
#line 1901 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 399 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_closure_var_list) = (yyvsp[-1].pupp_closure_var_list); 
                }
#line 1909 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 403 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_closure_var_list) = NULL;
                }
#line 1917 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 409 "parser.y" /* yacc.c:1646  */
    {
                        char seq_buf[32] = {0};
                        snprintf(seq_buf, sizeof(seq_buf), "%lu", anonymous_func_seq++);

                        FunctionNode * fun = new FunctionNode("anonymous_func_" + string(seq_buf));
                        fun->SetArgList((yyvsp[-4].pupp_function_arg_list));
                        fun->SetCopyList((yyvsp[-2].pupp_closure_var_list));
                        fun->SetRtnType((yyvsp[-1].pupp_datatype));
                        fun->SetNodeList((yyvsp[0].pupp_nodelist));

                        (yyval.pupp_function_node) = fun;
                }
#line 1934 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 425 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_lambda_expr) = new LambdaExpression((yyvsp[0].pupp_function_node));
                }
#line 1942 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 432 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_lvalue) = new LValue((yyvsp[0].pupp_ident));
                }
#line 1950 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 436 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_lvalue) = (yyvsp[-1].pupp_lvalue);
                        (yyval.pupp_lvalue)->AddOffsetExprList((yyvsp[0].pupp_exprlist));
                }
#line 1959 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 444 "parser.y" /* yacc.c:1646  */
    {
                        AssignStatement * stmt = new AssignStatement((yyvsp[-2].pupp_lvalue));
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        stmt->SetOperType(0); // no operation.
                        (yyval.pupp_statement) = stmt;
                }
#line 1970 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 451 "parser.y" /* yacc.c:1646  */
    {
                        AssignStatement * stmt = new AssignStatement((yyvsp[-2].pupp_lvalue));
                        stmt->SetOperType(1);  //add to self
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 1981 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 458 "parser.y" /* yacc.c:1646  */
    {
                        AssignStatement * stmt = new AssignStatement((yyvsp[-2].pupp_lvalue));
                        stmt->SetOperType(2);  //sub from self
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 1992 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 465 "parser.y" /* yacc.c:1646  */
    {
                        AssignStatement * stmt = new AssignStatement((yyvsp[-2].pupp_lvalue));
                        stmt->SetOperType(3);  //multiple with self
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2003 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 472 "parser.y" /* yacc.c:1646  */
    {
                        AssignStatement * stmt = new AssignStatement((yyvsp[-2].pupp_lvalue));
                        stmt->SetOperType(4);  //divide from self
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2014 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 482 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_statement) = new BreakStatement;
                }
#line 2022 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 489 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_statement) = new ContinueStatement;
                }
#line 2030 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 496 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = Integer;
                }
#line 2038 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 500 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = Decimal;
                }
#line 2046 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 504 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = String;
                }
#line 2054 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 508 "parser.y" /* yacc.c:1646  */
    { 
                        (yyval.pupp_datatype) = Boolean;
                }
#line 2062 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 512 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = Map;
                }
#line 2070 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 516 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_datatype) = Func;
                }
#line 2078 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 523 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_exprlist) = (yyvsp[-1].pupp_exprlist);
                }
#line 2086 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 527 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_exprlist) = NULL;
                }
#line 2094 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 534 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_vartype) = new VariableType((yyvsp[-1].pupp_datatype), (yyvsp[-1].pupp_datatype));
                        if((yyvsp[0].pupp_exprlist)!=NULL)
                        {
                                (yyval.pupp_vartype)->SetVarType(Array);

                                list<Expression*>::iterator i;
                                for(i = (yyvsp[0].pupp_exprlist)->begin(); i!=(yyvsp[0].pupp_exprlist)->end(); i++)
                                {
                                        (yyval.pupp_vartype)->AddDimention(*i);
                                } 
                        }
                }
#line 2112 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 551 "parser.y" /* yacc.c:1646  */
    {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement((yyvsp[-2].pupp_ident), (yyvsp[0].pupp_vartype));
                        (yyval.pupp_statement) = stmt;
                }
#line 2121 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 556 "parser.y" /* yacc.c:1646  */
    {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement((yyvsp[-2].pupp_ident), new VariableType(Null, Null));
                        stmt->SetInitExpr((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2131 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 562 "parser.y" /* yacc.c:1646  */
    {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement((yyvsp[-2].pupp_ident), new VariableType(Null, Null));
                        stmt->SetInitExpr((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2141 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 571 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[-2].pupp_identlist)->push_back((yyvsp[0].pupp_ident));
                        (yyval.pupp_identlist) = (yyvsp[-2].pupp_identlist);
                }
#line 2150 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 576 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_identlist) = new list<string*>;
                        (yyval.pupp_identlist)->push_back((yyvsp[0].pupp_ident));
                }
#line 2159 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 584 "parser.y" /* yacc.c:1646  */
    {
                        SleepStatement * stmt = new SleepStatement;
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2169 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 593 "parser.y" /* yacc.c:1646  */
    {
                        ReturnStatement * stmt = new ReturnStatement;
                        stmt->SetExpression((yyvsp[0].pupp_expr));
                        (yyval.pupp_statement) = stmt;
                }
#line 2179 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 602 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const_integer) = (yyvsp[0].pupp_const_integer);
                }
#line 2187 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 606 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const_integer) = new IntegerValue(0);
                }
#line 2195 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 613 "parser.y" /* yacc.c:1646  */
    {
                        CallStatement * stmt = new CallStatement;
                        (yyvsp[0].pupp_funcexpr)->SetThreadNum(-1);
                        stmt->SetExpression((yyvsp[0].pupp_funcexpr));
                        (yyval.pupp_statement) = static_cast<StatementNode*>(stmt);
                }
#line 2206 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 620 "parser.y" /* yacc.c:1646  */
    {
                        CallStatement * stmt = new CallStatement;
                        (yyvsp[-2].pupp_funcexpr)->SetThreadNum((yyvsp[0].pupp_const_integer)->GetValue());
                        stmt->SetExpression((yyvsp[-2].pupp_funcexpr));
                        (yyval.pupp_statement) = static_cast<StatementNode*>(stmt);
                }
#line 2217 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 630 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[-2].pupp_exprlist)->push_back((yyvsp[0].pupp_expr));
                        (yyval.pupp_exprlist) = (yyvsp[-2].pupp_exprlist);
                }
#line 2226 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 635 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_exprlist) = new list<Expression*>;
                        (yyval.pupp_exprlist)->push_back((yyvsp[0].pupp_expr));
                }
#line 2235 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 640 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_exprlist) = new list<Expression*>;
                }
#line 2243 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 647 "parser.y" /* yacc.c:1646  */
    {
                        PrintStatement * stmt = new PrintStatement;
                        stmt->SetExpressionList((yyvsp[0].pupp_exprlist));
                        (yyval.pupp_statement) = stmt;
                }
#line 2253 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 656 "parser.y" /* yacc.c:1646  */
    {
                        RollbackStatement * stmt = new RollbackStatement;
                        (yyval.pupp_statement) = stmt;
                }
#line 2262 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 664 "parser.y" /* yacc.c:1646  */
    {
                        CommitStatement * stmt = new CommitStatement;
                        (yyval.pupp_statement) = stmt;
                }
#line 2271 "parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 671 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const) = new StringValue("\n");
                }
#line 2279 "parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 678 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const) = (yyvsp[0].pupp_const_integer);
                }
#line 2287 "parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 682 "parser.y" /* yacc.c:1646  */
    {
                        (yyvsp[0].pupp_const_integer)->SetValue(0-(yyvsp[0].pupp_const_integer)->GetValue());
                        (yyval.pupp_const) = (yyvsp[0].pupp_const_integer);
                }
#line 2296 "parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 687 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const) = (yyvsp[0].pupp_const_decimal);
                }
#line 2304 "parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 691 "parser.y" /* yacc.c:1646  */
    {
                        DecimalValue * value = new DecimalValue(DecimalValue(0L) - *((yyvsp[0].pupp_const_decimal)));
                        delete (yyvsp[0].pupp_const_decimal);
                        (yyval.pupp_const) = value;
                }
#line 2314 "parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 697 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const) = (yyvsp[0].pupp_const_string);
                }
#line 2322 "parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 701 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_const) = (yyvsp[0].pupp_const_boolean);
                }
#line 2330 "parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 708 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_setexpr) = new SetExpression((yyvsp[-1].pupp_exprlist));
                }
#line 2338 "parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 715 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_funcexpr) = new FunctionExpression((yyvsp[-3].pupp_varexpr), (yyvsp[-1].pupp_exprlist));
                }
#line 2346 "parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 719 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_funcexpr) = new FunctionExpression((yyvsp[-3].pupp_lambda_expr), (yyvsp[-1].pupp_exprlist));
                }
#line 2354 "parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 723 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_funcexpr) = new FunctionExpression((yyvsp[-3].pupp_funcexpr), (yyvsp[-1].pupp_exprlist));
                }
#line 2362 "parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 730 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_arithexpr) = new PlusExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2370 "parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 734 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_arithexpr) = new SubtractExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2378 "parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 738 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_arithexpr) = new MultiplicationExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2386 "parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 742 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_arithexpr) = new DivisionExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2394 "parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 749 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new GTExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2402 "parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 753 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new LTExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2410 "parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 757 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new EQExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2418 "parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 761 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new NEQExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2426 "parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 765 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new GEExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2434 "parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 769 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_relexpr) = new LEExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2442 "parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 776 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_logicalexpr) = new ANDExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2450 "parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 780 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_logicalexpr) = new ORExpression((yyvsp[-2].pupp_expr), (yyvsp[0].pupp_expr));
                }
#line 2458 "parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 784 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_logicalexpr) = new NOTExpression((yyvsp[0].pupp_expr));
                }
#line 2466 "parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 791 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_kvexpr) = new KVExpression((yyvsp[-3].pupp_expr),(yyvsp[-1].pupp_expr));
                }
#line 2474 "parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 798 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_exprlist) = (yyvsp[-1].pupp_exprlist);
                }
#line 2482 "parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 805 "parser.y" /* yacc.c:1646  */
    {
                        VarExpression * expr = new VarExpression((yyvsp[0].pupp_ident));
                        (yyval.pupp_varexpr) = expr;
                }
#line 2491 "parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 810 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_varexpr) = (yyvsp[-1].pupp_varexpr);
                        (yyval.pupp_varexpr)->AddOffsetExprsList((yyvsp[0].pupp_exprlist));
                }
#line 2500 "parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 818 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>(new ConstValueExpression((yyvsp[0].pupp_const)));
                }
#line 2508 "parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 822 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>(new ConstValueExpression((yyvsp[0].pupp_const)));
                }
#line 2516 "parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 826 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_arithexpr));
                }
#line 2524 "parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 830 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_relexpr));
                }
#line 2532 "parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 834 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_logicalexpr));
                }
#line 2540 "parser.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 838 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_kvexpr));
                }
#line 2548 "parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 842 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_setexpr));
                }
#line 2556 "parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 846 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_varexpr));
                }
#line 2564 "parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 850 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_funcexpr));
                }
#line 2572 "parser.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 854 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = static_cast<Expression*>((yyvsp[0].pupp_lambda_expr));
                }
#line 2580 "parser.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 858 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.pupp_expr) = (yyvsp[-1].pupp_expr); 
                }
#line 2588 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2592 "parser.cpp" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 862 "parser.y" /* yacc.c:1906  */


int yyerror(char *s)
{
    fprintf(stderr, "pupp parsing error: %s at line: %lu\n", s, parse_line);
    return 0;
}

ContainerNode * parse()
{
    yyparse();
        
    return final;
}
