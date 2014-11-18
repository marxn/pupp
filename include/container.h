#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <iostream>
#include "node.h"

using namespace std;

class ContainerNode :public Node
{
        public:
                void Invoke()
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                                (*i)->Invoke();
                }
                void SetNodeList(list<Node*> * nodelist)
                {
                        subnodelist = nodelist;
                }
                void TransformAll()
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                (*i)->TransformAll();
                        }
                }
        protected:
                list<Node*> * subnodelist;
};

class LoopNode :public ContainerNode
{
        public:
                void Invoke()
                {
                        bool localctl = true;
                        while(localctl && Evaluate())
                        {
                                this->SetNeedBreak(false);
                                this->SetNeedContinue(false);

                                list<Node*>::iterator i;

                                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                                {
                                        (*i)->Invoke();
                                         if(this->GetNeedBreak())
                                         {
                                                 localctl = false;
                                                 break;
                                         }
                                         if(this->GetNeedContinue())
                                                 break;
                                }
                        }
                }
                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                void TransformAll()
                {
                        ContainerNode::TransformAll();
                        condition->SetParentNode(this->GetParentNode());
                        condition->TransformExpr();
                }
        private:
                bool Evaluate()
                {
                        ConstValue * temp = condition->GetValue();
                        if(temp->GetType() == Boolean)
                                return static_cast<BooleanValue*>(temp)->GetValue();

                        return false;
                }
                Expression * condition;
};


class BranchNode :public ContainerNode
{
        public:
                void Invoke()
                {
                        if(Evaluate())
                        {
                                list<Node*>::iterator i;
                                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                                        (*i)->Invoke();
                        }
                }
                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                void TransformAll()
                {
                        ContainerNode::TransformAll();
                        condition->SetParentNode(this->GetParentNode());
                        condition->TransformExpr();
                }
        private:
                bool Evaluate()
                {
                        ConstValue * temp = condition->GetValue();
                        if(temp->GetType() == Boolean)
                                return static_cast<BooleanValue*>(temp)->GetValue();

                        return false;
                }

                Expression * condition;
};


#endif
