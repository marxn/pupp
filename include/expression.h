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
#include "errstack.h"

using namespace std;

enum ExpType {UnknownExpType, ConstExp, VarExp, MixedExp};

class Expression: public PuppyObject
{
public:
	Expression():ExpressType(UnknownExpType), ExpDataType(UnknownDataType)
	{
	}
	Expression(ExpType type):ExpressType(type), ExpDataType(UnknownDataType)
	{
	}
	virtual ~Expression(){}
	virtual ConstValue * GetValue() = 0;
	virtual void Swipe(){}

	void SetExpType(ExpType type)
	{
		this->ExpressType = type;
	}
	ExpType GetExpType()
	{
		return this->ExpressType;
	}
	void SetDataType(DataType type)
	{
		this->ExpDataType = type;
	}
	DataType GetDataType()
	{
		return this->ExpDataType;
	}
	void SetParentNode(Node * node)
	{
		this->ParentNode = node;
	}

	virtual bool Transform(ErrorStack * errstack) = 0;

protected:
	Node * ParentNode;
	DataType ExpDataType;
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
	~BinaryExpression(){}
	
	void Swipe()
	{
		left->Swipe();
		right->Swipe();
		if(this->intermediate)
		{
			delete this->intermediate;
			this->intermediate = NULL;
		}
	}
	bool Transform(ErrorStack * errstack)
	{
		left->SetParentNode(this->ParentNode);
		right->SetParentNode(this->ParentNode);

		if(left->Transform(errstack)==false)
		{
			errstack->PushFrame(0, "Transform left failed.");
			return false;
		}
		if(right->Transform(errstack)==false)
		{
			errstack->PushFrame(0, "Transform right failed.");
                        return false;
		}

		if(left->GetExpType()==VarExp || left->GetExpType()==MixedExp || right->GetExpType()==VarExp || right->GetExpType()==MixedExp)
                {
                        this->SetExpType(MixedExp);
                }
		else
		{
			this->SetExpType(ConstExp);
		}

		return true;
	}

protected:
	Expression * left;
	Expression * right;
	ConstValue * intermediate;
};

class ArithmeticExpression: public BinaryExpression
{
public:
	ArithmeticExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	bool Transform(ErrorStack * errstack)
	{
		if(BinaryExpression::Transform(errstack)==false)
		{
			return false;
		}

		DataType type = Operation::GetOperationRetType(left->GetDataType(), right->GetDataType());
                if(type==UnknownDataType)
                {
                        errstack->PushFrame(0, "value type of expression is undeterminated.");
                        return false;
                }
		this->SetDataType(type);
		return true;
	}
};

class RelationExpression: public BinaryExpression
{
public:
	RelationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        bool Transform(ErrorStack * errstack)
        {
                if(BinaryExpression::Transform(errstack)==false)
                {
                        return false;
                }

                this->SetDataType(Boolean);
                return true;
        }
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
	bool Transform(ErrorStack * errstack)
	{
		this->SetDataType(this->Value->GetType());
		return true;
	}
private:
	ConstValue * Value;
};

class VarExpression: public Expression
{
public:
	VarExpression():Var(NULL),Expression(VarExp){}
	VarExpression(string * varname):VarName(varname),Expression(VarExp){}
	~VarExpression(){}
	ConstValue * GetValue()
	{
		if(this->Var) 
		{
			return this->Var->GetValue();
		}
		return NULL;
	}
	bool Transform(ErrorStack * errstack)
	{
		this->Var = this->ParentNode->FindVariable(*VarName);
		if(this->Var==NULL)
		{
			errstack->PushFrame(0, "Variable "+*VarName+" not defined");
                        return false;
		}
		this->SetDataType(this->Var->GetType());
		return true;
	}
private:
	string * VarName;
	Variable * Var;
};

class PlusExpression: public ArithmeticExpression 
{
public:
	PlusExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~PlusExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::AddOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
private:
	ConstValue * Result;
};

class SubtractExpression: public ArithmeticExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::SubOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class MultiplicationExpression: public ArithmeticExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::MulOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class DivisionExpression: public ArithmeticExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::DivOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class GTExpression: public RelationExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::GTOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class LTExpression: public RelationExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::LTOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class EQExpression: public RelationExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue * GetValue()
	{
		this->intermediate = Operation::EQOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
};

class NEQExpression: public RelationExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~NEQExpression(){}
        ConstValue * GetValue()
        {
                this->intermediate = Operation::NEQOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
        }
};

class GEExpression: public RelationExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~GEExpression(){}
        ConstValue * GetValue()
        {
                this->intermediate = Operation::GEOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
        }
};

class LEExpression: public RelationExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~LEExpression(){}
        ConstValue * GetValue()
        {
                this->intermediate = Operation::LEOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
        }
};

#endif
