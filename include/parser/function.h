#ifndef _PUPP_FUNCTION_H_
#define _PUPP_FUNCTION_H_

#include "constval.h"
#include "container.h"

using namespace std;

class FuncArgDef
{
public:
        FuncArgDef(string name, DataType type, bool isref)
        {
                this->Name = name;
                this->Type = type;
                this->isref = isref;
        }
        bool isRef()
        {
                return this->isref;
        }
        string GetName()
        {
                return this->Name;
        }
        DataType GetType()
        {
                return this->Type;
        }
        void SetElementType(DataType type)
        {
                this->ElementType = type;
        }
        DataType GetElementType()
        {
                return this->ElementType;
        }
private:
        string Name;
        DataType Type;
        DataType ElementType;
        bool isref;
};

class ClosureVarDesc
{
public:
        ClosureVarDesc(string name, bool b):Name(name), isRef(b){}

        string GetVarName()
        {
            return this->Name;
        }
        bool IsRef()
        {
            return this->isRef;
        }
private:
        string Name;
        bool   isRef;
};

class FunctionNode:public ContainerNode
{
public:
        FunctionNode(string name):ContainerNode(Function)
        {
                this->Name = name;
        }
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NORMAL;
        }
        int Run(NodeContext * context)
        {
                return ContainerNode::Invoke(context);
        }
        
        void SetRtnType(DataType type)
        {
                this->RtnType = type;
        }
        void SetArgList(list<FuncArgDef*> * arglist)
        {
                this->ArgList = arglist;
        }
        list<FuncArgDef*> * GetArgList()
        {
                return this->ArgList;
        }
        void SetCopyList(list<ClosureVarDesc*> * copylist)
        {
                this->CopyList = copylist;
        }
        list<ClosureVarDesc*> * GetCopyList()
        {
                return this->CopyList;
        }
        string GetName()
        {
                return this->Name;
        }
        void SetName(string name)
        {
                this->Name = name;
        }
        DataType GetRtnType()
        {
                return this->RtnType;
        }
        bool Provision()
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

private:
        string Name;
        ConstValue * ReturnVal;
        DataType RtnType;
        list<FuncArgDef*> * ArgList;
        list<ClosureVarDesc*> * CopyList;
};

#endif

