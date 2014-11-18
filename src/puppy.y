%{
    #include <iostream>
    #include <stdio.h>
    #include <list>
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
	Expression                  * puppy_expr;
	Node                        * puppy_node;
	Identifier                  * puppy_ident;
	list<Node*>                 * puppy_nodelist;
	list<Identifier*>           * puppy_identlist;
	AssignStatement             * puppy_assignstatement;
	BreakStatement              * puppy_breakstatement;
	ContinueStatement           * puppy_continuestatement;
	Variable                    * puppy_variable;
	VarDefinitionStatement      * puppy_vardef;
	string                      * puppy_variable_ref;
	PrintStatement              * puppy_printstatement;
}

%nonassoc '='
%nonassoc  '<' '>' EQUAL_OP
%left '+' '-'
%left '*' '/'
%left UMINUS

%token <puppy_const_integer> INTEGER 
%token <puppy_const_float> FLOAT
%token <puppy_const_boolean> BOOLEAN
%token <puppy_const_string> STRING

%token <puppy_ident> IDENTIFIER 
%token TYPE_INTEGER TYPE_FLOAT TYPE_STRING TYPE_BOOLEAN
%token DEF IF WHILE BREAK CONTINUE AS PRINT

%type  <puppy_const> const_value
%type  <puppy_expr>  expr
%type  <puppy_node>  program_node simple_node loop_node branch_node
%type  <puppy_assignstatement> assign_statement
%type  <puppy_printstatement> print_statement
%type  <puppy_breakstatement> break_statement
%type  <puppy_continuestatement> continue_statement
%type  <puppy_nodelist>  node_list node_block
%type  <puppy_identlist> identifier_list
%type  <puppy_variable_ref>  variable_ref
%type  <puppy_vardef>    vardefstatement
%type  <puppy_datatype>  def_data_type

%%

node_block: 
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

branch_node:
	IF '(' expr ')' '{' node_list '}'
		{
			BranchNode * node = new BranchNode;
			
			node->SetCondition($3);
			node->SetNodeList($6);
			
			$$ = (Node *)node;
		}
	;

assign_statement:
	IDENTIFIER '=' expr
		{
			$$ = new AssignStatement;
			$$->SetVariableName($1->GetName());
			$$->SetExpression($3);
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
	;
	
vardefstatement:
	DEF identifier_list AS def_data_type
		{
			$$ = new VarDefinitionStatement($2, $4);
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
	
print_statement:
	PRINT expr
		{
			$$ = new PrintStatement;
			$$->SetExpression($2);
		}
	;
	
variable_ref:
	IDENTIFIER
		{
			$$ = new string($1->GetName());
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
	
expr:
    const_value
		{
			$$ = new ConstValueExpression($1);
		}
    | variable_ref
		{
			$$ = new VarExpression($1);
		}
    | expr '+' expr            
		{ 
			$$ = new PlusExpression($1, $3);
		}
    | expr '-' expr
		{
			$$ = new SubtractExpression($1, $3);
		}
    | expr '*' expr
		{
			$$ = new MultiplicationExpression($1, $3);
		}
    | expr '/' expr
		{
			$$ = new DivisionExpression($1, $3);	
		}
    | expr '>' expr
		{
			$$ = new GTExpression($1, $3);
		}
    | expr '<' expr
		{
			$$ = new LTExpression($1, $3);
		}
    | expr EQUAL_OP expr
		{
			$$ = new EQExpression($1, $3);
		}
    | '(' expr ')'             
		{
			$$ = $2; 
		}
    ;
%%

int yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
    return 0;
}

Node * parse()
{
    yyparse();
	
    return (Node*)final_node;
}
