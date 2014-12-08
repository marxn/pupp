%{
    #include <stdlib.h>
    #include "constval.h"
    #include "expression.h"
    #include "identifier.h"
    #include "variable.h"
    #include "node.h"
    #include "statement.h"
    #include "container.h"
    #include "puppy.hpp"
    
    int yyerror(char *);
%}

equal      "=="
not_equal  "!="|"<>"
ge_op      ">="
le_op      "<="
digit      [0-9]
integer    -?{digit}+
float      -?(({digit}*\.{digit}+)|({digit}+\.{digit}*))
string     '(\\.|''|[^'\n])*'
self       [-+\*\/();\:\=\#\&\{\}\<\>,]
space      [ \t\r\f\n]
identifier [a-zA-Z]+[0-9]*

%%

{equal}     	{
				return EQUAL_OP;
		}

{not_equal}	{
				return NOT_EQUAL_OP;
		}
{ge_op}		{
				return GE_OP;
		}

{le_op}		{
				return LE_OP;
		}

{self}		{
				return yytext[0];
		}

{string}	{
				yytext[strlen(yytext)-1] = '\0';
				yylval.puppy_const_string = new StringValue(yytext+1);
				return STRING;
		}
		
{integer}	{
				yylval.puppy_const_integer = new IntegerValue(atoi(yytext));
				return INTEGER;
		}
{float}		{
				yylval.puppy_const_float = new FloatValue(atof(yytext));
				return FLOAT;
		}
{identifier}	{
				//Boolean constant value
				if(strcasecmp(yytext, "true") == 0)
				{
                                        yylval.puppy_const_boolean = new BooleanValue(true);
					return BOOLEAN;
				}
                                if(strcasecmp(yytext, "false") == 0)
				{
                                        yylval.puppy_const_boolean = new BooleanValue(false);
                                	return BOOLEAN;
				}

				//Key words
				if(strcasecmp(yytext,"if")==0)
					return IF;
				if(strcasecmp(yytext,"else")==0)
					return ELSE;
				if(strcasecmp(yytext,"while")==0)
					return WHILE;
				if(strcasecmp(yytext,"break")==0)
					return BREAK;
				if(strcasecmp(yytext,"continue")==0)
					return CONTINUE;
				if(strcasecmp(yytext,"def")==0)
					return DEF;
				if(strcasecmp(yytext,"as")==0)
					return AS;
				if(strcasecmp(yytext,"integer")==0)
					return TYPE_INTEGER;
				if(strcasecmp(yytext,"float")==0)
					return TYPE_FLOAT;
				if(strcasecmp(yytext,"string")==0)
					return TYPE_STRING;
				if(strcasecmp(yytext,"boolean")==0)
					return TYPE_BOOLEAN;
				if(strcasecmp(yytext,"print")==0)
					return PRINT;
				if(strcasecmp(yytext, "sleep")==0)
					return SLEEP;

				//symbolic constants

				//new line
				if(strcmp(yytext, "NL")==0)
					return NL;

				if(strcmp(yytext, "PI")==0)
					return PI;

				//Common identifiers
				yylval.puppy_ident = new Identifier(yytext);
				return IDENTIFIER;
		}
{space}		{}

%%

int yywrap(void)
{
    return 1;
}
