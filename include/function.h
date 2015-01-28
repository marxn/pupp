#ifndef _PUPPY_FUNCTION_H_
#define _PUPPY_FUNCTION_H_

#include "constval.h"
#include "container.h"

using namespace std;

class FuncArgDef
{
public:
        FuncArgDef(string * name, DataType type)
        {
                this->Name = *name;
                this->Type = type;
        }
        string GetName()
        {
                return this->Name;
        }
        DataType GetType()
        {
                return this->Type;
        }
private:
        string Name;
        DataType Type;
};

class FunctionNode:public ContainerNode
{
public:
        FunctionNode(string * name)
        {
                this->Name = *name;
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
        string GetName()
        {
                return this->Name;
        }
        bool Provision(ErrorStack * errstack)
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
                        errstack->PushFrame(0, "Function cannot be defined without container.");
                        return false;
                }
                parent->AddFunctionDef(this->Name, this);

		if(ContainerNode::Provision(errstack)==false)
                {
                        errstack->PushFrame(0, "Failed to transform container.");
                        return false;
                }

                return true;
        }

private:
        string Name;
	ConstValue * ReturnVal;
        DataType RtnType;
        list<FuncArgDef*> * ArgList;
};

#endif

