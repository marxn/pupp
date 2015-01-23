#ifndef _NODE_H_
#define _NODE_H_

#include <stack>
#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include "puppybase.h"
#include "variable.h"
#include "errstack.h"

using namespace std;

#define NODE_RET_NORMAL       0
#define NODE_RET_ERROR       -1
#define NODE_RET_NEEDBREAK    1
#define NODE_RET_NEEDCONTINUE 2
#define NODE_RET_NEEDRETURN   3

#define EVA_TRUE 1
#define EVA_FALSE 0
#define EVA_ERROR -1

class NodeContext;

class Node: public PuppyObject
{
public:
	Node()
	{
	}
	virtual ~Node()
	{
	}
	void display()
        {
                cerr<<"size="<<this->VariableDefTable.size();
                map<string, VariableDef*>::iterator i;
                for(i=this->VariableDefTable.begin(); i!=this->VariableDefTable.end();i++)
                {
                        cerr<<" displaying:"<<i->first<<" var="<<i->second<<endl;
                }
        }

	int Execute(NodeContext * context)
	{
		int ret = this->Invoke(context);
		this->Swipe(context);
		return ret;
	}

	Node * GetParentNode()
	{
		return this->ParentNode;
	}
	void SetParentNode(Node * node)
	{
		this->ParentNode = node;
	}
	virtual bool Provision(ErrorStack * errstack) = 0;
	virtual int Invoke(NodeContext * context) = 0;
	virtual void Swipe(NodeContext * context) = 0;

	void AddVariable(VariableDef * var)
        {
                VariableDef * thevar = this->VariableDefTable[var->GetVarName()];
                if(thevar != NULL)
                {
                        delete thevar;
                        this->VariableDefTable.erase(var->GetVarName());
                }
		this->VariableDefTable[var->GetVarName()] = var;
        }
        VariableDef * FindVariable(string varname)
        {
		map<string, VariableDef*>::iterator i = this->VariableDefTable.find(varname);
		if(i!=this->VariableDefTable.end())
		{
			return i->second;
		}
		if(this->ParentNode!=NULL)
		{
			return this->ParentNode->FindVariable(varname);
		}

                return NULL;
        }
	
        void AddFunctionDef(string name, Node * func)
        {
                this->FunctionDefTable.insert(pair<string, Node*>(name, func));
        }
        Node * FindFunctionDef(string name)
        {
                Node * result = this->FunctionDefTable[name];
                if(result == NULL && this->ParentNode!=NULL)
                {
                        return ParentNode->FindFunctionDef(name);
                }

                return result;
        }

	Node * ParentNode;
	map<string, VariableDef*> VariableDefTable;
	map<string, Node*> FunctionDefTable;
};


struct ForeachLoopCtx 
{
	ConstValue * Keeper;
	map<string, ConstValue*> * SetValueHolder;
	map<string, ConstValue*>::iterator ValueHandle;
};

class NodeContext
{
public:
	NodeContext(){}
	~NodeContext()
	{
		while(this->Frames.size()>0)
		{
			this->PopFrame();
		}
	}
	void AddFrame(Node * snapshot)
	{
		map<string, Variable*> * frame = new map<string, Variable*>;

		map<string, VariableDef*>::iterator i;
                for(i = snapshot->VariableDefTable.begin(); i!=snapshot->VariableDefTable.end(); i++)
                {
                        string name = i->first;
                        VariableDef * def = i->second;
                        frame->insert(pair<string, Variable*>(i->first, i->second->GetInstance()));
                }
		this->Frames.push_front(frame);
	}
	void PopFrame()
	{
		map<string, Variable*> * frame = this->Frames.front();
		map<string, Variable*>::iterator i;
                for(i = frame->begin(); i!=frame->end(); i++)
                {
                        delete i->second;
                }
		delete frame;
                this->Frames.erase(this->Frames.begin());
	}
        Variable * GetVariable(string name)
        {
		list<map<string, Variable*>* >::iterator i;
		for(i=this->Frames.begin(); i!=this->Frames.end();i++)
		{
			map<string, Variable*>::iterator j = (*i)->find(name);
			if(j!=(*i)->end())
			{
				return j->second;
			}
		}
                return NULL;
        }

        stack<ForeachLoopCtx*> ForeachCtx;
        ConstValue * FunctionRet;

private:
        list<map<string, Variable*>* > Frames;
};

#endif
