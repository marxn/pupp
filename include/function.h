#ifndef _PUPPY_FUNCTION_H_
#define _PUPPY_FUNCTION_H_

#include "constval.h"
#include "container.h"

using namespace std;

class FuncArgDef
{
public:
        FuncArgDef(string * name, DataType type, bool isref)
        {
                this->Name = *name;
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
private:
        string Name;
        DataType Type;
	bool isref;
};

class FunctionNode:public ContainerNode
{
public:
        FunctionNode(string * name):ContainerNode(Function)
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
                        cerr<<"puppy provision error: A function cannot be defined without a context."<<endl;
                        return false;
                }

                parent->AddFunctionDef(this->Name, this);

		return ContainerNode::Provision();
        }

private:
        string Name;
	ConstValue * ReturnVal;
        DataType RtnType;
        list<FuncArgDef*> * ArgList;
};

#endif

