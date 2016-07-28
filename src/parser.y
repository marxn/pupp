%{
    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
    #include <list>
    #include <string>

    #include "parser/constval.h"
    #include "parser/expression.h"
    #include "parser/variable.h"
    #include "parser/node.h"
    #include "parser/statement.h"
    #include "parser/container.h"
    #include "parser/loop.h"
    #include "parser/ifelse.h"
    #include "parser/function.h"
    #include "parser/transaction.h"

    using namespace std;
    
    int yyerror(char *);
    int yylex(void);
    ContainerNode * final;

    unsigned long anonymous_func_seq = 0;
    extern unsigned long parse_line;
%}

%union
{
        DataType                      pupp_datatype;
        VariableType                * pupp_vartype;
        IntegerValue                * pupp_const_integer;
        DecimalValue                * pupp_const_decimal;
        BooleanValue                * pupp_const_boolean;
        StringValue                 * pupp_const_string;
        ConstValue                  * pupp_const;
        Expression                  * pupp_expr;
        BinaryExpression            * pupp_relexpr;
        BinaryExpression            * pupp_arithexpr;
        BinaryExpression            * pupp_logicalexpr;
        KVExpression                * pupp_kvexpr;
        SetExpression               * pupp_setexpr;
        VarExpression               * pupp_varexpr;
        FunctionExpression          * pupp_funcexpr;
        LambdaExpression            * pupp_lambda_expr;
        std::string                 * pupp_ident;
        Node                        * pupp_node;
        std::list<Node*>            * pupp_nodelist;
        std::list<std::string*>     * pupp_identlist;
        std::list<Expression*>      * pupp_exprlist;
        LValue                      * pupp_lvalue;
        StatementNode               * pupp_statement;
        FunctionNode                * pupp_function_node;
        FuncArgDef                  * pupp_function_argdef;
        std::list<FuncArgDef*>      * pupp_function_arg_list;
        ClosureVarDesc              * pupp_closure_var_desc;
        std::list<ClosureVarDesc*>  * pupp_closure_var_list;
        long                          pupp_opt_prec_desc;
        bool                          pupp_opt_function_key;
}

%left OR
%left AND
%left NOT
%nonassoc '='
%nonassoc  '<' '>' EQUAL_OP GE_OP LE_OP NOT_EQUAL_OP
%left '+' '-'
%left '*' '/'

%token <pupp_const_integer> INTEGER 
%token <pupp_const_decimal> DECIMAL
%token <pupp_const_boolean> BOOLEAN
%token <pupp_const_string> STRING

%token <pupp_ident> IDENTIFIER 
%token TYPE_INTEGER TYPE_DECIMAL TYPE_STRING TYPE_BOOLEAN TYPE_SET
%token DEF FUNCTION USING RETURN IF ELSE WHILE BREAK CONTINUE FOR FOREACH IN CALL PRINT SLEEP 
%token TRANSACTION ROLLBACK COMMIT
%token AND OR NOT
%token NL

%type  <pupp_const> literal_value symbolic_constant
%type  <pupp_expr>  expr
 
%type  <pupp_arithexpr> arith_expr
%type  <pupp_relexpr> rel_expr
%type  <pupp_logicalexpr> logical_expr

%type  <pupp_kvexpr> kvexpr
%type  <pupp_setexpr> set_expr
%type  <pupp_varexpr> var_expr
%type  <pupp_funcexpr> func_expr
%type  <pupp_lambda_expr> lambda_expr

%type  <pupp_node>  puppbean
%type  <pupp_function_argdef> func_arg
%type  <pupp_closure_var_desc> closure_var_desc
%type  <pupp_function_arg_list> arg_list
%type  <pupp_function_node> lambda_node
%type  <pupp_node>  program_node statement_node loop_node while_loop for_loop foreach_loop branch_node transaction_node
%type  <pupp_nodelist>  program_node_block program_node_list optional_else_block

%type  <pupp_identlist> identifier_list
%type  <pupp_closure_var_list> closure_var_list opt_copy_clause
%type  <pupp_exprlist>  expr_list
%type  <pupp_exprlist>  opt_dimension_def
%type  <pupp_opt_prec_desc> opt_prec_desc
%type  <pupp_datatype>  data_type function_return_prototype
%type  <pupp_vartype>  var_type func_arg_type
%type  <pupp_lvalue> lvalue 
%type  <pupp_statement> assign_statement print_statement break_statement continue_statement 
%type  <pupp_statement> vardefstatement sleep_statement call_statement
%type  <pupp_statement> returnstatement rollback_statement commit_statement

%%

puppbean: 
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
        | ELSE branch_node
                {
                        list<Node*> * elseiflist =  new list<Node*>;
                        elseiflist->push_back($2);
                        $$ = elseiflist;
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

closure_var_desc:
        IDENTIFIER
                {
                        ClosureVarDesc * desc = new ClosureVarDesc(*($1), false);
                        $$ = desc;
                }
        | '&' IDENTIFIER 
                {
                        ClosureVarDesc * desc = new ClosureVarDesc(*($2), true);
                        $$ = desc;
                }
        ;

closure_var_list:
        closure_var_desc
                {
                        $$ = new list<ClosureVarDesc*>;
                        $$->push_back($1);
                }
        | closure_var_list ',' closure_var_desc
                {
                        $1->push_back($3);
                        $$ = $1;
                }
        ;

opt_copy_clause:
        USING '(' closure_var_list ')'
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
    fprintf(stderr, "pupp parsing error: %s at line: %lu\n", s, parse_line);
    return 0;
}

ContainerNode * parse()
{
    yyparse();
        
    return final;
}
