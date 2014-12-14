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
        bool Transform(ErrorStack * errstack)
	{
		return true;
        }
	void Swipe(){}
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
        bool Transform(ErrorStack * errstack)
        {
		return true;
        }
	void Swipe(){}
};

class AssignStatement: public StatementNode
{
public:
        void Invoke()
        {
                if(this->Var)
                {
			ConstValue * value = this->Var->GetValue();
			if(value)
			{
				delete value;
			}

                        this->Var->SetValue(Expr->GetValue()->DupValue());
                }
        }
	void Swipe()
	{
		this->Expr->Swipe();
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
        bool Transform(ErrorStack * errstack)
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
                if(this->Expr->Transform(errstack)==false)
		{
			errstack->PushFrame(0, "Transform expression falied.");
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

class VarDefinitionStatement: public StatementNode
{
public:
	VarDefinitionStatement(list<Identifier*> * list, DataType vartype):IdentList(list), VarType(vartype)
	{
	}
        void Invoke()
        {
        }
        bool Transform(ErrorStack * errstack)        
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
	void Swipe(){}
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
	                printf("%s",(*i)->GetValue()->toString().c_str());
			fflush(stdout);
		}
        }
	void Swipe()
	{
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			(*i)->Swipe();
                }
	}
        void SetExpressionList(list<Expression*> * exprlist)
        {
                this->ExprList = exprlist;
        }
        bool Transform(ErrorStack * errstack)
        {
		list<Expression*>::iterator i;
		for(i = ExprList->begin(); i!= ExprList->end(); i++)
		{
			(*i)->SetParentNode(this->GetParentNode());
                	if((*i)->Transform(errstack)==false)
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
		IntegerValue * value = static_cast<IntegerValue*>(this->Expr->GetValue());
		usleep(value->GetValue());
        }
	void Swipe()
	{
		this->Expr->Swipe();
	}
	void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Transform(ErrorStack * errstack)
        {
		this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Transform(errstack)==false)
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

#endif

