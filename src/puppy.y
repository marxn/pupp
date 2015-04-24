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
    #include "transaction.h"

    using namespace std;
    
    int yyerror(char *);
    int yylex(void);
    ContainerNode * final;
%}

%locations

%union
{
        DataType                      puppy_datatype;
        VariableType                * puppy_vartype;
        IntegerValue                * puppy_const_integer;
        DecimalValue                * puppy_const_decimal;
        BooleanValue                * puppy_const_boolean;
        StringValue                 * puppy_const_string;
        ConstValue                  * puppy_const;
        Expression                  * puppy_expr;
        BinaryExpression            * puppy_relexpr;
        BinaryExpression            * puppy_arithexpr;
        BinaryExpression            * puppy_logicalexpr;
        KVExpression                * puppy_kvexpr;
        SetExpression               * puppy_setexpr;
        LValueExpression            * puppy_lvalueexpr;
        FunctionExpression          * puppy_funcexpr;
        string                      * puppy_ident;
        Node                        * puppy_node;
        list<Node*>                 * puppy_nodelist;
        list<string*>               * puppy_identlist;
        list<Expression*>           * puppy_exprlist;
        LValue                      * puppy_lvalue;
        StatementNode               * puppy_statement;
        FunctionNode                * puppy_function_node;
        FuncArgDef                  * puppy_function_argdef;
        list<FuncArgDef*>           * puppy_function_arg_list;
        long                          puppy_opt_prec_desc;
}

%left OR
%left AND
%left NOT
%nonassoc '='
%nonassoc  '<' '>' EQUAL_OP GE_OP LE_OP NOT_EQUAL_OP
%left '+' '-'
%left '*' '/'

%token <puppy_const_integer> INTEGER 
%token <puppy_const_decimal> DECIMAL
%token <puppy_const_boolean> BOOLEAN
%token <puppy_const_string> STRING

%token <puppy_ident> IDENTIFIER 
%token TYPE_INTEGER TYPE_DECIMAL TYPE_STRING TYPE_BOOLEAN TYPE_SET
%token DEF FUNCTION RETURN IF ELSE WHILE BREAK CONTINUE FOR FOREACH IN DO AS PRINT SLEEP 
%token TRANSACTION ROLLBACK COMMIT
%token AND OR NOT
%token NIL NL PI

%type  <puppy_const> const_value symbolic_constant
%type  <puppy_expr>  expr
 
%type  <puppy_arithexpr> arith_expr
%type  <puppy_relexpr> rel_expr
%type  <puppy_logicalexpr> logical_expr

%type  <puppy_kvexpr> kvexpr
%type  <puppy_setexpr> set_expr
%type  <puppy_lvalueexpr> lvalue_expr
%type  <puppy_funcexpr> func_expr

%type  <puppy_node>  puppybean
%type  <puppy_function_argdef> func_arg
%type  <puppy_function_arg_list> arg_list
%type  <puppy_function_node> function_node
%type  <puppy_node>  program_node statement_node loop_node while_loop for_loop foreach_loop branch_node transaction_node
%type  <puppy_nodelist>  program_node_block program_node_list optional_else_block

%type  <puppy_identlist> identifier_list
%type  <puppy_exprlist>  expr_list
%type  <puppy_opt_prec_desc> opt_prec_desc
%type  <puppy_datatype>  data_type function_return_prototype
%type  <puppy_vartype>  var_type func_arg_type
%type  <puppy_lvalue> lvalue 
%type  <puppy_statement> assign_statement print_statement break_statement continue_statement 
%type  <puppy_statement> vardefstatement sleep_statement call_statement
%type  <puppy_statement> returnstatement rollback_statement commit_statement

%%

puppybean: 
        program_node_list
                {
                        final = new ContainerNode;
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
        | transaction_node
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
        | rollback_statement
                {
                        $$ = $1;
                }
        | commit_statement
                {
                        $$ = $1;
                }
        | call_statement
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
        FOREACH '<' IDENTIFIER ',' IDENTIFIER '>' IN expr program_node_block 
                {
                        ForeachLoopNode * node = new ForeachLoopNode;

                        node->SetPreLoopStatement(NULL);
                        node->SetPerOnceStatement(NULL);

                        node->SetCondition(new ConstValueExpression(new BooleanValue(true)));
                        node->SetKV(*($3), *($5));
                        node->SetCollectionExpr(static_cast<SetExpression*>($8));
                        node->SetNodeList($9);

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

transaction_node:
        TRANSACTION '(' identifier_list ')' program_node_block
                {
                        TransNode * node = new TransNode;
                        node->SetIdentList($3);
                        node->SetNodeList($5);
                        $$ = node;
                }

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

func_arg_type:
        data_type
                {
                        $$ = new VariableType($1, $1, -1);
                }
        | data_type '[' ']'
                {
                        $$ = new VariableType(Array, $1, -1);
                }
        ;

func_arg:
        IDENTIFIER AS func_arg_type
                {
                        FuncArgDef * def = new FuncArgDef($1, $3->GetVarType(), false);
                        def->SetElementType($3->GetElementType());
                        $$ = def;
                }
        | IDENTIFIER '&' AS func_arg_type
                {
                        FuncArgDef * def = new FuncArgDef($1, $4->GetVarType(), true);
                        def->SetElementType($4->GetElementType());
                        $$ = def;
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
        AS data_type
                {
                        $$ = $2;
                }
        |
                {
                        $$ = Null; 
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

lvalue:
        lvalue '[' expr ']'
                {
                        $1->AddOffsetExpr($3);
                        $$ = $1;
                }
        | IDENTIFIER
                {
                        $$ = new LValue($1);
                }
        ;

assign_statement:
        lvalue '=' expr
                {
                        AssignStatement * stmt = new AssignStatement($1);
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

data_type:
        TYPE_INTEGER
                {
                        $$ = Integer;
                }
        | TYPE_DECIMAL
                {
                        $$ = Decimal;
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

var_type:
        var_type '[' expr ']'
                {
                        $1->SetVarType(Array);
                        $1->AddDimention($3);
                        $$ = $1;
                }
        | data_type opt_prec_desc
                {
                        $$ = new VariableType($1, $1, $2);
                }
        ;

opt_prec_desc:
        '(' INTEGER ')'
                {
                        $$ = $2->GetValue();
                }
        | /*empty*/
                {
                        $$ = -1;
                }
        ;

vardefstatement:
        DEF IDENTIFIER AS var_type
                {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement($2, $4);
                        $$ = stmt;
                }
        | DEF IDENTIFIER '=' const_value
                {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement($2, new VariableType($4->GetType(), $4->GetType(), -1));
                        stmt->SetInitValue($4);
                        $$ = stmt;
                } 
        | DEF IDENTIFIER '=' set_expr
                {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement($2, new VariableType(Set, Null, -1));
                        stmt->SetInitExpr($4);
                        $$ = stmt;
                }
        | DEF IDENTIFIER '=' func_expr
                {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement($2, new VariableType(Null, Null, -1));
                        stmt->SetInitExpr($4);
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

call_statement:
        func_expr
                {
                        CallStatement * stmt = new CallStatement;
                        stmt->SetExpression($1);
                        $$ = static_cast<StatementNode*>(stmt);
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

rollback_statement:
        ROLLBACK
                {
                        RollbackStatement * stmt = new RollbackStatement;
                        $$ = stmt;
                }
        ;

commit_statement:
        COMMIT
                {
                        CommitStatement * stmt = new CommitStatement;
                        $$ = stmt;
                }
        ;
symbolic_constant:
        NL
                {
                        $$ = new StringValue("\n");
                }
        | PI
                {
                        $$ = new DecimalValue(3.1415926);
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
        | '-' INTEGER
                {
                        $2->SetValue(0-$2->GetValue());
                        $$ = $2;
                }
        | DECIMAL                    
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
        TYPE_SET '{' expr_list '}'
                {
                        $$ = new SetExpression($3);
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

lvalue_expr:
    lvalue_expr '[' expr ']'
                {
                        $1->AddOffsetExpr($3);
                        $$ = $1;;
                }
    | IDENTIFIER 
                {
                        LValueExpression * expr = new LValueExpression($1);
                        $$ = expr;
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
    | lvalue_expr
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
    fprintf(stderr, "puppy parsing error: %s\n", s);
    return 0;
}

ContainerNode * parse()
{
    yyparse();
        
    return final;
}
