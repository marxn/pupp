#ifndef _PUPP_IFELSE_H_
#define _PUPP_IFELSE_H_

#include <list>
#include "expression.h"
#include "container.h"

class BranchNode :public ContainerNode
{
public:
        BranchNode();
        int Invoke(NodeContext * context);
        void SetIfNode(Node * node);
        void SetElseNode(Node * node);

        void SetCondition(Expression * condition);
        bool Provision();
        bool Check();

private:
        int Evaluate(NodeContext * context);

        Expression * condition;
        Node * ifnode;
        Node * elsenode;
};

#endif
