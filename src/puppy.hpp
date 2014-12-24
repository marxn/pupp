/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* Line 2068 of yacc.c  */
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



/* Line 2068 of yacc.c  */
#line 110 "puppy.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


