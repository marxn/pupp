#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include "constval.h"
#include "variable.h"
#include "node.h"

using namespace std;

enum ExpType {UnknownExpType, ConstExp, VarExp, MixedExp};

class Expression
{
public:
	Expression():ExpressType(UnknownExpType){}
	Expression(ExpType type):ExpressType(type){}
	virtual ~Expression(){}
	virtual ConstValue * GetValue() = 0;

	void SetExpType(ExpType type)
	{
		this->ExpressType = type;
	}
	ExpType GetExpType()
	{
		return this->ExpressType;
	}
	void SetParentNode(Node * node)
	{
		this->ParentNode = node;
	}

	virtual void TransformExpr() = 0;
protected:
	Node * ParentNode;
	ExpType ExpressType;
};
/*
class UnaryExpression: public Expression
{
public:
	UnaryExpression(Expression * expr):data(expr){}
	void TransformExpr()
	{
		data->SetParentNode(this->ParentNode);
		data->TransformExpr();
	}
	~UnaryExpression()
	{
	}
protected:
	Expression * data;
};
*/
class BinaryExpression: public Expression
{
public:
	BinaryExpression(Expression * arg1, Expression * arg2):left(arg1),right(arg2){}
	~BinaryExpression()
	{
	}
	void TransformExpr()
	{
		left->SetParentNode(this->ParentNode);
		right->SetParentNode(this->ParentNode);

		left->TransformExpr();
		right->TransformExpr();

		if(left->GetExpType()==VarExp || left->GetExpType()==MixedExp || right->GetExpType()==VarExp || right->GetExpType()==MixedExp)
                {
                        this->SetExpType(MixedExp);
                }
		else
		{
			this->SetExpType(ConstExp);
		}

	}

protected:
	Expression * left;
	Expression * right;
};

class ConstValueExpression: public Expression
{
public:
	ConstValueExpression():Value(NULL),Expression(ConstExp){}
	ConstValueExpression(ConstValue * value):Value(value),Expression(ConstExp){}
	~ConstValueExpression()
	{
	}
	ConstValue * GetValue()
	{
		return Value;
	}
	void TransformExpr()
	{
	}
private:
	ConstValue * Value;
};

class VarExpression: public Expression
{
public:
	VarExpression():Var(NULL),Expression(VarExp){}
	VarExpression(string * varname):VarName(varname),Expression(VarExp){}
	~VarExpression()
	{
	}
	ConstValue * GetValue()
	{
		if(this->Var) 
		{
			return this->Var->GetValue();
		}
		return NULL;
	}
	void TransformExpr()
	{
		this->Var = this->ParentNode->FindVariable(*VarName);
		if(this->Var==NULL)
		{
			cout<<"VarExpression::Variable "<<*VarName<<" not defined."<<endl;
		}
	}
private:
	string * VarName;
	Variable * Var;
};

class PlusExpression: public BinaryExpression
{
public:
	PlusExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~PlusExpression(){}
	ConstValue * GetValue()
	{
		if(this->Result)
		{
			delete this->Result;
		}
		Result = Operation::AddOperation(left->GetValue(), right->GetValue());
		return this->Result;
	}
private:
	ConstValue * Result;
};

class SubtractExpression: public BinaryExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue * GetValue()
	{
		return Operation::SubOperation(left->GetValue(), right->GetValue());
	}
};

class MultiplicationExpression: public BinaryExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue * GetValue()
	{
		return Operation::MulOperation(left->GetValue(), right->GetValue());
	}
};

class DivisionExpression: public BinaryExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue * GetValue()
	{
		return Operation::DivOperation(left->GetValue(), right->GetValue());
	}
};

class GTExpression: public BinaryExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue * GetValue()
	{
		return Operation::GTOperation(left->GetValue(), right->GetValue());
	}
};

class LTExpression: public BinaryExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue * GetValue()
	{
		return Operation::LTOperation(left->GetValue(), right->GetValue());
	}
};

class EQExpression: public BinaryExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue * GetValue()
	{
		return Operation::EQOperation(left->GetValue(), right->GetValue());
	}
};
#endif
