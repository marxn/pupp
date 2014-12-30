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
	Expression(): ExpDataType(UnknownDataType)
	{
	}
	virtual ~Expression(){}
	virtual ConstValue * Calculate() = 0;
	virtual void Swipe(){}

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

	virtual bool Provision(ErrorStack * errstack) = 0;

protected:
	Node * ParentNode;
	DataType ExpDataType;
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
                this->SetDataType(this->Value->GetType());
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
		this->SetDataType(KeyValue);
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
	SetExpression(list<Expression*> * exp):exprlist(exp){}
	ConstValue * Calculate()
	{
		SetValue * result = new SetValue;
                
		list<KVExpression*>::iterator i;
                for(i = this->kvexprlist->begin(); i!=this->kvexprlist->end(); i++)
                {
			KVValue * kv = static_cast<KVValue*>((*i)->Calculate());
			result->AddKV(kv);
			delete kv;
                }

		return result;
	}
	bool Provision(ErrorStack * errstack)
	{
		if(this->exprlist!=NULL)
		{
			//Transform the elements into key-value pair

			list<Expression*> tobedone;

			this->kvexprlist = new list<KVExpression*>;

			list<Expression*>::iterator i;
	                for(i = this->exprlist->begin(); i!=this->exprlist->end(); i++)
                	{
				(*i)->SetParentNode(this->ParentNode);
        	                if((*i)->Provision(errstack)!=true)
	                        {
                                	return false;
                        	}
				if((*i)->GetDataType()==KeyValue)
				{
					tobedone.push_back(*i);
				}
				else
				{
					IntegerValue * key = new IntegerValue(this->Index);
					KVExpression * kve = new KVExpression(new ConstValueExpression(key), *i);
					this->kvexprlist->push_back(kve);
					this->Index++;
				}
			}
			for(i = tobedone.begin(); i!=tobedone.end(); i++)
                        {
				this->kvexprlist->push_back(static_cast<KVExpression*>(*i));
			}
		}

		list<KVExpression*>::iterator i;
		for(i = this->kvexprlist->begin(); i!=this->kvexprlist->end(); i++)
		{
			(*i)->SetParentNode(this->ParentNode);
			if((*i)->Provision(errstack)!=true)
			{
				return false;
			}
		}
		this->SetDataType(Set);
		return true;
	}
	long GetIndex()
	{
		return this->Index;
	}
private:
	long Index;
	list<KVExpression*> * kvexprlist;
	list<Expression*> * exprlist;
};

class OffsetExpression: public BinaryExpression
{
public:
	OffsetExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        bool Provision(ErrorStack * errstack)
        {
                if(BinaryExpression::Provision(errstack)==false)
                {
                        return false;
                }
		if(this->left->GetDataType()!=Set)
		{
			errstack->PushFrame(0, "Expect a collection to the left of '['.");
                        return false;
		}
                this->SetDataType(Offset);
                return true;
        }

        ConstValue * CarryOut()
        {
		SetValue * collection = static_cast<SetValue*>(this->left_store);
		ConstValue * key = this->right_store;

                ConstValue * result = collection->FindByKey(key->toString());
		if(result == NULL)
		{
			result = new StringValue("");
		}
		else
		{
			result = result->DupValue();
		}

                return result;
        }
};

class ArithmeticExpression: public BinaryExpression
{
public:
	ArithmeticExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	bool Provision(ErrorStack * errstack)
	{
		if(BinaryExpression::Provision(errstack)==false)
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
        bool Provision(ErrorStack * errstack)
        {
                if(BinaryExpression::Provision(errstack)==false)
                {
                        return false;
                }
		if(left->GetDataType()!=Integer && left->GetDataType()!=Float || right->GetDataType()!=Integer &&  right->GetDataType()!=Float)
		{
			errstack->PushFrame(0, "Relation expression expect numeric input.");
                        return false;
		}
                this->SetDataType(Boolean);
                return true;
        }
};

class LogicalExpression: public BinaryExpression
{
public:
        LogicalExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        bool Provision(ErrorStack * errstack)
        {
                if(BinaryExpression::Provision(errstack)==false)
                {
                        return false;
                }
		if(left->GetDataType()!=Boolean || right->GetDataType()!=Boolean)
		{
			errstack->PushFrame(0, "Logical expression expect input of boolean type.");
                        return false;
		}
                this->SetDataType(Boolean);
                return true;
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
	ConstValue * CarryOut()
	{
		return Operation::AddOperation(this->left_store, this->right_store);
	}
};

class SubtractExpression: public ArithmeticExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::SubOperation(this->left_store, this->right_store);
	}
};

class MultiplicationExpression: public ArithmeticExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::MulOperation(this->left_store, this->right_store);
	}
};

class DivisionExpression: public ArithmeticExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):ArithmeticExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::DivOperation(this->left_store, this->right_store);
	}
};

class GTExpression: public RelationExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::GTOperation(this->left_store, this->right_store);
	}
};

class LTExpression: public RelationExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::LTOperation(this->left_store, this->right_store);
	}
};

class EQExpression: public RelationExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue * CarryOut()
	{
		return Operation::EQOperation(this->left_store, this->right_store);
	}
};

class NEQExpression: public RelationExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~NEQExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::NEQOperation(this->left_store, this->right_store);
        }
};

class GEExpression: public RelationExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~GEExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::GEOperation(this->left_store, this->right_store);
        }
};

class LEExpression: public RelationExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2):RelationExpression(arg1, arg2){}
        ~LEExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::LEOperation(this->left_store, this->right_store);
        }
};

class ANDExpression: public LogicalExpression
{
public:
	ANDExpression(Expression * arg1, Expression * arg2):LogicalExpression(arg1, arg2){}
        ~ANDExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::ANDOperation(this->left_store, this->right_store);
        }
};

class ORExpression: public LogicalExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2):LogicalExpression(arg1, arg2){}
        ~ORExpression(){}
        ConstValue * CarryOut()
        {
                return Operation::OROperation(this->left_store, this->right_store);
        }
};

class NOTExpression: public LogicalExpression
{
public:
        NOTExpression(Expression * arg):LogicalExpression(NULL, arg)
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
