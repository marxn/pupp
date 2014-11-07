#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include "variable.h"
#include "expression.h"
#include "nodecontext.h"

using namespace std;


class Node
{
	public:
		Node():PrevNode(NULL){}
		virtual void Invoke() = 0;
		virtual void TransformAll() = 0;

		void SetPrevNode(Node * node)
		{
			this->PrevNode = node;
			this->Context.SetPrevNodeContext(&(node->Context));
		}

		Node * GetPrevNode()
		{
			return this->PrevNode;
		}
		
		NodeContext Context;
	protected:
		Node * PrevNode;
};

class SimpleNode :public Node
{
	public:
		void Invoke()
		{
		}
		void TransformAll()
		{
		}
};

class BreakStatement: public SimpleNode
{
public:
	void Invoke()
	{
		Node * prevnode = this->GetPrevNode();
		if(prevnode)
			prevnode->Context.SetNeedBreak(true);
	}
	void TransformAll()
	{
	}
};

class ContinueStatement: public SimpleNode
{
public:
	void Invoke()
	{
		Node * prevnode = this->GetPrevNode();
		if(prevnode)
			prevnode->Context.SetNeedContinue(true);
	}
	void TransformAll()
	{
	}
};

class AssignStatement: public SimpleNode
{
public:
	void Invoke()
	{
		Node * prevnode = GetPrevNode();
		if(prevnode)
		{
			//cout<<this->VarName<<"="<<this->Expr->GetValue().toString()<<endl;
			Variable * var = this->Context.FindVariable(VarName);//prevnode->Context.GetVariable(VarName);
			if(var)
				var->SetValue(Expr->GetValue());
			else
				cout<<"variable "<<this->VarName<<" not defined"<<endl;
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
		this->Expr->SetRelatedContext(&(this->GetPrevNode()->Context));
		this->Expr->TransformExpr();
	}
private:
	string VarName;
	Expression * Expr;
};

class VarDefinitionStatement: public SimpleNode
{
public:
	void Invoke()
	{
		list<Identifier*>::iterator i;
		Node * prevnode = GetPrevNode();

		if(prevnode)
		{
			for(i = IdentList->begin(); i != IdentList->end(); i++)  
			{
				Variable * var = new Variable((*i)->GetName());
				switch(this->VarType)
				{
				case ConstValue::Integer:
					var->SetValue(0);
					break;
				case ConstValue::Float:
					var->SetValue(0.0f);
					break;
				case ConstValue::String:
					var->SetValue("");
					break;
				case ConstValue::Boolean:
					var->SetValue(false);
					break;
				}
				prevnode->Context.AddVariable(var);
			}
		}
	}
	void SetVarList(list<Identifier*> * list)
	{
		this->IdentList = list;
	}
	void SetVarType(ConstValue::ValueType vartype)
	{
		this->VarType = vartype;
	}
	void TransformAll()
	{
	}
private:
	list<Identifier*> * IdentList;
	string VarName;
	ConstValue::ValueType VarType;
};

class PrintStatement: public SimpleNode
{
public:
	void Invoke()
	{
		cout<<this->Expr->GetValue().toString();
	}
	void SetExpression(Expression * expr)
	{
		this->Expr = expr;
	}
	void TransformAll()
	{
		this->Expr->SetRelatedContext(&(this->GetPrevNode()->Context));
		this->Expr->TransformExpr();
	}
private:
	Expression * Expr;
};

class ContainerNode :public Node
{
	public:
		void Invoke()
		{
			list<Node*>::iterator i;
  			for(i = subnodelist->begin(); i != subnodelist->end(); i++)  
  				(*i)->Invoke();
		}
		void SetNodeList(list<Node*> * nodelist)
		{
			subnodelist = nodelist;
		}
		void TransformAll()
		{
			list<Node*>::iterator i;
			for(i = subnodelist->begin(); i != subnodelist->end(); i++)  
			{
				(*i)->SetPrevNode(this);
				(*i)->TransformAll();
			}
		}
	protected:
		list<Node*> * subnodelist;
};

class LoopNode :public ContainerNode
{
	public:
		void Invoke()
		{
			bool localctl = true;
			while(localctl && Evaluate())
			{
				this->Context.SetNeedBreak(false);
				this->Context.SetNeedContinue(false);

				list<Node*>::iterator i;

  				for(i = subnodelist->begin(); i != subnodelist->end(); i++)  
				{
  					(*i)->Invoke();
					 if(this->Context.GetNeedBreak())
					 {
						 localctl = false;
						 break;
					 }
					 if(this->Context.GetNeedContinue())
						 break;
				}
  			}
		}
		void SetCondition(Expression * condition)
		{
			this->condition = condition;
		}
		void TransformAll()
		{
			ContainerNode::TransformAll();
			condition->SetRelatedContext(&(this->GetPrevNode()->Context));
			condition->TransformExpr();
		}
	private:
		bool Evaluate()
		{
			ConstValue temp = condition->GetValue();
			if(temp.GetType() == ConstValue::Boolean)
				return temp.GetBoolean();
			
			return false;
		}
		Expression * condition;
};


class BranchNode :public ContainerNode
{
	public:
		void Invoke()
		{
			if(Evaluate())
			{
				list<Node*>::iterator i;
  				for(i = subnodelist->begin(); i != subnodelist->end(); i++)  
  					(*i)->Invoke();
			}
		}
		void SetCondition(Expression * condition)
		{
			this->condition = condition;
		}
		void TransformAll()
		{
			ContainerNode::TransformAll();
			condition->SetRelatedContext(&(this->GetPrevNode()->Context));
			condition->TransformExpr();
		}
	private:
		bool Evaluate()
		{
			ConstValue temp = condition->GetValue();
			if(temp.GetType() == ConstValue::Boolean)
				return temp.GetBoolean();
			
			return false;
		}
		Expression * condition;
};


//int main()
//{
//	/*
//	BranchNode node;
//	node.SetCondition(true);
//	
//	node.AddNode(new SimpleNode);
//	node.AddNode(new SimpleNode);
//	*/
//	//Variable * var = new Variable("name");
//	string varname = "name";
//	ConstValue * val = new ConstValue(10);
//	FinalExpression * exp = new FinalExpression(val);
//
//	LoopNode node;
//
//	node.SetCondition(NULL);
//	
//	AssignStatement * p = new AssignStatement;
//	p->SetPrevNode(&node);
//	p->SetVariableName(varname);
//	p->SetExpression(exp);
//
//	BreakStatement * breaks = new BreakStatement;
//	breaks->SetPrevNode(&node);
//
//	list<Node*> * nodelist = new list<Node*>;
//
//	nodelist->push_back(p);
//	nodelist->push_back(breaks);
//
//	node.SetNodeList(nodelist);
//	
//	node.Invoke();
//	getchar();
//	return 0;
//}


#endif
