#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <unistd.h>
#include <iostream>
#include "node.h"
#include "expression.h"

using namespace std;

class StatementNode :public Node
{
public:
	StatementNode(){}
	~StatementNode(){}
	void Swipe(){}
};

class BreakStatement: public StatementNode
{
public:
        bool Invoke()
        {
                Node * node = this->GetParentNode();
                if(node)
                {
                        node->SetNeedBreak(true);
                }
		return true;
        }
        bool Provision(ErrorStack * errstack)
	{
		return true;
        }
};

class ContinueStatement: public StatementNode
{
public:
        bool Invoke()
        {
                Node * node = this->GetParentNode();
                if(node)
		{
                        node->SetNeedContinue(true);
		}
		return true;
        }
        bool Provision(ErrorStack * errstack)
        {
		return true;
        }
};

class AssignStatement: public StatementNode
{
public:
        bool Invoke()
        {
		ConstValue * value = Expr->Calculate();
		if(this->Var->GetVarType()!=value->GetType() && this->Var->GetVarType()!=Any)
		{
			cerr<<"Data type mismatch"<<endl;

			delete value;
			return false;
		}
		this->Var->SetValue(value);
		delete value;

		return true;
        }
        void SetVariableName(string name)
        {
                this->VarName = name;
        }
        string GetVariableName()
        {
                return this->VarName;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision(ErrorStack * errstack)
        {
                this->Var = this->FindVariable(VarName);
                if(this->Var==NULL)
                {
                        errstack->PushFrame(0, "Variable "+this->VarName+" not defined");
                        return false;
                }
                if(this->Expr==NULL)
                {
                        errstack->PushFrame(0,"Illeagle Expression");
			return false;
                }

		this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision(errstack)==false)
		{
			errstack->PushFrame(0, "Provision expression falied.");
			return false;
		}
		
		return true;
        }
private:
        string VarName;
        Variable * Var;
        Expression * Expr;
};

class CollectionElementRef
{
public:
	CollectionElementRef(string * id, Expression * exp)
	{
		this->Var = *id;
		this->ExpList = new list<Expression*>;
		this->ExpList->push_back(exp);
	}
	string GetVarName()
	{
		return this->Var;
	}
	list<Expression*> * GetExpList()
	{
		return this->ExpList;
	}
	void AddOffsetExpr(Expression * expr)
	{
		this->ExpList->push_back(expr);
	}
private:
	string Var;
	list<Expression*> * ExpList;
};

class SetElementAssignStatement: public StatementNode
{
public:
	SetElementAssignStatement(CollectionElementRef * ref):Reference(ref){}
	bool Invoke()
        {
		if(this->Var->GetVarType()!=Set && this->Var->GetVarType()!=Any)
		{
			cerr<<"puppy warning: Cannot accept a non-collection variable."<<endl;
			return true;
		}
		if(this->Var->GetReference()->GetType()==Null)
		{
			this->Var->SetValue(new SetValue);
		}

		SetValue * vref = static_cast<SetValue*>(this->Var->GetReference());
		list<Expression*>* exprlist = this->Reference->GetExpList();

		int listsize = exprlist->size();

		bool need_clear = false;
		SetValue * last_set = NULL;
		ConstValue * last_offset_value = NULL;
		list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++, listsize--)
                {
			ConstValue * offset_value = (*i)->Calculate();
                        string offstr = offset_value->toString();

			if(vref->GetType()!=Set)
			{
				if(vref->GetType()==Null)
				{
					SetValue * stub = new SetValue;
					KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(last_offset_value, stub));
					last_set->AddKV(kv);

					delete kv;
					delete stub;

					vref = static_cast<SetValue*>(last_set->FindByKey(last_offset_value->toString()));
				}
				else
				{
					cerr<<"puppy warning: Cannot use a scalar value as a collection."<<endl;
					delete offset_value;
					return true;
				}
			}

			if(need_clear)
			{
				delete last_offset_value;
				need_clear = false;
			}

			ConstValue * value = vref->FindByKey(offstr);

			if(value==NULL)
			{
				if(listsize>1)
				{
					SetValue * stub = new SetValue;
					KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(offset_value, stub));
					vref->AddKV(kv);

					vref = static_cast<SetValue*>(vref->FindByKey(offstr));

					delete kv;
					delete value;
					delete stub;
				}
				else
				{
					ConstValue * target_value = Expr->Calculate();
					KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(offset_value, target_value));
					vref->AddKV(kv);

					delete kv;
					delete target_value;
				}
			}
			else
			{
				last_set = vref;
				last_offset_value = offset_value->DupValue();
				vref = static_cast<SetValue*>(value);
				need_clear = true;
			}
			
			delete offset_value;
		}

		return true;
        }
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
	bool Provision(ErrorStack * errstack)
        {
		this->Var = this->FindVariable(this->Reference->GetVarName());
                if(this->Var==NULL)
                {
                        errstack->PushFrame(0, "Variable "+this->Reference->GetVarName()+" not defined");
                        return false;
                }

		list<Expression*>* exprlist = this->Reference->GetExpList();

		list<Expression*>::iterator i;
		for(i=exprlist->begin(); i!=exprlist->end(); i++)
		{
			(*i)->SetParentNode(this->GetParentNode());
			if((*i)->Provision(errstack)==false)
	                {
                        	return false;
                	}
		}

                this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision(errstack)==false)
                {
                        return false;
                }

                return true;
        }
private:
	CollectionElementRef * Reference;
        Expression * Expr;
	Variable * Var;
};

class VarDefinitionStatement: public StatementNode
{
public:
	VarDefinitionStatement(list<string*> * list, DataType vartype):IdentList(list), VarType(vartype)
	{
	}
        bool Invoke()
        {
		return true;
        }
        bool Provision(ErrorStack * errstack)        
	{
                list<string*>::iterator i;
                Node * parent = GetParentNode();

                if(parent)
                {
                        for(i = IdentList->begin(); i != IdentList->end(); i++)
                        {
				if(parent->FindVariable(*(*i)))
				{
					errstack->PushFrame(0, "Duplicated variable: "+*(*i));
        		                return false;
				}

                                Variable * var = new Variable(*(*i));
				var->SetVarType(this->VarType);
				var->SetValue(new NullValue);
                                parent->AddVariable(var);
                        }
                }
		return true;
        }
private:
        list<string*> * IdentList;
        DataType VarType;
};

class PrintStatement: public StatementNode
{
public:
        bool Invoke()
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			ConstValue * value = (*i)->Calculate();
	                printf("%s",value->toString().c_str());
			fflush(stdout);
			
			delete value;
		}
		return true;
        }
        void SetExpressionList(list<Expression*> * exprlist)
        {
                this->ExprList = exprlist;
        }
        bool Provision(ErrorStack * errstack)
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			(*i)->SetParentNode(this->GetParentNode());
                	if((*i)->Provision(errstack)==false)
			{
                        	return false;
			}
		}
		return true;
        }
private:
        list<Expression*> * ExprList;
};

class SleepStatement: public StatementNode
{
public:
        bool Invoke()
        {
		ConstValue * value = this->Expr->Calculate();
		if(value->GetType()!=Integer)
                {
                        //TODO
			cerr<<"SLEEP Statement MUST have a Integer parameter "<<endl;
			return false;
                }

		usleep(static_cast<IntegerValue*>(value)->GetValue());

		delete value;
		return true;
        }
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision(ErrorStack * errstack)
        {
		this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision(errstack)==false)
                {
                        return false;
                }
                return true;
        }
private:
	Expression * Expr;
};

class ObjectStatement: public StatementNode
{
public:
	ObjectStatement(string * objname, string * method_name, list<Expression*> * expr_list): ObjName(*objname), ExprList(expr_list)
	{
		this->MethodName = *method_name;
	}
	bool Invoke()
	{
		//this->FindMethodMapping(this->ObjName, this->MethodName);
		return true;
	}
	bool Provision(ErrorStack * errstack)
	{
		list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        (*i)->SetParentNode(this->GetParentNode());
                        if((*i)->Provision(errstack)==false)
                        {
                                errstack->PushFrame(0, "PRINT Statement failed in transforming expressions ");
                                return false;
                        }
                }

		Variable * var = this->FindVariable(this->ObjName);
                if(var==NULL)
                {
                        errstack->PushFrame(0, "Object "+this->ObjName+" not defined");
                        return false;
                }

                return true;
	}
private:
	list<Expression*> * ExprList;
	string ObjName;
	string MethodName;
};
#endif

