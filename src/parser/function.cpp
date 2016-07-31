#include "parser/function.h"

using namespace std;

FuncArgDef::FuncArgDef(string name, DataType type, bool isref)
{
        this->Name = name;
        this->Type = type;
        this->isref = isref;
}

bool FuncArgDef::isRef()
{
        return this->isref;
}

string FuncArgDef::GetName()
{
        return this->Name;
}

DataType FuncArgDef::GetType()
{
        return this->Type;
}

void FuncArgDef::SetElementType(DataType type)
{
        this->ElementType = type;
}

DataType FuncArgDef::GetElementType()
{
        return this->ElementType;
}


ClosureVarDesc::ClosureVarDesc(string name, bool b)
{
        this->Name  = name;
        this->isRef = b;
}

string ClosureVarDesc::GetVarName()
{
    return this->Name;
}

bool ClosureVarDesc::IsRef()
{
    return this->isRef;
}


FunctionNode::FunctionNode(string name):ContainerNode(Function)
{
        this->Name = name;
}

int FunctionNode::Invoke(NodeContext * context)
{
        return NODE_RET_NORMAL;
}

int FunctionNode::Run(NodeContext * context)
{
        return ContainerNode::Invoke(context);
}

void FunctionNode::SetRtnType(DataType type)
{
        this->RtnType = type;
}

void FunctionNode::SetArgList(list<FuncArgDef*> * arglist)
{
        this->ArgList = arglist;
}

list<FuncArgDef*> * FunctionNode::GetArgList()
{
        return this->ArgList;
}

void FunctionNode::SetCopyList(list<ClosureVarDesc*> * copylist)
{
        this->CopyList = copylist;
}

list<ClosureVarDesc*> * FunctionNode::GetCopyList()
{
        return this->CopyList;
}

string FunctionNode::GetName()
{
        return this->Name;
}

void FunctionNode::SetName(string name)
{
        this->Name = name;
}

DataType FunctionNode::GetRtnType()
{
        return this->RtnType;
}

bool FunctionNode::Provision()
{
        list<FuncArgDef*>::iterator i;
        for(i = this->ArgList->begin(); i!=this->ArgList->end(); i++)
        {
                string argname = (*i)->GetName();
                DataType argtype = (*i)->GetType();
                VariableDef * localvar = new VariableDef(argname);
                localvar->SetVarType(argtype);
                this->AddVariable(localvar);
        }

        Node * parent = this->GetParentNode();
        if(parent==NULL)
        {
                cerr<<"pupp provision error: A function cannot be defined without a context."<<endl;
                return false;
        }
/*
        list<ClosureVarDesc*>::iterator j;

        if(this->CopyList!=NULL)
        {
            for(j = this->CopyList->begin(); j!= this->CopyList->end(); j++)
            {
                    string varname = (*j)->GetVarName();
                    VariableDef * vardef = parent->FindVariable(varname);
                    if(vardef==NULL)
                    {
                            cerr<<"pupp provision error: Variable "<<varname<<" has not been defined"<<endl;
                            return false;
                    }
            }
        }
*/
        return ContainerNode::Provision();
}


