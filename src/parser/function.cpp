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

void FuncArgDef::SetArgIndex(unsigned long index)
{
        this->ArgIndex = index;
}

unsigned long FuncArgDef::GetArgIndex()
{
        return this->ArgIndex;
}

ClosureVarDesc::ClosureVarDesc(string name, bool b)
{
        this->Name  = name;
        this->isRef = b;
        this->SrcLayer = 0;
        this->SrcDef   = NULL;
        this->DstDef   = NULL;
}

string ClosureVarDesc::GetVarName()
{
    return this->Name;
}

bool ClosureVarDesc::IsRef()
{
    return this->isRef;
}

void ClosureVarDesc::SetSrcLayer(unsigned long layer)
{
        this->SrcLayer = layer;
}

void ClosureVarDesc::SetSrcDef(VariableDef * def)
{
        this->SrcDef = def;
}

void ClosureVarDesc::SetDstDef(VariableDef * def)
{
        this->DstDef = def;
}
                                      
unsigned long ClosureVarDesc::GetSrcLayer()
{
        return this->SrcLayer;
}

VariableDef * ClosureVarDesc::GetSrcDef()
{
        return this->SrcDef;
}

VariableDef * ClosureVarDesc::GetDstDef()
{
        return this->DstDef;
}

FunctionNode::FunctionNode(string name):ContainerNode(Function)
{
        this->Name = name;
        this->ClosureVarStartIndex = 0;
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
                
                unsigned long varindex = localvar->GetVarIndex();
                (*i)->SetArgIndex(varindex);
        }

        Node * parent = this->GetParentNode();
        if(parent==NULL)
        {
                cerr<<"pupp provision error: A function cannot be defined without a context."<<endl;
                return false;
        }

        list<ClosureVarDesc*>::iterator j;

        if(this->CopyList!=NULL)
        {
                int pos = 0;
                
                for(j = this->CopyList->begin(); j!= this->CopyList->end(); j++)
                {
                        //Because its parent is NOT a Node but a lambda exp, the layer has to be set to 0 to skip the exp.
                        //This is extremely unusual.
                        unsigned long layer = 0; 
                        
                        string varname = (*j)->GetVarName();
                        VariableDef * vardef = parent->FindVariable(varname, &layer);
                        if(vardef==NULL)
                        {
                                cerr<<"pupp provision error: Variable "<<varname<<" has not been defined"<<endl;
                                return false;
                        }
                        
                        VariableDef * newvardef = new VariableDef(varname);
                        newvardef->SetVarType(vardef->GetVarType());
                        newvardef->SetNeedInstance(false);

                        this->AddVariable(newvardef);
                        
                        (*j)->SetSrcLayer(layer);
                        (*j)->SetSrcDef(vardef);
                        (*j)->SetDstDef(newvardef);
                        
                        if(pos++==0)
                        {
                                this->ClosureVarStartIndex = newvardef->GetVarIndex();
                        }
                        
                }
        }
        
        return ContainerNode::Provision();
}

unsigned long FunctionNode::GetClosureVarStartIndex()
{
        return this->ClosureVarStartIndex;
}
