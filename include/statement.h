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
        void Invoke()
        {
                Node * node = this->GetParentNode();
                if(node)
                {
                        node->SetNeedBreak(true);
                }
        }
        bool Provision(ErrorStack * errstack)
	{
		return true;
        }
};

class ContinueStatement: public StatementNode
{
public:
        void Invoke()
        {
                Node * node = this->GetParentNode();
                if(node)
                        node->SetNeedContinue(true);
        }
        bool Provision(ErrorStack * errstack)
        {
		return true;
        }
};

class AssignStatement: public StatementNode
{
public:
        void Invoke()
        {
		ConstValue * value = Expr->Calculate();
		this->Var->SetValue(value);
		delete value;
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
		
		if(this->Expr->GetDataType()!=this->Var->GetType())
		{
			errstack->PushFrame(0,"Datatype mismatch for "+this->VarName);
			return false;
		}
		return true;
        }
private:
        string VarName;
        Variable * Var;
        Expression * Expr;
};

class SetElementAssignStatement: public StatementNode
{
public:
	void Invoke()
        {
                ConstValue * offset_value = OffsetExpr->Calculate();
		ConstValue * target_value = Expr->Calculate();

		SetValue * value = static_cast<SetValue*>(this->Var->GetReference());
		KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(offset_value, target_value));
		value->AddKV(kv);

		delete kv;
                delete target_value;
		delete offset_value;
        }
	void SetVariableName(string name)
        {
                this->VarName = name;
        }
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
	void SetOffsetExpr(Expression * expr)
	{
		this->OffsetExpr = expr;
	}
	bool Provision(ErrorStack * errstack)
        {
		this->Var = this->FindVariable(VarName);
                if(this->Var==NULL)
                {
                        errstack->PushFrame(0, "Variable "+this->VarName+" not defined");
                        return false;
                }

                this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision(errstack)==false)
                {
                        return false;
                }

		this->OffsetExpr->SetParentNode(this->GetParentNode());
		if(this->OffsetExpr->Provision(errstack)==false)
		{
			return false;
		}

                return true;
        }
private:
	string VarName;
	Variable * Var;
        Expression * Expr;
	Expression * OffsetExpr;
};

class VarDefinitionStatement: public StatementNode
{
public:
	VarDefinitionStatement(list<Identifier*> * list, DataType vartype):IdentList(list), VarType(vartype)
	{
	}
        void Invoke()
        {
        }
        bool Provision(ErrorStack * errstack)        
	{
                list<Identifier*>::iterator i;
                Node * parent = GetParentNode();

                if(parent)
                {
                        for(i = IdentList->begin(); i != IdentList->end(); i++)
                        {
				if(parent->FindVariable((*i)->GetName()))
				{
					errstack->PushFrame(0, "Duplicated variable: "+(*i)->GetName());
        		                return false;
				}

                                Variable * var = new Variable((*i)->GetName());
				var->SetType(this->VarType);

				switch(this->VarType)
				{
					case Integer:
					var->SetValue(new IntegerValue(0));
					break;
					case Float:
					var->SetValue(new FloatValue(0.0f));
					break;
					case String:
					var->SetValue(new StringValue(""));
					break;
					case Boolean:
					var->SetValue(new BooleanValue(false));
					break;
					case Set:
					var->SetValue(new SetValue());
					break;
				}
                                parent->AddVariable(var);
                        }
                }
		return true;
        }
private:
        list<Identifier*> * IdentList;
        DataType VarType;
};

class PrintStatement: public StatementNode
{
public:
        void Invoke()
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			ConstValue * value = (*i)->Calculate();
	                printf("%s",value->toString().c_str());
			fflush(stdout);
			
			delete value;
		}
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
				//errstack->PushFrame(0, "PRINT Statement failed in transforming expressions ");
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
        void Invoke()
        {
		ConstValue * value = this->Expr->Calculate();
		usleep(static_cast<IntegerValue*>(value)->GetValue());

		delete value;
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
                        //errstack->PushFrame(0, "SLEEP Statement failed in transforming expression ");
                        return false;
                }
		if(this->Expr->GetDataType()!=Integer)
		{
			errstack->PushFrame(0, "SLEEP Statement MUST have a Integer parameter ");
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
	ObjectStatement(string * objname, Identifier * method_name, list<Expression*> * expr_list): ObjName(*objname), ExprList(expr_list)
	{
		this->MethodName = method_name->GetName();
	}
	void Invoke()
	{
		//this->FindMethodMapping(this->ObjName, this->MethodName);
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

