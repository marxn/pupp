#ifndef _PUPPY_CLOSURE_H_
#define _PUPPY_CLOSURE_H_

#include <list>
#include "function.h"
#include "variable.h"

using namespace std;

class FuncValue: public ConstValue
{
public:
        FuncValue(FunctionNode * node, list<Variable*> * cv):ConstValue(Func), FuncNode(node), ClosureVars(cv)
        {
        }
        ~FuncValue()
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

        ConstValue * DupValue()
        {
                list<Variable*> * cvs = new list<Variable*>;
                list<Variable*>::iterator i;

                for(i = this->ClosureVars->begin(); i!= this->ClosureVars->end(); i++)
                {
                        Variable * cv = (*i)->CreateVarRef();
                        cvs->push_back(cv);
                }

                return new FuncValue(this->FuncNode, cvs);
        }
        FunctionNode * GetFuncNode()
        {
                return this->FuncNode;
        }
        list<Variable*> * GetClosureVars()
        {
                return this->ClosureVars;
        }
        string toString()
        {
                return "function";
        }
private:
        FunctionNode * FuncNode;
        list<Variable*> * ClosureVars;
};

#endif

