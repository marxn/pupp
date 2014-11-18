#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <iostream>
#include "node.h"
#include "expression.h"

using namespace std;

class StatementNode :public Node
{
public:
	StatementNode(){}
	~StatementNode(){}
        void Invoke()
        {
        }
        void TransformAll()
        {
        }
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
        void TransformAll()
        {
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
        void TransformAll()
        {
        }
};

class AssignStatement: public StatementNode
{
public:
        void Invoke()
        {
                if(this->Var)
                {
                        this->Var->SetValue(Expr->GetValue());
                }
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
        void TransformAll()
        {
                this->Var = this->FindVariable(VarName);
                if(this->Var==NULL)
                {
                        cout<<"variable "<<this->VarName<<" not defined"<<endl;
                        return;
                }
                if(this->Expr==NULL)
                {
                        cout<<"Illeagle Expression"<<endl;
                }

		this->Expr->SetParentNode(this->GetParentNode());
                this->Expr->TransformExpr();
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
        void TransformAll()
        {
                list<Identifier*>::iterator i;
                Node * parent = GetParentNode();

                if(parent)
                {
                        for(i = IdentList->begin(); i != IdentList->end(); i++)
                        {
                                Variable * var = new Variable((*i)->GetName());
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
				}
                                parent->AddVariable(var);
                        }
                }
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
                cout<<this->Expr->GetValue()->toString();
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        void TransformAll()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                this->Expr->TransformExpr();
        }
private:
        Expression * Expr;
};

#endif

