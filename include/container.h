#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include "node.h"

using namespace std;

class ContainerNode :public Node
{
        public:
                ContainerNode():Node(Generic){}
                ContainerNode(NodeType type):Node(type){}

                int Invoke(NodeContext * context)
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                if((*i)->Type==Function)
                                {
                                        continue;
                                }

                                context->AddFrame(*i);
                                int cond = (*i)->Execute(context);
                                context->PopFrame();

                                if(cond!=NODE_RET_NORMAL)
                                {
                                        return cond;
                                }
                        }

                        return NODE_RET_NORMAL;
                }
                void Swipe(NodeContext * context){}
                void SetNodeList(list<Node*> * nodelist)
                {
                        subnodelist = nodelist;
                }
                bool Provision()
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Provision()!=true)
                                {
                                        return false;
                                }
                        }
                        return true;
                }
                bool Check()
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                if((*i)->Check()!=true)
                                {
                                        return false;
                                }
                        }
                        return true;
                }

                list<Node*> * subnodelist;
};

#endif
