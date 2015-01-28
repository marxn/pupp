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
	void Swipe(NodeContext * context){}
};

class BreakStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
		return NODE_RET_NEEDBREAK;
        }
        bool Provision(ErrorStack * errstack)
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
        bool Provision(ErrorStack * errstack)
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
			var = this->VarDef->GetInstance();
			if(var==NULL)
			{
				cerr<<"Puppy runtime error: cannot find variable: "<<this->VarName<<endl;
				return NODE_RET_ERROR;
			}
		}

		ConstValue * value = Expr->Calculate(context);
		if(var->GetVarType()!=value->GetType() && var->GetVarType()!=Any)
		{
			cerr<<"Data type mismatch"<<endl;

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
        bool Provision(ErrorStack * errstack)
        {
		Node * parent = this->GetParentNode();

		this->Expr->SetParentNode(parent);
                if(this->Expr->Provision(errstack)==false)
		{
			errstack->PushFrame(0, "Provision expression falied.");
			return false;
		}
		return true;
        }
	bool Check(ErrorStack * errstack)
	{
		Node * parent = this->GetParentNode();
                VariableDef * vardef = parent->FindVariable(VarName);
                if(vardef==NULL)
                {
                        errstack->PushFrame(0, "Variable "+this->VarName+" not defined");
                        return false;
                }
		this->VarDef = vardef;
		return true;
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
		Variable * var = context->GetVariable(this->Var->GetVarName());

		if(var->GetValueType()!=Set && var->GetValueType()!=Any)
		{
			cerr<<"puppy warning: Cannot accept a non-collection variable."<<endl;
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

					if(need_clear)
                        		{
                	                	delete last_offset_value;
	        	                        need_clear = false;
		                        }

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
					ConstValue * target_value = Expr->Calculate(context);
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
	bool Provision(ErrorStack * errstack)
        {
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
	bool Check(ErrorStack * errstack)
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
                        if((*i)->Check(errstack)==false)
                        {
                                return false;
                        }
                }

                if(this->Expr->Check(errstack)==false)
                {
                        return false;
                }

                return true;

	}
private:
	CollectionElementRef * Reference;
        Expression * Expr;
	VariableDef * Var;
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
        bool Provision(ErrorStack * errstack)        
	{
                list<string*>::iterator i;
                Node * parent = this->GetParentNode();
                if(parent)
                {
                        for(i = IdentList->begin(); i != IdentList->end(); i++)
                        {
				if(parent->FindVariable(*(*i)))
				{
					errstack->PushFrame(0, "Duplicated variable: "+*(*i));
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
	                printf("%s",value->toString().c_str());
			fflush(stdout);
			
			delete value;
		}
		return NODE_RET_NORMAL;
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
	bool Check(ErrorStack * errstack)
        {
                list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        if((*i)->Check(errstack)==false)
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
		if(value->GetType()!=Integer)
                {
                        //TODO
			cerr<<"SLEEP Statement MUST have a Integer parameter "<<endl;
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
        bool Provision(ErrorStack * errstack)
        {
		this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision(errstack);
        }
	bool Check(ErrorStack * errstack)
        {
                return this->Expr->Check(errstack);
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
		context->FunctionRet = value->DupValue();

		delete value;
                return NODE_RET_NEEDRETURN;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision(ErrorStack * errstack)
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision(errstack);
	}
	bool Check(ErrorStack * errstack)
	{
		return this->Expr->Check(errstack);
	}
private:
        Expression * Expr;

};

class CallStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                this->RetVal = this->Expr->Calculate(context);
                return NODE_RET_NORMAL;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision(ErrorStack * errstack)
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision(errstack);
        }
	bool Check(ErrorStack * errstack)
        {
                return this->Expr->Check(errstack);
        }

	ConstValue * GetRetVal()
	{
		return this->RetVal;
	}
private:
	ConstValue * RetVal;
        Expression * Expr;
};

class ObjectStatement: public StatementNode
{
public:
	ObjectStatement(string * objname, string * method_name, list<Expression*> * expr_list): ObjName(*objname), ExprList(expr_list)
	{
		this->MethodName = *method_name;
	}
	int Invoke(NodeContext * context)
	{
		//this->FindMethodMapping(this->ObjName, this->MethodName);
		return NODE_RET_NORMAL;
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

		VariableDef * vardef = this->FindVariable(this->ObjName);
                if(vardef==NULL)
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

