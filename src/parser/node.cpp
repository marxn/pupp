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
        var->SetVarIndex(this->VarDefArray.size());
        
        this->VariableDefTable[var->GetVarName()] = var;
        this->VarDefArray.push_back(var);
}

VariableDef * Node::FindVariable(string varname, unsigned long * layer)
{
        map<string, VariableDef*>::iterator i = this->VariableDefTable.find(varname);
        if(i!=this->VariableDefTable.end())
        {
                return i->second;
        }

        if(this->ParentNode!=NULL)
        {
                if(layer)
                {
                        (*layer)++;
                }
                
                VariableDef * result = this->ParentNode->FindVariable(varname, layer);
                return result;
        }

        return NULL;
}
        
NodeContext::NodeContext()
{
        FunctionRet = 0;
        this->VarTable.reserve(64);
}

NodeContext::~NodeContext()
{
        while(this->VarTable.size()>0)
        {
                this->PopFrame();
        }
}

void NodeContext::AddFrame(Node * snapshot)
{
        vector<Variable*> frame;

        vector<VariableDef*>::iterator i;
        for(i = snapshot->VarDefArray.begin(); i!=snapshot->VarDefArray.end(); i++)
        {
                VariableDef * def = *i;
                if((*i)->NeedInstance())
                {
                        Variable * var = def->GetInstance();
                        frame.push_back(var);
                }
                else
                {
                        frame.push_back(NULL);
                }
        }
        
        this->VarTable.push_back(frame);
        
}

void NodeContext::PopFrame()
{
        if(this->VarTable.size()<=0)
        {
                return;
        }
        
        vector<Variable*>& frame = this->VarTable.back();
        vector<Variable*>::iterator i;
        	
        for(i = frame.begin(); i!=frame.end(); i++)
        {
                delete *i;
        }

        this->VarTable.pop_back();
}

void NodeContext::ReplaceVariable(unsigned long start_index, list<Variable*> * closurevars)
{
        int i = 0;
        vector<Variable*>& frame = this->VarTable[this->VarTable.size() - 1];
        
        list<Variable*>::iterator closurevarindex;
                
        for(closurevarindex = closurevars->begin(); closurevarindex != closurevars->end(); closurevarindex++)
        {
                frame[start_index + i] = (*closurevarindex)->CreateVarRef();
                i++;
        }
}

Variable * NodeContext::GetVariable(unsigned long layer, unsigned long index)
{
        if(layer > this->VarTable.size() || index > this->VarTable[this->VarTable.size() - layer - 1].size())
        {
                return NULL;
        }
        
        return this->VarTable[this->VarTable.size() - layer - 1][index];
}


