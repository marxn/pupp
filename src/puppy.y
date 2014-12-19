%{
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
%}

%union
{
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
}

%left OR
%left AND
%left NOT
%nonassoc '='
%nonassoc  '<' '>' EQUAL_OP GE_OP LE_OP NOT_EQUAL_OP
%left '+' '-'
%left '*' '/'
%left UMINUS

%token <puppy_const_integer> INTEGER 
%token <puppy_const_float> FLOAT
%token <puppy_const_boolean> BOOLEAN
%token <puppy_const_string> STRING

%token <puppy_ident> IDENTIFIER 
%token TYPE_INTEGER TYPE_FLOAT TYPE_STRING TYPE_BOOLEAN TYPE_SET
%token DEF IF ELSE WHILE BREAK CONTINUE AS PRINT SLEEP
%token AND OR NOT
%token NL PI

%type  <puppy_const> const_value symbolic_constant
%type  <puppy_expr>  expr 
%type  <puppy_arithexpr> arith_expr
%type  <puppy_relexpr> rel_expr
%type  <puppy_logicalexpr> logical_expr
%type  <puppy_kvexpr> kvexpr
%type  <puppy_setexpr> set_expr
%type  <puppy_offsetexpr> offset_expr
%type  <puppy_node>  program_node simple_node loop_node branch_node
%type  <puppy_nodelist>  optional_else_list node_list final_block
%type  <puppy_identlist> identifier_list
%type  <puppy_exprlist>  expr_list
%type  <puppy_variable>  variable
%type  <puppy_datatype>  def_data_type
%type  <puppy_statement> assign_statement print_statement break_statement continue_statement vardefstatement sleep_statement

%%

final_block: 
	node_list 
		{
			final_node = new ContainerNode;
			final_node->SetNodeList($1);
		}
	;
	
node_list: 
	node_list program_node
		{
			$1->push_back($2);
			$$ = $1;
		}
	| program_node
		{
			$$ = new list<Node*>;
			$$->push_back($1);
		}
	;
	
program_node:
	simple_node ';'
		{
			$$ = $1;
		}
	| loop_node
		{
			$$ = $1;
		}
	| branch_node
		{
			$$ = $1;
		} 
	;

simple_node:
    assign_statement
		{
			$$ = $1;
		}
	| break_statement
		{
			$$ = $1;
		}
	| continue_statement
		{
			$$ = $1;
		}
	| vardefstatement
		{
			$$ = $1;
		}
	| sleep_statement
		{
			$$ = $1;
		}
	| print_statement
		{
			$$ = $1;
		}
    ;

loop_node:
	WHILE '(' expr ')' '{' node_list '}'
		{
			LoopNode * node = new LoopNode;
			
			node->SetCondition($3);
			node->SetNodeList($6);
			
			$$ = (Node *)node;
		}
	;

optional_else_list:
	ELSE '{' node_list '}'
		{
			$$ = $3;
		}
	| /*empty*/
		{
			$$ = NULL;
		};

branch_node:
	IF '(' expr ')' '{' node_list '}' optional_else_list
		{
			BranchNode * node = new BranchNode;
			
			node->SetCondition($3);
			node->SetNodeList($6);
			node->SetElseNodeList($8);
			$$ = (Node *)node;
		}
	;

assign_statement:
	IDENTIFIER '=' expr
		{
			AssignStatement * stmt = new AssignStatement;
			stmt->SetVariableName($1->GetName());
			stmt->SetExpression($3);
			$$ = stmt;
		}
	;

break_statement:
	BREAK
		{
			$$ = new BreakStatement;
		}
	;
	
continue_statement:
	CONTINUE
		{
			$$ = new ContinueStatement;
		}
	;

def_data_type:
	TYPE_INTEGER
		{
			$$ = Integer;
		}
	| TYPE_FLOAT
		{
			$$ = Float;
		}
	| TYPE_STRING
		{
			$$ = String;
		}
	| TYPE_BOOLEAN
		{
			$$ = Boolean;
		}
	| TYPE_SET
		{
			$$ = Set;
		}
	;
	
vardefstatement:
	DEF identifier_list AS def_data_type
		{
			VarDefinitionStatement * stmt = new VarDefinitionStatement($2, $4);
			$$ = stmt;
		}
	;

identifier_list:
	identifier_list ',' IDENTIFIER
		{
			$1->push_back($3);
			$$ = $1;
		}
	| IDENTIFIER
		{
			$$ = new list<Identifier*>;
			$$->push_back($1);
		}
	;

sleep_statement:
	SLEEP expr
		{
			SleepStatement * stmt = new SleepStatement;
			stmt->SetExpression($2);
			$$ = stmt;
		}

expr_list:
	expr_list ',' expr
		{
			$1->push_back($3);
			$$ = $1;
		}
	| expr
		{
			$$ = new list<Expression*>;
			$$->push_back($1);
		}
	|
		{
			$$ = new list<Expression*>;
		}
	;

print_statement:
	PRINT expr_list
		{
			PrintStatement * stmt = new PrintStatement;
			stmt->SetExpressionList($2);
			$$ = stmt;
		}
	;
	
variable:
	IDENTIFIER
		{
			$$ = new string($1->GetName());
		}
	;
symbolic_constant:
	NL
		{
			$$ = new StringValue("\n");
		}
	| PI
		{
			$$ = new FloatValue(3.1415926);
		}
	;
const_value:
	INTEGER                    
		{
			$$ = $1;
		}
	| FLOAT                    
		{
			$$ = $1;
		}
	| STRING
		{
			$$ = $1;
		}
	| BOOLEAN
		{
			$$ = $1;
		}
	;

set_expr:
    '{' expr_list '}'
		{
			$$ = new SetExpression($2);
		}
	;

arith_expr:
    expr '+' expr
                {
                        $$ = static_cast<ArithmeticExpression*>(new PlusExpression($1, $3));
                }
    | expr '-' expr
                {
                        $$ = static_cast<ArithmeticExpression*>(new SubtractExpression($1, $3));
                }
    | expr '*' expr
                {
                        $$ = static_cast<ArithmeticExpression*>(new MultiplicationExpression($1, $3));
                }
    | expr '/' expr
                {
                        $$ = static_cast<ArithmeticExpression*>(new DivisionExpression($1, $3));
                }
    ;

rel_expr:
     expr '>' expr
                {
                        $$ = static_cast<RelationExpression*>(new GTExpression($1, $3));
                }
    | expr '<' expr
                {
                        $$ = static_cast<RelationExpression*>(new LTExpression($1, $3));
                }
    | expr EQUAL_OP expr
                {
                        $$ = static_cast<RelationExpression*>(new EQExpression($1, $3));
                }
    | expr NOT_EQUAL_OP expr
                {
                        $$ = static_cast<RelationExpression*>(new NEQExpression($1, $3));
                }
    | expr GE_OP expr
                {
                        $$ = static_cast<RelationExpression*>(new GEExpression($1, $3));
                }
    | expr LE_OP expr
                {
                        $$ = static_cast<RelationExpression*>(new LEExpression($1, $3));
                }
    ;

logical_expr:
    expr AND expr
                {
                        $$ = static_cast<LogicalExpression*>(new ANDExpression($1, $3));
                }
    | expr OR expr
                {
                        $$ = static_cast<LogicalExpression*>(new ORExpression($1, $3));
                }
    | NOT expr
                {
                        $$ = static_cast<LogicalExpression*>(new NOTExpression($2));
                }
    ;

kvexpr:
    '<' expr ',' expr '>'
		{
			$$ = static_cast<KVExpression*>(new KVExpression($2,$4));
		}
    ;

offset_expr:
    variable '[' expr ']' 
		{
			VarExpression * var = new VarExpression($1);
			$$ = static_cast<OffsetExpression*>(new OffsetExpression(var,$3));
		}
    ;

expr:
    symbolic_constant
		{
			$$ = static_cast<Expression*>(new ConstValueExpression($1));
		}
    | const_value
		{
			$$ = static_cast<Expression*>(new ConstValueExpression($1));
		}
    | variable
		{
			$$ = static_cast<Expression*>(new VarExpression($1));
		}
    | arith_expr
		{
			$$ = static_cast<Expression*>($1);
		}
    | rel_expr
		{
			$$ = static_cast<Expression*>($1);
		}
    | logical_expr
                {
                        $$ = static_cast<Expression*>($1);
                }
    | kvexpr
		{
			$$ = static_cast<Expression*>($1);
		}
    | set_expr
		{
			$$ = static_cast<Expression*>($1);
		}
    | offset_expr
		{
			$$ = static_cast<Expression*>($1);
		}
    | '(' expr ')'             
		{
			$$ = $2; 
		}
    ;
%%

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
