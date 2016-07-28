#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include "node.h"

class ContainerNode :public Node
{
        public:
                ContainerNode();
                ContainerNode(NodeType type);

                int Invoke(NodeContext * context);
                void Swipe(NodeContext * context);
                void SetNodeList(std::list<Node*> * nodelist);

                void AddNode(Node * node);

                bool Provision();
                bool Check();

                std::list<Node*> * subnodelist;
};

#endif
