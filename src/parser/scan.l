%{
    #include <stdlib.h>
    #include "parser/constval.h"
    #include "parser/expression.h"
    #include "parser/variable.h"
    #include "parser/node.h"
    #include "parser/statement.h"
    #include "parser/container.h"
    #include "parser.hpp"
   
    using namespace std;
 
    int yyerror(char *);
    unsigned long parse_line = 1;
%}

comment    "//".*
equal      "=="
self_incr  "+="
self_decr  "-="
self_mul   "*="
self_div   "/="
not_equal  "!="|"<>"
ge_op      ">="
le_op      "<="
digit      [0-9]
integer    {digit}+
decimal    (({digit}*\.{digit}+)|({digit}+\.{digit}*))
string     \"([^\\"]|\\.)*\"
self       [-+\*\/();\:\=\#\&\{\}\[\]\<\>\.,@]
space      [ \t\r\f\n]
identifier [a-zA-Z_]+[0-9]*

%%

{equal}             {
                                return EQUAL_OP;
                }

{not_equal}        {
                                return NOT_EQUAL_OP;
                }
{ge_op}                {
                                return GE_OP;
                }

{le_op}                {
                                return LE_OP;
                }

{self_incr}     {
                                return SELF_INCR;
                }
{self_decr}     {
                                return SELF_DECR;
                }

{self_mul}      {
                                return SELF_MUL;
                }

{self_div}      {
                                return SELF_DIV;
                }

{self}                {
                                return yytext[0];
                }

{string}        {
                                yytext[strlen(yytext)-1] = '\0';
                                yylval.pupp_const_string = new StringValue(yytext+1);
                                return STRING;
                }
                
{integer}        {
                                yylval.pupp_const_integer = new IntegerValue(atol(yytext));
                                return INTEGER;
                }
{decimal}        {
                                StringValue temp(yytext);
                                yylval.pupp_const_decimal = new DecimalValue(temp.toString());
                                return DECIMAL;
                }
{identifier}        {
                                //Boolean constant value
                                if(strcasecmp(yytext, "true") == 0)
                                {
                                        yylval.pupp_const_boolean = new BooleanValue(true);
                                        return BOOLEAN;
                                }
                                if(strcasecmp(yytext, "false") == 0)
                                {
                                        yylval.pupp_const_boolean = new BooleanValue(false);
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
                                if(strcasecmp(yytext,"for")==0)
                                        return FOR;
                                if(strcasecmp(yytext,"foreach")==0)
                                        return FOREACH;
                                if(strcasecmp(yytext,"in")==0)
                                        return IN;
                                if(strcasecmp(yytext,"print")==0)
                                        return PRINT;
                                if(strcasecmp(yytext,"sleep")==0)
                                        return SLEEP;
                                if(strcasecmp(yytext,"function")==0)
                                        return FUNCTION;
                                if(strcasecmp(yytext,"USING")==0)
                                        return USING;
                                if(strcasecmp(yytext,"transaction")==0)
                                        return TRANSACTION;
                                if(strcasecmp(yytext,"rollback")==0)
                                        return ROLLBACK;
                                if(strcasecmp(yytext,"commit")==0)
                                        return COMMIT;
                                if(strcasecmp(yytext,"return")==0)
                                        return RETURN;

                                //Variable Type
                                if(strcasecmp(yytext,"int")==0)
                                        return TYPE_INTEGER;
                                if(strcasecmp(yytext,"decimal")==0)
                                        return TYPE_DECIMAL;
                                if(strcasecmp(yytext,"string")==0)
                                        return TYPE_STRING;
                                if(strcasecmp(yytext,"bool")==0)
                                        return TYPE_BOOLEAN;
                                if(strcasecmp(yytext,"set")==0)
                                        return TYPE_SET;

                                //Logical operation
                                if(strcasecmp(yytext,"and")==0)
                                        return AND;
                                if(strcasecmp(yytext,"or")==0)
                                        return OR;
                                if(strcasecmp(yytext,"not")==0)
                                        return NOT;

                                //symbolic constants
                                //new line
                                if(strcmp(yytext, "NL")==0)
                                        return NL;

                                //Common identifiers
                                yylval.pupp_ident = new string(yytext);
                                return IDENTIFIER;
                }
{space}         {
                                if(yytext[0]=='\n')
                                {
                                    parse_line++;
                                }
                }
{comment}       {}

%%

int yywrap(void)
{
    return 1;
}
