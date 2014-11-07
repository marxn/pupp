#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include "constval.h"
#include "variable.h"
#include "nodecontext.h"

using namespace std;

class Expression
{
public:
	virtual ~Expression(){}
	virtual ConstValue GetValue() = 0;

	void SetRelatedContext(NodeContext * context)
	{
		this->RelatedContext = context;
	}

	virtual void TransformExpr() = 0;
protected:
	NodeContext * RelatedContext;
};

class UnaryExpression: public Expression
{
public:
	UnaryExpression(Expression * expr):data(expr){}
	void TransformExpr()
	{
		data->SetRelatedContext(this->RelatedContext);
		data->TransformExpr();
	}
	~UnaryExpression()
	{
		delete data;
	}
protected:
	Expression * data;
};

class BinaryExpression: public Expression
{
public:
	BinaryExpression(Expression * arg1, Expression * arg2):left(arg1),right(arg2){}
	~BinaryExpression()
	{
		delete left;
		delete right;
	}
	void TransformExpr()
	{
		left->SetRelatedContext(this->RelatedContext);
		right->SetRelatedContext(this->RelatedContext);
		left->TransformExpr();
		right->TransformExpr();
	}

protected:
	Expression * left;
	Expression * right;
};

class FinalExpression: public Expression
{
public:
	FinalExpression():value(NULL),varName(NULL){}
	FinalExpression(ConstValue * arg): value(arg),varName(NULL){}
	FinalExpression(string * arg): value(NULL),varName(arg){}
	~FinalExpression()
	{
		if(value)
			delete value;
	}
	void TransformExpr()
	{
		//nothing
	}
	ConstValue GetValue()
	{
		if(value)
			return *value;
		else
		{
			Variable * var = this->RelatedContext->FindVariable(*varName);
			return var->GetValue();
		}
	}
private:
	ConstValue * value;
	string     * varName;
};

class PlusExpression: public BinaryExpression
{
public:
	PlusExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~PlusExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() + right->GetValue();
	}
};

class SubtractExpression: public BinaryExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() - right->GetValue();
	}
};

class MultiplicationExpression: public BinaryExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() * right->GetValue();
	}
};

class DivisionExpression: public BinaryExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() / right->GetValue();
	}
};

class GTExpression: public BinaryExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() > right->GetValue();
	}
};

class LTExpression: public BinaryExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() < right->GetValue();
	}
};

class EQExpression: public BinaryExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue GetValue()
	{
		return left->GetValue() == right->GetValue();
	}
};
#endif
