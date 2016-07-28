#ifndef _PUPP_CLOSURE_H_
#define _PUPP_CLOSURE_H_

#include <list>
#include "function.h"
#include "variable.h"

class FuncValue: public ConstValue
{
public:
        FuncValue(FunctionNode * node, std::list<Variable*> * cv);
        ~FuncValue();

        ConstValue * DupValue();
        FunctionNode * GetFuncNode();
        std::list<Variable*> * GetClosureVars();
        std::string toString();
private:
        FunctionNode * FuncNode;
        std::list<Variable*> * ClosureVars;
};

#endif

