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
#include "function.h"
#include "portal.h"

using namespace std;

class Expression: public PuppyObject
{
public:
	Expression():lValue(false)
	{
	}
	virtual ~Expression(){}
	virtual ConstValue * Calculate(NodeContext * context) = 0;

	void SetParentNode(Node * node)
	{
		this->ParentNode = node;
	}

	virtual bool Provision(ErrorStack * errstack) = 0;
	virtual bool Check(ErrorStack * errstack) 
	{
		return true;
	}
	void LValue(bool flag)
	{
		this->lValue = flag;
	}
	bool isLValue()
	{
		return this->lValue;
	}
protected:
	bool lValue;
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
        ConstValue * Calculate(NodeContext * context)
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

	ConstValue * Calculate(NodeContext * context)
	{
		ConstValue * left_store = this->left->Calculate(context);
                ConstValue * right_store = this->right->Calculate(context);

		ConstValue * result = this->CarryOut(left_store, right_store);

		delete left_store;
		delete right_store;

		return result;
	}

	virtual ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store) = 0;

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
	bool Check(ErrorStack * errstack)
        {
                if(left->Check(errstack)==false)
                {
                        return false;
                }
                if(right->Check(errstack)==false)
                {
                        return false;
                }

                return true;
        }


protected:
	Expression * left;
	Expression * right;
};

class KVExpression: public BinaryExpression
{
public:
	KVExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2)
	{
	}

	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return new KVValue(pair<ConstValue*, ConstValue*>(left_store, right_store));
	}
};

class SetExpression: public Expression
{
public:
	SetExpression(list<Expression*> * exp):ExprList(exp){}
	ConstValue * Calculate(NodeContext * context)
	{
		long index = 0;
		SetValue * result = new SetValue;
                list<KVValue*> tobedone;

		list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
			ConstValue * value = (*i)->Calculate(context);
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
				delete key;
				delete value;
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
	bool Check(ErrorStack * errstack)
        {
                list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
                        if((*i)->Check(errstack)!=true)
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

        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
		if(left_store->GetType()!=Set)
		{
			//TODO
                        //errstack->PushFrame(0, "Expect a collection to the left of '['.");
                        return new NullValue;
                }

		SetValue * collection = static_cast<SetValue*>(left_store);
		ConstValue * key = right_store;

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
	VarExpression()
	{
		this->lValue = true;
	}
	VarExpression(string * varname):VarName(varname)
	{
		this->lValue = true;
	}
	~VarExpression(){}
	ConstValue * Calculate(NodeContext * context)
	{
		Variable * var = context->GetVariable(*VarName);
		if(var==NULL)
		{
			var = context->GetPortal()->GetSharedVariable(this->VarDef);
			if(var==NULL)
			{
				cerr<<"puppy runtime error: cannot find variable:"<<*VarName<<endl;
				return NULL; 
			}
		}
		return var->GetValue();
	}
	string GetVarName()
	{
		return *VarName;
	}
	VariableDef * GetVarDef()
	{
		return this->VarDef;
	}
	bool Provision(ErrorStack * errstack)
	{
		return true;
	}
	bool Check(ErrorStack * errstack)
	{
		VariableDef * vardef = this->ParentNode->FindVariable(*VarName);
		if(vardef==NULL)
		{
			errstack->PushFrame(this->GetObjLoc(), "Variable "+*VarName+" has not been defined");	
			return false;
		}
		this->VarDef = vardef;
		return Expression::Check(errstack);
	}
private:
	string * VarName;
	VariableDef * VarDef;
};

class FunctionExpression: public Expression
{
public:
	FunctionExpression(string * funcname, list<Expression*> * exprlist)
	{
		this->FuncName = *funcname;
		this->ExprList = exprlist;
	}
	ConstValue * Calculate(NodeContext * context)
        {
		ConstValue * result = NULL;

                if(this->Func)
                {
			int rtn = 0;

			NodeContext * new_ctx = new NodeContext(context->GetPortal());
			new_ctx->AddFrame(this->Func);

			list<Expression*>::iterator i;
			list<FuncArgDef*>::iterator j;

			for(i = this->ExprList->begin(), j = this->Func->GetArgList()->begin(); 
				i!=this->ExprList->end() && j!=this->Func->GetArgList()->end(); i++,j++)
			{
				if((*j)->isRef())
				{
					VarExpression * exp = static_cast<VarExpression*>(*i);

					Variable * var = context->GetVariable(exp->GetVarName());
					if(var==NULL)
			                {
                        			var = context->GetPortal()->GetSharedVariable(exp->GetVarDef());
			                        if(var==NULL)
                        			{
			                                cerr<<"Puppy runtime error: cannot find variable: "<<exp->GetVarName()<<endl;
                        			        return new NullValue;
			                        }
			                }

					Variable * avatar = new_ctx->GetVariable((*j)->GetName());
					if(avatar)
					{
						avatar->SetRefVar(var->GetRealVar());
					}
				}
				else
				{
					ConstValue * value = (*i)->Calculate(context);
					if(value->GetType()!=(*j)->GetType() && (*j)->GetType()!=Any)
					{
						delete new_ctx;
						cerr<<"puppy runtime error: data type mismatch when calling a function."<<endl;
						return new NullValue;
					}

					string argname = (*j)->GetName();
					Variable * localvar = new_ctx->GetVariable(argname);
					if(localvar)
					{
						localvar->SetValue(value);
					}

					delete value;
				}
			}

			rtn = this->Func->Run(new_ctx);
			if(rtn==NODE_RET_NEEDRETURN)
			{
				result = new_ctx->FunctionRet;
			}
			else
			{
				result = new NullValue;
			}

			delete new_ctx;
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
	bool Check(ErrorStack * errstack)
	{
		this->Func = static_cast<FunctionNode*>(this->ParentNode->FindFunctionDef(this->FuncName));
		if(this->Func==NULL)
                {
                        errstack->PushFrame(this->GetObjLoc(), "Function "+this->FuncName+"() has not been defined");
                        return false;
                }
		if(this->Func->GetArgList()->size()!=this->ExprList->size())
                {
                        errstack->PushFrame(0, "Number of arguments mismatch");
                        return false;
                }

		list<Expression*>::iterator i;
                list<FuncArgDef*>::iterator j;

		for(i = this->ExprList->begin(), j = this->Func->GetArgList()->begin();
                        i!=this->ExprList->end() && j!=this->Func->GetArgList()->end(); i++,j++)
                {
                        if((*i)->Check(errstack)!=true)
                        {
                                return false;
                        }
			if((*j)->isRef())
			{
				if((*i)->isLValue()==false)
				{
					errstack->PushFrame(0, "Must pass a lvalue to a reference:" + (*j)->GetName());
					return false;
				}
			}
                }

		return Expression::Check(errstack);
	}
private:
	string FuncName;
	FunctionNode * Func;
	list<Expression*> * ExprList;
};


class PlusExpression: public BinaryExpression 
{
public:
	PlusExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~PlusExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::AddOperation(left_store, right_store);
	}
};

class SubtractExpression: public BinaryExpression
{
public:
	SubtractExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~SubtractExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::SubOperation(left_store, right_store);
	}
};

class MultiplicationExpression: public BinaryExpression
{
public:
	MultiplicationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~MultiplicationExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::MulOperation(left_store, right_store);
	}
};

class DivisionExpression: public BinaryExpression
{
public:
	DivisionExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~DivisionExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::DivOperation(left_store, right_store);
	}
};

class GTExpression: public BinaryExpression
{
public:
	GTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~GTExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::GTOperation(left_store, right_store);
	}
};

class LTExpression: public BinaryExpression
{
public:
	LTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~LTExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::LTOperation(left_store, right_store);
	}
};

class EQExpression: public BinaryExpression
{
public:
	EQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
	~EQExpression(){}
	ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
	{
		return Operation::EQOperation(left_store, right_store);
	}
};

class NEQExpression: public BinaryExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~NEQExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::NEQOperation(left_store, right_store);
        }
};

class GEExpression: public BinaryExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~GEExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::GEOperation(left_store, right_store);
        }
};

class LEExpression: public BinaryExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~LEExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::LEOperation(left_store, right_store);
        }
};

class ANDExpression: public BinaryExpression
{
public:
	ANDExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ANDExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::ANDOperation(left_store, right_store);
        }
};

class ORExpression: public BinaryExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ORExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::OROperation(left_store, right_store);
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
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::NOTOperation(right_store);
        }
};





#endif
