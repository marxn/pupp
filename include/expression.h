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

class Expression: public PuppyObject
{
public:
	Expression()
	{
	}
	virtual ~Expression(){}
	virtual ConstValue * Calculate() = 0;

	void SetParentNode(Node * node)
	{
		this->ParentNode = node;
	}

	virtual bool Provision(ErrorStack * errstack) = 0;

protected:
	Node * ParentNode;
};

class ConstValueExpression: public Expression
{
public:
        ConstValueExpression():Value(NULL){}
        ConstValueExpression(ConstValue * value):Value(value){}
        ~ConstValueExpression()
        {
        }
        ConstValue * Calculate()
        {
                return Value->DupValue();
        }
        bool Provision(ErrorStack * errstack)
        {
                return true;
        }
private:
        ConstValue * Value;
};

class BinaryExpression: public Expression
{
public:
	BinaryExpression(Expression * arg1, Expression * arg2):left(arg1),right(arg2)
	{
	}

	ConstValue * Calculate()
	{
		this->left_store = this->left->Calculate();
                this->right_store = this->right->Calculate();

		ConstValue * result = this->CarryOut();

		delete this->left_store;
		delete this->right_store;

		this->left_store = NULL;
		this->right_store = NULL;

		return result;
	}

	virtual ConstValue * CarryOut() = 0;

	bool Provision(ErrorStack * errstack)
	{
		left->SetParentNode(this->ParentNode);
		right->SetParentNode(this->ParentNode);

		if(left->Provision(errstack)==false)
		{
			return false;
		}
		if(right->Provision(errstack)==false)
		{
                        return false;
		}

		return true;
	}

protected:
	Expression * left;
	Expression * right;
	ConstValue * left_store;
	ConstValue * right_store;
};

class KVExpression: public BinaryExpression
{
public:
	KVExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2)
	{
	}
	bool Provision(ErrorStack * errstack)
	{
		if(BinaryExpression::Provision(errstack)==false)
                {
                        return false;
                }
		return true;
	}

	ConstValue * CarryOut()
	{
		return new KVValue(pair<ConstValue*, ConstValue*>(this->left_store, this->right_store));
	}
};

class SetExpression: public Expression
{
public:
	SetExpression(list<Expression*> * exp):ExprList(exp){}
	ConstValue * Calculate()
	{
		long index = 0;
		SetValue * result = new SetValue;
                list<KVValue*> tobedone;

		list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
			ConstValue * value = (*i)->Calculate();
			if(value->GetType()==KeyValue)
			{
				tobedone.push_back(static_cast<KVValue*>(value));
			}
			else
			{
				IntegerValue * key = new IntegerValue(index);
				KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(key, value));
				result->AddKV(kv);
                                delete kv;
                                index++;
			}
                }

		list<KVValue*>::iterator j;
		for(j = tobedone.begin(); j!=tobedone.end(); j++)
                {
			KVValue * value = *j;
                        result->AddKV(value);
			delete value;
                }

		return result;
	}
	bool Provision(ErrorStack * errstack)
	{
		list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
               	{
			(*i)->SetParentNode(this->ParentNode);
       	                if((*i)->Provision(errstack)!=true)
                        {
                               	return false;
                       	}
		}

		return true;
	}
private:
	list<Expression*> * ExprList;
};

class OffsetExpression: public BinaryExpression
{
public:
	OffsetExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}

        ConstValue * CarryOut()
        {
		if(this->left_store->GetType()!=Set)
		{
			//TODO
                        //errstack->PushFrame(0, "Expect a collection to the left of '['.");
                        return new NullValue;
                }

		SetValue * collection = static_cast<SetValue*>(this->left_store);
		ConstValue * key = this->right_store;

                ConstValue * result = collection->FindByKey(key->toString());
		if(result == NULL)
		{
			result = new NullValue;
		}
		else
		{
			result = result->DupValue();
		}

                return result;
        }
};

class VarExpression: public Expression
{
public:
	VarExpression():Var(NULL){}
	VarExpression(string * varname):VarName(varname){}
	~VarExpression(){}
	ConstValue * Calculate()
	{
		if(this->Var) 
		{
			return this->Var->GetValue();
		}
		return NULL;
	}
	bool Provision(ErrorStack * errstack)
	{
		this->Var = this->ParentNode->FindVariable(*VarName);
		if(this->Var==NULL)
		{
			errstack->PushFrame(this->GetObjLoc(), "Variable "+*VarName+" not defined");
                        return false;
		}
		return true;
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
	ConstValue * CarryOut()
	{
		return Operation::AddOperation(this->left_store, this->right_store);
	}
};

class SubtractExpression: public BinaryExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::SubOperation(this->left_store, this->right_store);
	}
};

class MultiplicationExpression: public BinaryExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::MulOperation(this->left_store, this->right_store);
	}
};

class DivisionExpression: public BinaryExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::DivOperation(this->left_store, this->right_store);
	}
};

class GTExpression: public BinaryExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::GTOperation(this->left_store, this->right_store);
	}
};

class LTExpression: public BinaryExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::LTOperation(this->left_store, this->right_store);
	}
};

class EQExpression: public BinaryExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::EQOperation(this->left_store, this->right_store);
	}
};

class NEQExpression: public BinaryExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~NEQExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::NEQOperation(this->left_store, this->right_store);
        }
};

class GEExpression: public BinaryExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~GEExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::GEOperation(this->left_store, this->right_store);
        }
};

class LEExpression: public BinaryExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~LEExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::LEOperation(this->left_store, this->right_store);
        }
};

class ANDExpression: public BinaryExpression
{
public:
	ANDExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ANDExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::ANDOperation(this->left_store, this->right_store);
        }
};

class ORExpression: public BinaryExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ORExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::OROperation(this->left_store, this->right_store);
        }
};

class NOTExpression: public BinaryExpression
{
public:
        NOTExpression(Expression * arg):BinaryExpression(NULL, arg)
	{
		this->left = new ConstValueExpression(new BooleanValue(true));
	}
        ~NOTExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::NOTOperation(this->right_store);
        }
};





#endif
