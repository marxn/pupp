%{
    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
    #include <list>

    #include "constval.h"
    #include "expression.h"
    #include "variable.h"
    #include "node.h"
    #include "statement.h"
    #include "container.h"
    #include "loop.h"
    #include "ifelse.h"
    #include "function.h"
    #include "bean.h"

    using namespace std;
    
    int yyerror(char *);
    int yylex(void);
    PuppyBean * final;
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
	BinaryExpression            * puppy_relexpr;
	BinaryExpression            * puppy_arithexpr;
	BinaryExpression            * puppy_logicalexpr;
	KVExpression                * puppy_kvexpr;
	SetExpression               * puppy_setexpr;
	OffsetExpression            * puppy_offsetexpr;
	FunctionExpression          * puppy_funcexpr;
        string                      * puppy_ident;
	Node                        * puppy_node;
	list<Node*>                 * puppy_nodelist;
	list<string*>               * puppy_identlist;
	list<Expression*>           * puppy_exprlist;
	CollectionElementRef        * puppy_collection_eleref;
	StatementNode               * puppy_statement;
	FunctionNode                * puppy_function_node;
	FuncArgDef                  * puppy_function_argdef;
	list<FuncArgDef*>           * puppy_function_arg_list;
}

%left OR
%left AND
%left NOT
%nonassoc '='
%nonassoc  '<' '>' EQUAL_OP GE_OP LE_OP NOT_EQUAL_OP
%left '+' '-'
%left '*' '/'

%token <puppy_const_integer> INTEGER 
%token <puppy_const_float> FLOAT
%token <puppy_const_boolean> BOOLEAN
%token <puppy_const_string> STRING

%token <puppy_ident> IDENTIFIER 
%token TYPE_ANY TYPE_INTEGER TYPE_FLOAT TYPE_STRING TYPE_BOOLEAN TYPE_SET
%token DEF FUNCTION RETURN IF ELSE WHILE BREAK CONTINUE FOR FOREACH IN DO AS PRINT SLEEP 
%token AND OR NOT
%token NIL NL PI

%type  <puppy_const> const_value symbolic_constant
%type  <puppy_expr>  expr
 
%type  <puppy_arithexpr> arith_expr
%type  <puppy_relexpr> rel_expr
%type  <puppy_logicalexpr> logical_expr

%type  <puppy_kvexpr> kvexpr
%type  <puppy_setexpr> set_expr
%type  <puppy_offsetexpr> offset_expr
%type  <puppy_funcexpr> func_expr

%type  <puppy_node>  puppybean
%type  <puppy_function_argdef> func_arg
%type  <puppy_function_arg_list> arg_list
%type  <puppy_function_node> function_node
%type  <puppy_node>  program_node statement_node loop_node while_loop for_loop foreach_loop branch_node
%type  <puppy_nodelist>  program_node_block program_node_list optional_else_block

%type  <puppy_identlist> identifier_list qualified_object
%type  <puppy_exprlist>  expr_list
%type  <puppy_variable>  variable
%type  <puppy_datatype>  def_data_type function_return_prototype

%type  <puppy_collection_eleref> collection_element_ref
%type  <puppy_statement> assign_statement print_statement break_statement continue_statement 
%type  <puppy_statement> vardefstatement sleep_statement element_assign_statement object_statement
%type  <puppy_statement> returnstatement

%%

puppybean: 
	program_node_list
		{
			final = new PuppyBean;
			final->SetNodeList($1);
			final->ParentNode = NULL;
		}
	;

program_node_block:
	'{' program_node_list '}'
		{
			$$ = $2;
		}
	| '{' '}'
		{
			$$ = new list<Node*>;
		}
	;

program_node_list: 
	program_node_list program_node
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
	statement_node ';'
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
	| function_node
		{
			$$ = $1;
		}
	;

statement_node:
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
	| returnstatement
		{
			$$ = $1;
		}
	| element_assign_statement
		{
			$$ = $1;
		}
	| object_statement
		{
			$$ = $1;
		}
    ;

while_loop:
	WHILE '(' expr ')' program_node_block
                {
                        WhileLoopNode * node = new WhileLoopNode;

                        node->SetCondition($3);
                        node->SetNodeList($5);

                        $$ = static_cast<Node*>(node);
                }
	;

for_loop:
	FOR '(' statement_node ';' expr ';' statement_node ')' program_node_block 
                {
			ForLoopNode * node = new ForLoopNode;
			node->SetPreLoopStatement($3);
			node->SetCondition($5);
			node->SetPerOnceStatement($7);
			node->SetNodeList($9);

			$$ = static_cast<Node*>(node);
                }
	;

foreach_loop:
	FOREACH '(' '<' IDENTIFIER ',' IDENTIFIER '>' IN expr ')' program_node_block 
		{
			ForeachLoopNode * node = new ForeachLoopNode;

			node->SetPreLoopStatement(NULL);
			node->SetPerOnceStatement(NULL);

			node->SetCondition(new ConstValueExpression(new BooleanValue(true)));
			node->SetKV(*($4), *($6));
			node->SetCollectionExpr(static_cast<SetExpression*>($9));
                        node->SetNodeList($11);

                        $$ = static_cast<Node*>(node);
		}
	;

loop_node:
	while_loop
		{
			$$ = $1;
		}
	| for_loop
		{
			$$ = $1;
		}
	| foreach_loop
		{
			$$ = $1;
		}
	;

optional_else_block:
	ELSE program_node_block 
		{
			$$ = $2;
		}
	| /*empty*/
		{
			$$ = NULL;
		}
	;

branch_node:
	IF '(' expr ')' program_node_block optional_else_block
		{
			BranchNode * node = new BranchNode;
			
			node->SetCondition($3);

			ContainerNode * ifnode = new ContainerNode;
			ifnode->SetNodeList($5);
			node->SetIfNode(ifnode);

                        if($6!=NULL)
                        {
                                ContainerNode * elsenode = new ContainerNode;
                                elsenode->SetNodeList($6);
                                node->SetElseNode(elsenode);
                        }

			$$ = static_cast<Node*>(node);
		}
	;

func_arg:
        IDENTIFIER AS def_data_type
                {
                        $$ = new FuncArgDef($1, $3, false);
                }
	| IDENTIFIER '&' AS def_data_type
		{
			$$ = new FuncArgDef($1, $4, true);
		}
        ;

arg_list:
        arg_list ',' func_arg
                {
                        $1->push_back($3);
                        $$ = $1;
                }
        | func_arg
                {
                        $$ = new list<FuncArgDef*>;
                        $$->push_back($1);
                }
        |
                {
                        $$ = new list<FuncArgDef*>;
                }
        ;

function_return_prototype:
	AS def_data_type
		{
			$$ = $2;
		}
	|
		{
			$$ = Any; 
		}

function_node:
        DEF FUNCTION IDENTIFIER '(' arg_list ')' function_return_prototype program_node_block 
                {
                        FunctionNode * fun = new FunctionNode($3);
                        fun->SetArgList($5);
                        fun->SetRtnType($7);
                        fun->SetNodeList($8);

                        $$ = fun;
                }
        ;

assign_statement:
	IDENTIFIER '=' expr
		{
			AssignStatement * stmt = new AssignStatement;
			stmt->SetVariableName(*($1));
			stmt->SetExpression($3);
			$$ = stmt;
		}
	;

collection_element_ref:
	collection_element_ref '[' expr ']'
                {
                        $1->AddOffsetExpr($3);
                        $$ = $1;
                }
	| IDENTIFIER '[' expr ']'
		{
			$$ = new CollectionElementRef($1, $3);
		}
	;

element_assign_statement:
	collection_element_ref '=' expr
		{
			SetElementAssignStatement * stmt = new SetElementAssignStatement($1);
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
	TYPE_ANY
		{
			$$ = Any;
		}
	| TYPE_INTEGER
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
			$$ = new list<string*>;
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
	;

returnstatement:
	RETURN expr
		{
			ReturnStatement * stmt = new ReturnStatement;
			stmt->SetExpression($2);
			$$ = stmt;
		}
	;

qualified_object:
	qualified_object '.' IDENTIFIER
		{
			$1->push_back($3);
                        $$ = $1;
		}
	| IDENTIFIER
		{
			$$ = new list<string*>;
                        $$->push_back($1);
		}
	;

object_statement:
	qualified_object expr_list
		{
			//ObjectStatement * stmt = new ObjectStatement($1, $3, $4);
			//$$ = static_cast<StatementNode*>(stmt);
		}
	;

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
			$$ = new string(*($1));
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
	| NIL
		{
			$$ = new NullValue;
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

func_expr:
	IDENTIFIER '(' expr_list ')'
		{
			$$ = new FunctionExpression($1, $3);
		}
	;

arith_expr:
    expr '+' expr
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
    ;

rel_expr:
     expr '>' expr
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
    | expr NOT_EQUAL_OP expr
                {
                        $$ = new NEQExpression($1, $3);
                }
    | expr GE_OP expr
                {
                        $$ = new GEExpression($1, $3);
                }
    | expr LE_OP expr
                {
                        $$ = new LEExpression($1, $3);
                }
    ;

logical_expr:
    expr AND expr
                {
                        $$ = new ANDExpression($1, $3);
                }
    | expr OR expr
                {
                        $$ = new ORExpression($1, $3);
                }
    | NOT expr
                {
                        $$ = new NOTExpression($2);
                }
    ;

kvexpr:
    '<' expr ',' expr '>'
		{
			$$ = new KVExpression($2,$4);
		}
    ;

offset_expr:
    variable '[' expr ']' 
		{
			VarExpression * var = new VarExpression($1);
			$$ = new OffsetExpression(var,$3);
		}
    | offset_expr '[' expr ']'
		{
			$$ = new OffsetExpression($1, $3);
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
    | func_expr
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

PuppyBean * parse()
{
    yyparse();
	
    return final;
}
