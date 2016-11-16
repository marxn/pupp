#ifndef _PUPP_TRANSACTION_H_
#define _PUPP_TRANSACTION_H_

#include <list>
#include "container.h"

struct TransIdentDesc
{
        VariableDef * SrcVardef;
        VariableDef * DstVarDef;
        unsigned long SrcLayer;
};

class TransNode: public ContainerNode
{
public:
        TransNode();

        int Invoke(NodeContext * context);

        void SetIdentList(std::list<std::string*> * idlist);

        bool Provision();
        bool Check();
private:
        std::list<std::string*> * IdList;
        std::list<TransIdentDesc> VarDefList;
};

#endif
