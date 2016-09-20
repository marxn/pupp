#include "parser/closure.h"

using namespace std;

FuncValue::FuncValue(FunctionNode * node, list<Variable*> * cv):ConstValue(Func)
{
        this->FuncNode    = node;
        this->ClosureVars = cv;
}

FuncValue::~FuncValue()
{
        list<Variable*>::iterator i;
        if(this->ClosureVars!=NULL)
        {
                for(i = this->ClosureVars->begin(); i!=this->ClosureVars->end(); i++)
                {
                        delete *i;
                }
                delete this->ClosureVars;
        }
}

ConstValue * FuncValue::DupValue()
{
        if(this->FuncNode==NULL)
        {
                return new FuncValue(NULL, NULL);
        }
        
        list<Variable*> * cvs = new list<Variable*>;
        list<Variable*>::iterator i;

        for(i = this->ClosureVars->begin(); i!= this->ClosureVars->end(); i++)
        {
                Variable * cv = (*i)->CreateVarRef();
                cvs->push_back(cv);
        }

        return new FuncValue(this->FuncNode, cvs);
}

FunctionNode * FuncValue::GetFuncNode()
{
        return this->FuncNode;
}

list<Variable*> * FuncValue::GetClosureVars()
{
        return this->ClosureVars;
}

string FuncValue::toString()
{
        return "function";
}

