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

equal    "=="
digit    [0-9]
integer  {digit}+
float    (({digit}*\.{digit}+)|({digit}+\.{digit}*))
boolean  "true"|"TRUE"|"false"|"FALSE"
self     [-+\*\/();\:\=\#\&\{\}\<\>,]
space    [ \t\r\f\n]
identifier [a-zA-Z]+[0-9]*

%%

{equal}     {
				return EQUAL_OP;
			}
{self}		{
				return yytext[0];
			}
		
{integer}	{
				yylval.puppy_const_integer = new IntegerValue(atoi(yytext));
				return INTEGER;
			}
{float}		{
				yylval.puppy_const_float = new FloatValue(atof(yytext));
				return FLOAT;
			}
{boolean}   {
				if(strcasecmp(yytext, "true") == 0)
					yylval.puppy_const_boolean = new BooleanValue(true);
				else
					yylval.puppy_const_boolean = new BooleanValue(false);
				return BOOLEAN;
			}
{identifier} {
				if(strcmp(yytext,"if")==0)
					return IF;
				if(strcmp(yytext,"while")==0)
					return WHILE;
				if(strcmp(yytext,"break")==0)
					return BREAK;
				if(strcmp(yytext,"continue")==0)
					return CONTINUE;
				if(strcmp(yytext,"def")==0)
					return DEF;
				if(strcmp(yytext,"as")==0)
					return AS;
				if(strcmp(yytext,"integer")==0)
					return TYPE_INTEGER;
				if(strcmp(yytext,"float")==0)
					return TYPE_FLOAT;
				if(strcmp(yytext,"string")==0)
					return TYPE_STRING;
				if(strcmp(yytext,"boolean")==0)
					return TYPE_BOOLEAN;
				if(strcmp(yytext,"print")==0)
					return PRINT;
				
				yylval.puppy_ident = new Identifier(yytext);
				return IDENTIFIER;
			}
{space}		{}

%%

int yywrap(void)
{
    return 1;
}
