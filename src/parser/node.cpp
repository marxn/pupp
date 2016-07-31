#include "parser/node.h"

using namespace std;

Node::Node()
{
        this->Type       = Generic;
        this->ParentNode = NULL;
}

Node::Node(NodeType type)
{
        this->Type       = type;
        this->ParentNode = NULL;
}

Node::~Node()
{
}

int Node::Execute(NodeContext * context)
{
        int ret = this->Invoke(context);
        this->Swipe(context);
        return ret;
}

Node * Node::GetParentNode()
{
        return this->ParentNode;
}

void Node::SetParentNode(Node * node)
{
        this->ParentNode = node;
}

bool Node::Check()
{
        return true;
}

void Node::AddVariable(VariableDef * var)
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

VariableDef * Node::FindVariable(string varname)
{
        map<string, VariableDef*>::iterator i = this->VariableDefTable.find(varname);
        if(i!=this->VariableDefTable.end())
        {
                return i->second;
        }

        if(this->ParentNode!=NULL)
        {
                VariableDef * result = this->ParentNode->FindVariable(varname);
                return result;
        }

        return NULL;
}
        
NodeContext::NodeContext()
{
        FunctionRet = 0;
}

NodeContext::~NodeContext()
{
        while(this->Frames.size()>0)
        {
                this->PopFrame();
        }
}

void NodeContext::AddFrame(Node * snapshot)
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

void NodeContext::PopFrame()
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

void NodeContext::AddVariableToCurrentFrame(Variable * var)
{
        map<string, Variable*> * frame = this->Frames.front();
        if(frame==NULL)
        {
                return;
        }

        map<string, Variable*>::iterator ex = frame->find(var->GetVarName());

        if(ex != frame->end())
        {
                delete ex->second;
                frame->erase(ex);
        }

        frame->insert(pair<string, Variable*>(var->GetVarName(), var));
}

Variable * NodeContext::GetVariable(string name)
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

Variable * NodeContext::GetVariableFromOuterLayer(string name)
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

