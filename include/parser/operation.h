#ifndef _PUPP_OPERATION_H_
#define _PUPP_OPERATION_H_

#include "constval.h"

class Operation
{
public:
        static ConstValue * AddOperation(ConstValue * left, ConstValue * right);

        static ConstValue * SubOperation(ConstValue * left, ConstValue * right);

        static ConstValue * MulOperation(ConstValue * left, ConstValue * right);

        static ConstValue * DivOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * GTOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * LTOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * EQOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * NEQOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * GEOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * LEOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * ANDOperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * OROperation(ConstValue * left, ConstValue * right);
        
        static ConstValue * NOTOperation(ConstValue * value);
};

#endif
