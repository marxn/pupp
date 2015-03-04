#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <unistd.h>
#include <iostream>
#include "node.h"
#include "expression.h"
#include "portal.h"

using namespace std;

class StatementNode :public Node
{
public:
	StatementNode(){}
	~StatementNode(){}
	void Swipe(NodeContext * context){}
};

class BreakStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
		return NODE_RET_NEEDBREAK;
        }
        bool Provision()
	{
		return true;
        }
};

class ContinueStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
		return NODE_RET_NEEDCONTINUE;
        }
        bool Provision()
        {
		return true;
        }
};

class AssignStatement: public StatementNode
{
public:
	AssignStatement():VarDef(NULL){}

        int Invoke(NodeContext * context)
        {
		Variable * var = context->GetVariable(this->VarName);
		if(var==NULL)
		{
			var = context->GetPortal()->GetSharedVariable(this->VarDef);
			if(var==NULL)
			{
				cerr<<"Puppy runtime error: cannot find variable: "<<this->VarName<<endl;
				return NODE_RET_ERROR;
			}
		}

		ConstValue * value = Expr->Calculate(context);
		if(value==NULL)
		{
			return NODE_RET_ERROR;
		}

		if(var->GetVarType()!=value->GetType() && var->GetVarType()!=Any)
		{
			cerr<<"puppy runtime error: Data type mismatch"<<endl;

			delete value;
			return false;
		}
		var->SetValue(value);
		delete value;
		return NODE_RET_NORMAL;
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
        bool Provision()
        {
		Node * parent = this->GetParentNode();

		this->Expr->SetParentNode(parent);
                if(this->Expr->Provision()==false)
		{
			return false;
		}
		return true;
        }
	bool Check()
	{
		Node * parent = this->GetParentNode();
                VariableDef * vardef = parent->FindVariable(VarName);
                if(vardef==NULL)
                {
                        cerr<<"puppy provision error: Variable "<<this->VarName+"has not defined."<<endl;
                        return false;
                }
		this->VarDef = vardef;

		return this->Expr->Check();
	}
private:
        string VarName;
	VariableDef * VarDef;
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
	int Invoke(NodeContext * context)
        {
		Variable * var = context->GetVariable(this->VarDef->GetVarName());
                if(var==NULL)
                {
			var = context->GetPortal()->GetSharedVariable(this->VarDef);
                        if(var==NULL)
			{
                        	cerr<<"puppy runtime error: cannot find variable: "<<this->VarDef->GetVarName()<<endl;
                        	return NODE_RET_ERROR;
			}
                }

		if(var->GetValueType()!=Set && var->GetValueType()!=Any)
		{
			cerr<<"puppy runtime error: Cannot accept a non-collection variable: "<<var->GetVarName()<<endl;
			return NODE_RET_ERROR;
		}

		if(var->GetReference()->GetType()==Null)
		{
			var->SetValue(new SetValue);
		}

		SetValue * vref = static_cast<SetValue*>(var->GetReference());
		list<Expression*>* exprlist = this->Reference->GetExpList();

		int listsize = exprlist->size();

		bool need_clear = false;
		SetValue * last_set = NULL;
		ConstValue * last_offset_value = NULL;
		list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++, listsize--)
                {
			ConstValue * offset_value = (*i)->Calculate(context);
			if(offset_value==NULL)
			{
				return NODE_RET_ERROR;
			}

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
					cerr<<"puppy runtime warning: Cannot use a scalar value as a collection."<<endl;

					if(need_clear)
                        		{
                	                	delete last_offset_value;
	        	                        need_clear = false;
		                        }

					delete offset_value;
					return NODE_RET_NORMAL;
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
					ConstValue * target_value = Expr->Calculate(context);
					if(target_value==NULL)
					{
						return NODE_RET_ERROR;
					}

					KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(offset_value, target_value));
					vref->AddKV(kv);

					delete kv;
					delete target_value;
				}
			}
			else
			{
				if(listsize>1)
				{
					last_set = vref;
					last_offset_value = offset_value->DupValue();
					vref = static_cast<SetValue*>(value);
					need_clear = true;
				}
				else
				{
					ConstValue * target_value = Expr->Calculate(context);
					if(target_value==NULL)
                                        {
                                                return NODE_RET_ERROR;
                                        }

                                        KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(offset_value, target_value));
                                        vref->AddKV(kv);

                                        delete kv;
                                        delete target_value;
				}
			}
			
			delete offset_value;
		}

		return NODE_RET_NORMAL;
        }
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
	bool Provision()
        {
		list<Expression*>* exprlist = this->Reference->GetExpList();

		list<Expression*>::iterator i;
		for(i=exprlist->begin(); i!=exprlist->end(); i++)
		{
			(*i)->SetParentNode(this->GetParentNode());
			if((*i)->Provision()==false)
	                {
                        	return false;
                	}
		}

                this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision()==false)
                {
                        return false;
                }

                return true;
        }
	bool Check()
	{
		this->VarDef = this->FindVariable(this->Reference->GetVarName());
                if(this->VarDef==NULL)
                {
                        cerr<<"puppy provision error: Variable "<<this->Reference->GetVarName()<<"has not defined"<<endl;
                        return false;
                }

		list<Expression*>* exprlist = this->Reference->GetExpList();

                list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++)
                {
                        if((*i)->Check()==false)
                        {
                                return false;
                        }
                }

                if(this->Expr->Check()==false)
                {
                        return false;
                }

                return true;

	}
private:
	CollectionElementRef * Reference;
        Expression * Expr;
	VariableDef * VarDef;
};

class VarDefinitionStatement: public StatementNode
{
public:
	VarDefinitionStatement(list<string*> * list, DataType vartype):IdentList(list), VarType(vartype)
	{
	}
        int Invoke(NodeContext * context)
        {
		return NODE_RET_NORMAL;
        }
        bool Provision()
	{
                list<string*>::iterator i;
                Node * parent = this->GetParentNode();
                if(parent)
                {
                        for(i = IdentList->begin(); i != IdentList->end(); i++)
                        {
				if(parent->FindVariable(*(*i)))
				{
					cerr<<"puppy provision error: Duplicated variable: "<<**i<<endl;
        		                return false;
				}

                                VariableDef * vardef = new VariableDef(*(*i));
				vardef->SetVarType(this->VarType);
                                parent->AddVariable(vardef);
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
        int Invoke(NodeContext * context)
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			ConstValue * value = (*i)->Calculate(context);
			if(value==NULL)
                        {
 	                       return NODE_RET_ERROR;
                        }

	                fprintf(stdout, "%s", value->toString().c_str());
			fflush(stdout);
			
			delete value;
		}
		return NODE_RET_NORMAL;
        }
        void SetExpressionList(list<Expression*> * exprlist)
        {
                this->ExprList = exprlist;
        }
        bool Provision()
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			(*i)->SetParentNode(this->GetParentNode());
                	if((*i)->Provision()==false)
			{
                        	return false;
			}
		}
		return true;
        }
	bool Check()
        {
                list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        if((*i)->Check()==false)
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
        int Invoke(NodeContext * context)
        {
		ConstValue * value = this->Expr->Calculate(context);
		if(value==NULL)
                {
	                return NODE_RET_ERROR;
                }

		if(value->GetType()!=Integer)
                {
                        //TODO
			cerr<<"puppy runtime error: SLEEP Statement need a Integer parameter. "<<endl;
			return NODE_RET_ERROR;
                }

		usleep(static_cast<IntegerValue*>(value)->GetValue());

		delete value;
		return NODE_RET_NORMAL;
        }
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
		this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
        }
	bool Check()
        {
                return this->Expr->Check();
        }

private:
	Expression * Expr;
};

class ReturnStatement: public StatementNode
{
public:
	int Invoke(NodeContext * context)
        {
                ConstValue * value = this->Expr->Calculate(context);
		if(value==NULL)
                {
	                return NODE_RET_ERROR;
                }

		context->FunctionRet = value->DupValue();

		delete value;
                return NODE_RET_NEEDRETURN;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
	}
	bool Check()
	{
		return this->Expr->Check();
	}
private:
        Expression * Expr;

};

class RollbackStatement: public StatementNode
{
public:
	int Invoke(NodeContext * context)
	{
		return NODE_RET_NEEDROLLBACK;
	}
	bool Provision()
        {
                return true;
        }

};

class CommitStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NEEDCOMMIT;
        }
	bool Provision()
        {
                return true;
        }

};

class CallStatement: public StatementNode
{
public:
	CallStatement():RetVal(NULL){}

        int Invoke(NodeContext * context)
        {
                this->RetVal = this->Expr->Calculate(context);
		if(this->RetVal==NULL)
                {
	                return NODE_RET_ERROR;
                }

                return NODE_RET_NORMAL;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
        }
	bool Check()
        {
                return this->Expr->Check();
        }

	ConstValue * GetRetVal()
	{
		return this->RetVal;
	}
private:
	ConstValue * RetVal;
        Expression * Expr;
};

#endif

