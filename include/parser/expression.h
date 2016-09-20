#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include "constval.h"
#include "operation.h"
#include "variable.h"
#include "node.h"
#include "function.h"
#include "closure.h"

class Expression
{
public:
        Expression();
        virtual ~Expression();
        virtual ConstValue * Calculate(NodeContext * context) = 0;

        void SetParentNode(Node * node);

        virtual bool Provision() = 0;
        virtual bool Check();
        void LValue(bool flag);
        bool isLValue();
        bool IsLambdaExp();
protected:
        bool lValue;
        bool islambda;
        Node * ParentNode;
};

class ConstValueExpression: public Expression
{
public:
        ConstValueExpression();
        ConstValueExpression(ConstValue * value);
        ~ConstValueExpression();
        ConstValue * Calculate(NodeContext * context);
        bool Provision();
private:
        ConstValue * Value;
};

class BinaryExpression: public Expression
{
public:
        BinaryExpression(Expression * arg1, Expression * arg2);

        ConstValue * Calculate(NodeContext * context);

        virtual ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store) = 0;

        bool Provision();
        bool Check();

protected:
        Expression * left;
        Expression * right;
};

class KVExpression: public BinaryExpression
{
public:
        KVExpression(Expression * arg1, Expression * arg2);

        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class SetExpression: public Expression
{
public:
        SetExpression(std::list<Expression*> * exp);
        ConstValue * Calculate(NodeContext * context);
        bool Provision();
        bool Check();

private:
        std::list<Expression*> * ExprList;
};

class VarExpression: public Expression
{
public:
        VarExpression(std::string * arg);

        ValueBox * GetVarRef(NodeContext * context);

        ConstValue * Calculate(NodeContext * context);

        bool Provision();

        bool Check();

        void AddOffsetExprsList(std::list<Expression*> * exprlist);
        
private:
        std::string VarName;
        VariableDef * VarDef;
        std::list<std::list<Expression*>* > ExprsList;
        unsigned long VarLayer;
};

class FunctionExpression: public Expression
{
public:
        FunctionExpression(Expression * object, std::list<Expression*> * exprlist);
        ConstValue * Calculate(NodeContext * context);
        bool Provision();
        bool Check();
        
private:
        Expression *  FuncObj;
        VariableDef * FuncDef;
        std::list<Expression*> * ExprList;
};

class LambdaExpression: public Expression
{
public:
        LambdaExpression(FunctionNode * node);
        ConstValue * Calculate(NodeContext * context);

        bool Provision();
        bool Check();
private:
        FunctionNode * FuncNode;
};

class PlusExpression: public BinaryExpression 
{
public:
        PlusExpression(Expression * arg1, Expression * arg2);
        ~PlusExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class SubtractExpression: public BinaryExpression
{
public:
        SubtractExpression(Expression * arg1, Expression * arg2);
        ~SubtractExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class MultiplicationExpression: public BinaryExpression
{
public:
        MultiplicationExpression(Expression * arg1, Expression * arg2);
        ~MultiplicationExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class DivisionExpression: public BinaryExpression
{
public:
        DivisionExpression(Expression * arg1, Expression * arg2);
        ~DivisionExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class GTExpression: public BinaryExpression
{
public:
        GTExpression(Expression * arg1, Expression * arg2);
        ~GTExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class LTExpression: public BinaryExpression
{
public:
        LTExpression(Expression * arg1, Expression * arg2);
        ~LTExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class EQExpression: public BinaryExpression
{
public:
        EQExpression(Expression * arg1, Expression * arg2);
        ~EQExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class NEQExpression: public BinaryExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2);
        ~NEQExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class GEExpression: public BinaryExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2);
        ~GEExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class LEExpression: public BinaryExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2);
        ~LEExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class ANDExpression: public BinaryExpression
{
public:
        ANDExpression(Expression * arg1, Expression * arg2);
        ~ANDExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class ORExpression: public BinaryExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2);
        ~ORExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};

class NOTExpression: public BinaryExpression
{
public:
        NOTExpression(Expression * arg);
        ~NOTExpression();
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store);
};


#endif
