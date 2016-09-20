#ifndef _PUPP_STATEMENT_H_
#define _PUPP_STATEMENT_H_

#include <unistd.h>
#include <iostream>
#include "node.h"
#include "expression.h"

class StatementNode :public Node
{
public:
        StatementNode();
        ~StatementNode();
        void Swipe(NodeContext * context);
};

class BreakStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        bool Provision();
        bool Check();
};

class ContinueStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        bool Provision();
        bool Check();
};

class RollbackStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        bool Provision();
        bool Check();
};

class CommitStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        bool Provision();
        bool Check();
};

class ReturnStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        void SetExpression(Expression * expr);
        bool Provision();
        bool Check();

private:
        Expression * Expr;

};

class LValue 
{
public:
        LValue(std::string * id);
        std::string GetVarName();
        std::list<std::list<Expression*> *> * GetExpsList();
        void AddOffsetExprList(std::list<Expression *> * expr_list);
private:
        std::string Var;
        std::list<std::list<Expression*> *> * ExpsList;
};

class AssignStatement: public StatementNode
{
public:
        AssignStatement(LValue * ref);
        int Invoke(NodeContext * context);
        void SetExpression(Expression * expr);
        bool Provision();
        bool Check();
        void SetOperType(int type);
private:
        void AdjustValue(ValueBox * vbox, ConstValue * ad, int oper);
private:
        int OperType;
        LValue * Reference;
        Expression * Expr;
        VariableDef * VarDef;
        unsigned long VarLayer;
};

class VariableType
{
public:
        VariableType(DataType vartype, DataType elementtype, long prec);

        void SetVarType(DataType type);
        DataType GetVarType();
        DataType GetElementType();
        void SetElementType(DataType type);
        void AddDimention(Expression * exp);
        std::list<Expression*> * GetDimentions();
        void SetPrecision(long prec);
        long GetPrecision();
private:
        DataType VarType;
        DataType ElementType;
        std::list<Expression*> Dimentions;
        long Prec;
};

class VarDefinitionStatement: public StatementNode
{
public:
        VarDefinitionStatement(std::string * ident, VariableType * vartype);
        
        int Invoke(NodeContext * context);
        void SetInitExpr(Expression * expr);
        bool Provision();
        bool Check();
private:
        VariableDef * VarDef;
        Expression * InitExpr;
        std::string * Ident;
        VariableType * VarType;
};

class PrintStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        void SetExpressionList(std::list<Expression*> * exprlist);
        bool Provision();
        bool Check();

private:
        std::list<Expression*> * ExprList;
};

class SleepStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context);
        void SetExpression(Expression * expr);
        bool Provision();
        bool Check();

private:
        Expression * Expr;
};

class CallStatement: public StatementNode
{
public:
        CallStatement();

        int Invoke(NodeContext * context);
        void Swipe(NodeContext * context);
        void SetExpression(Expression * expr);
        bool Provision();
        bool Check();

        ConstValue * GetRetVal();
private:
        ConstValue * RetVal;
        Expression * Expr;
};

#endif

