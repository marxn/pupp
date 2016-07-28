#ifndef _PUPP_TRANSACTION_H_
#define _PUPP_TRANSACTION_H_

#include <list>
#include "container.h"

class TransNode: public ContainerNode
{
public:
        TransNode();

        int Invoke(NodeContext * context);

        void SetIdentList(std::list<std::string*> * idlist);

        bool Provision();

private:
        std::list<std::string*> * IdList;
        std::list<VariableDef*> VarDefList;
};

#endif
