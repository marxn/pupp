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

    unsigned long anonymous_func_seq = 0;
%}

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
        VarExpression               * puppy_varexpr;
        FunctionExpression          * puppy_funcexpr;
        LambdaExpression            * puppy_lambda_expr;
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
        bool                          puppy_opt_function_key;
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
%token DEF FUNCTION COPY RETURN IF ELSE WHILE BREAK CONTINUE FOR FOREACH IN CALL PRINT SLEEP 
%token TRANSACTION ROLLBACK COMMIT
%token AND OR NOT
%token NIL NL PI

%type  <puppy_const> literal_value symbolic_constant
%type  <puppy_expr>  expr
 
%type  <puppy_arithexpr> arith_expr
%type  <puppy_relexpr> rel_expr
%type  <puppy_logicalexpr> logical_expr

%type  <puppy_kvexpr> kvexpr
%type  <puppy_setexpr> set_expr
%type  <puppy_varexpr> var_expr
%type  <puppy_funcexpr> func_expr
%type  <puppy_lambda_expr> lambda_expr

%type  <puppy_node>  puppybean
%type  <puppy_function_argdef> func_arg
%type  <puppy_function_arg_list> arg_list
%type  <puppy_function_node> lambda_node
%type  <puppy_node>  program_node statement_node loop_node while_loop for_loop foreach_loop branch_node transaction_node
%type  <puppy_nodelist>  program_node_block program_node_list optional_else_block

%type  <puppy_identlist> identifier_list
%type  <puppy_identlist> opt_copy_clause
%type  <puppy_exprlist>  expr_list
%type  <puppy_exprlist>  opt_dimension_def
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
        IDENTIFIER ':' func_arg_type
                {
                        FuncArgDef * def = new FuncArgDef(*($1), $3->GetVarType(), false);
                        def->SetElementType($3->GetElementType());
                        $$ = def;
                }
        | '&' IDENTIFIER ':' func_arg_type
                {
                        FuncArgDef * def = new FuncArgDef(*($2), $4->GetVarType(), true);
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
        | /*empty*/
                {
                        $$ = new list<FuncArgDef*>;
                }
        ;

function_return_prototype:
        ':' data_type
                {
                        $$ = $2;
                }
        |
                {
                        $$ = Null; 
                }
        ;

opt_copy_clause:
        COPY '(' identifier_list ')'
                {
                        $$ = $3; 
                }
        | /*empty*/
                {
                        $$ = NULL;
                }
        ;  
lambda_node:
        '(' arg_list ')' opt_copy_clause function_return_prototype program_node_block
                {
                        char seq_buf[32] = {0};
                        snprintf(seq_buf, sizeof(seq_buf), "%lu", anonymous_func_seq++);

                        FunctionNode * fun = new FunctionNode("anonymous_func_" + string(seq_buf));
                        fun->SetArgList($2);
                        fun->SetCopyList($4);
                        fun->SetRtnType($5);
                        fun->SetNodeList($6);

                        $$ = fun;
                }
        ;

lambda_expr:
        lambda_node
                {
                        $$ = new LambdaExpression($1);
                }
        ;

lvalue:
        IDENTIFIER opt_dimension_def
                {
                        $$ = new LValue($1);
                        if($2!=NULL)
                        {
                                list<Expression*>::iterator i;
                                for(i = $2->begin(); i!=$2->end(); i++)
                                {
                                        $$->AddOffsetExpr(*i);
                                }
                        }
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
        | FUNCTION
                {
                        $$ = Func;
                }
        ;

opt_dimension_def:
        '[' expr_list ']'
                {
                        $$ = $2;
                }
        | /*empty*/
                {
                        $$ = NULL;
                }
        ;

var_type:
        data_type opt_prec_desc opt_dimension_def
                {
                        $$ = new VariableType($1, $1, $2);
                        if($3!=NULL)
                        {
                                $$->SetVarType(Array);

                                list<Expression*>::iterator i;
                                for(i = $3->begin(); i!=$3->end(); i++)
                                {
                                        $$->AddDimention(*i);
                                } 
                        }
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
        DEF IDENTIFIER ':' var_type
                {
                        VarDefinitionStatement * stmt = new VarDefinitionStatement($2, $4);
                        $$ = stmt;
                }
        | DEF IDENTIFIER '=' expr
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
        CALL func_expr
                {
                        CallStatement * stmt = new CallStatement;
                        stmt->SetExpression($2);
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
                        $$ = new DecimalValue("3.1415926");
                }
        | NIL
                {
                        $$ = new NullValue;
                }
        ;

literal_value:
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
        var_expr '(' expr_list ')'
                {
                        $$ = new FunctionExpression($1, $3);
                }
        | func_expr '(' expr_list ')'
                {
                        $$ = new FunctionExpression($1, $3);
                }
        | lambda_expr '(' expr_list ')'
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

var_expr:
    IDENTIFIER opt_dimension_def 
                {
                        VarExpression * expr = new VarExpression($1);
                        if($2!=NULL)
                        {
                                list<Expression*>::iterator i;
                                for(i = $2->begin(); i!=$2->end(); i++)
                                {
                                        expr->AddOffsetExpr(*i);
                                }
                        }

                        $$ = expr;
                }
    ;

expr:
    symbolic_constant
                {
                        $$ = static_cast<Expression*>(new ConstValueExpression($1));
                }
    | literal_value
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
    | var_expr
                {
                        $$ = static_cast<Expression*>($1);
                }
    | func_expr
                {
                        $$ = static_cast<Expression*>($1);
                }
    | lambda_expr
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
