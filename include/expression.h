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

class BinaryExpression: public Expression
{
public:
	BinaryExpression(Expression * arg1, Expression * arg2):left(arg1),right(arg2),intermediate(NULL){}
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
			return false;
		}
		if(right->Transform(errstack)==false)
		{
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

class KVExpression: public BinaryExpression
{
public:
	KVExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2), left_store(NULL), right_store(NULL){}
	bool Transform(ErrorStack * errstack)
	{
		if(BinaryExpression::Transform(errstack)==false)
                {
                        return false;
                }
		this->SetDataType(KeyValue);
		return true;
	}

	ConstValue * GetValue()
	{
		this->left_store = this->left->GetValue()->DupValue();
		this->right_store = this->right->GetValue()->DupValue();
		this->intermediate = new KVValue(pair<ConstValue*, ConstValue*>(this->left_store, this->right_store));
		return this->intermediate;
	}

	void Swipe()
	{
		BinaryExpression::Swipe();
		delete this->left_store;
		delete this->right_store;
	}
private:
	ConstValue * left_store;
	ConstValue * right_store;
};

class SetExpression: public Expression
{
public:
	SetExpression(list<Expression*> * exp):exprlist(exp){}
	ConstValue * GetValue()
	{
		SetValue * invoker = new SetValue;
                
		list<KVExpression*>::iterator i;
                for(i = this->kvexprlist->begin(); i!=this->kvexprlist->end(); i++)
                {
			KVValue * kv = static_cast<KVValue*>((*i)->GetValue());
			invoker->AddKV(kv);
                }

		this->tempvalue = static_cast<ConstValue*>(invoker);
		return this->tempvalue;
	}
	bool Transform(ErrorStack * errstack)
	{
		if(this->exprlist!=NULL)
		{
			long index = 0;
			this->kvexprlist = new list<KVExpression*>;

			list<Expression*>::iterator i;
	                for(i = this->exprlist->begin(); i!=this->exprlist->end(); i++)
                	{
				(*i)->SetParentNode(this->ParentNode);
        	                if((*i)->Transform(errstack)!=true)
	                        {
                                	return false;
                        	}
				if((*i)->GetDataType()==KeyValue)
				{
					this->kvexprlist->push_back(static_cast<KVExpression*>(*i));
				}
				else
				{
					IntegerValue * key = new IntegerValue(index);
					KVExpression * kve = new KVExpression(new ConstValueExpression(key), *i);
					this->kvexprlist->push_back(kve);
					index++;
				}
			}
		}

		list<KVExpression*>::iterator i;
		for(i = this->kvexprlist->begin(); i!=this->kvexprlist->end(); i++)
		{
			(*i)->SetParentNode(this->ParentNode);
			if((*i)->Transform(errstack)!=true)
			{
				return false;
			}
		}
		this->SetDataType(Set);
		return true;
	}
	void Swipe()
	{
		list<KVExpression*>::iterator i;
                for(i = this->kvexprlist->begin(); i!=this->kvexprlist->end(); i++)
                {
                        (*i)->Swipe();
                }

		if(this->tempvalue)
                {
                        delete this->tempvalue;
                        this->tempvalue = NULL;
                }
	}
private:
	ConstValue * tempvalue;
	list<KVExpression*> * kvexprlist;
	list<Expression*> * exprlist;
};

class OffsetExpression: public BinaryExpression
{
public:
	OffsetExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        bool Transform(ErrorStack * errstack)
        {
                if(BinaryExpression::Transform(errstack)==false)
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

        ConstValue * GetValue()
        {
		SetValue * collection = static_cast<SetValue*>(this->left->GetValue());
		string key = this->right->GetValue()->toString();

                this->intermediate = collection->FindByKey(key);
		if(this->intermediate == NULL)
		{
			this->intermediate = new StringValue("");
		}
		else
		{
			this->intermediate = this->intermediate->DupValue();
		}
                return this->intermediate;
        }

        void Swipe()
        {
                BinaryExpression::Swipe();
        }
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
        bool Transform(ErrorStack * errstack)
        {
                if(BinaryExpression::Transform(errstack)==false)
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
	ConstValue * GetValue()
	{
		this->intermediate = Operation::AddOperation(left->GetValue(), right->GetValue());
		return this->intermediate;
	}
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

class ANDExpression: public LogicalExpression
{
public:
	ANDExpression(Expression * arg1, Expression * arg2):LogicalExpression(arg1, arg2){}
        ~ANDExpression(){}
        ConstValue * GetValue()
        {
                this->intermediate = Operation::ANDOperation(left->GetValue(), right->GetValue());
                return this->intermediate;
        }
};

class ORExpression: public LogicalExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2):LogicalExpression(arg1, arg2){}
        ~ORExpression(){}
        ConstValue * GetValue()
        {
                this->intermediate = Operation::OROperation(left->GetValue(), right->GetValue());
                return this->intermediate;
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
        ConstValue * GetValue()
        {
                this->intermediate = Operation::NOTOperation(right->GetValue());
                return this->intermediate;
        }
};





#endif
