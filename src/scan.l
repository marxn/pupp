%{
    #include <stdlib.h>
    #include "constval.h"
    #include "expression.h"
    #include "variable.h"
    #include "node.h"
    #include "statement.h"
    #include "container.h"
    #include "puppy.hpp"
    
    int yyerror(char *);
%}

comment    "//".*
equal      "=="
not_equal  "!="|"<>"
ge_op      ">="
le_op      "<="
digit      [0-9]
integer    {digit}+
float      (({digit}*\.{digit}+)|({digit}+\.{digit}*))
string     \"([^\\"]|\\.)*\"
self       [-+\*\/();\:\=\#\&\{\}\[\]\<\>\.,]
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
				if(strcasecmp(yytext,"for")==0)
                                        return FOR;
				if(strcasecmp(yytext,"foreach")==0)
                                        return FOREACH;
				if(strcasecmp(yytext,"in")==0)
                                        return IN;
				if(strcasecmp(yytext,"do")==0)
                                        return DO;
				if(strcasecmp(yytext,"print")==0)
                                        return PRINT;
                                if(strcasecmp(yytext,"sleep")==0)
                                        return SLEEP;
				if(strcasecmp(yytext,"function")==0)
                                        return FUNCTION;
				if(strcasecmp(yytext,"transaction")==0)
                                        return TRANSACTION;
				if(strcasecmp(yytext,"rollback")==0)
                                        return ROLLBACK;
				if(strcasecmp(yytext,"commit")==0)
                                        return COMMIT;
				if(strcasecmp(yytext,"return")==0)
                                        return RETURN;
				//Variable Type
				if(strcasecmp(yytext,"any")==0)
                                        return TYPE_ANY;
				if(strcasecmp(yytext,"integer")==0)
					return TYPE_INTEGER;
				if(strcasecmp(yytext,"float")==0)
					return TYPE_FLOAT;
				if(strcasecmp(yytext,"string")==0)
					return TYPE_STRING;
				if(strcasecmp(yytext,"boolean")==0)
					return TYPE_BOOLEAN;
				if(strcasecmp(yytext,"collection")==0)
                                        return TYPE_SET;

				//Logical operation
				if(strcasecmp(yytext,"and")==0)
                                        return AND;
                                if(strcasecmp(yytext,"or")==0)
                                        return OR;
                                if(strcasecmp(yytext,"not")==0)
                                        return NOT;

				//symbolic constants
				//Null object
				if(strcmp(yytext, "NIL")==0)
                                        return NIL;

				//new line
				if(strcmp(yytext, "NL")==0)
					return NL;

				//PI
				if(strcmp(yytext, "PI")==0)
					return PI;

				//Common identifiers
				yylval.puppy_ident = new string(yytext);
				return IDENTIFIER;
		}
{space}		{}
{comment}       {}

%%

int yywrap(void)
{
    return 1;
}
