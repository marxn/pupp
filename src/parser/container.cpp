#include "parser/container.h"

using namespace std;

ContainerNode::ContainerNode():Node(Generic)
{
}

ContainerNode::ContainerNode(NodeType type):Node(type)
{
}

int ContainerNode::Invoke(NodeContext * context)
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

void ContainerNode::Swipe(NodeContext * context)
{
}

void ContainerNode::SetNodeList(list<Node*> * nodelist)
{
        subnodelist = nodelist;
}

void ContainerNode::AddNode(Node * node)
{
        this->subnodelist->push_front(node);
}

bool ContainerNode::Provision()
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

bool ContainerNode::Check()
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
