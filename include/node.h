#ifndef _NODE_H_
#define _NODE_H_

#include <stack>
#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include "variable.h"

using namespace std;

#define NODE_RET_NORMAL       0
#define NODE_RET_ERROR       -1
#define NODE_RET_NEEDBREAK    1
#define NODE_RET_NEEDCONTINUE 2
#define NODE_RET_NEEDRETURN   3

#define NODE_RET_NEEDROLLBACK 4
#define NODE_RET_NEEDCOMMIT   5

#define EVA_TRUE 1
#define EVA_FALSE 0
#define EVA_ERROR -1

class NodeContext;

enum NodeType
{
        Generic,
        Function,
        IfElse,
        Loop,
        Transaction
};

class Node
{
public:
        Node():Type(Generic), ParentNode(NULL)
        {
        }
        Node(NodeType type):Type(type), ParentNode(NULL)
        {
        }
        virtual ~Node()
        {
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
        virtual bool Provision() = 0;
        virtual bool Check()
        {
                return true;
        }

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
                var->SetAttachedNode(this);
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
                        VariableDef * result = this->ParentNode->FindVariable(varname);

                        if(result!=NULL && this->Type==Function)
                        {
                                result->SetUsedByInnerNode(true);
                        }
                        return result;
                }

                return NULL;
        }
        
        NodeType Type;
        Node * ParentNode;
        map<string, VariableDef*> VariableDefTable;
};


struct ForeachLoopCtx 
{
        ConstValue * Keeper;
        map<string, ValueBox*> * SetValueHolder;
        map<string, ValueBox*>::iterator ValueHandle;
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
                        Variable * var = def->GetInstance();
                        frame->insert(pair<string, Variable*>(name, var));
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
        void AddVariableToCurrentFrame(Variable * var)
        {
                map<string, Variable*> * frame = this->Frames.front();
                if(frame!=NULL && frame->find(var->GetVarName())==frame->end())
                {
                        frame->insert(pair<string, Variable*>(var->GetVarName(), var));
                }
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
        Variable * GetVariableFromOuterLayer(string name)
        {
                int count = 0;
                list<map<string, Variable*>* >::iterator i;
                for(i=this->Frames.begin(); i!=this->Frames.end();i++)
                {
                        if(count++==0)
                        {
                                continue;
                        }
                        map<string, Variable*>::iterator j = (*i)->find(name);
                        if(j!=(*i)->end())
                        {
                                return j->second;
                        }
                }

                return NULL;
        }

        list<Variable*> * BuildClosureVars()
        {
                list<Variable*> * ret = new list<Variable*>;

                list<map<string, Variable*>* >::iterator i;
                for(i=this->Frames.begin(); i!=this->Frames.end();i++)
                {
                        map<string, Variable*>::iterator j;
                        for(j = (*i)->begin(); j!=(*i)->end(); j++)
                        {
                                VariableDef * vardef = j->second->GetSource();
                                if(vardef->UsedByInnerNode())// && vardef->ActualParameter())
                                {
                                        //Put all the references of variables into the packet.
                                        Variable * origin = j->second;
                                        Variable * closure_var = origin->CreateVarRef();
                                        ret->push_back(closure_var);
                                }
                        }
                }
                return ret;
        }

        stack<ForeachLoopCtx*> ForeachCtx;
        ConstValue * FunctionRet;

private:
        list<map<string, Variable*>* > Frames;
};

#endif
