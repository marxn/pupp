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
                bool Transform(ErrorStack * errstack)
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Transform(errstack)!=true)
				{
					//errstack->PushFrame(0, "ContainerNode transform failed.");
					return false;
				}
                        }
			return true;
                }
        protected:
                list<Node*> * subnodelist;
};

class ProcNode: public ContainerNode
{
public:
	void Invoke()
	{
		list<Node*>::iterator i;

		for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                {
			(*i)->Invoke();
		}
	}
private:
	
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
                bool Transform(ErrorStack * errstack)
                {
                        if(ContainerNode::Transform(errstack)==false)
			{
				//errstack->PushFrame(0, "LoopNode transform failed - step 1");
				return false;
			}
                        condition->SetParentNode(this->GetParentNode());
                        if(condition->Transform(errstack)==false)
			{
				//errstack->PushFrame(0, "LoopNode transform failed - step 2");
                                return false;
			}
			return true;
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
                bool Transform(ErrorStack * errstack)
                {
                        if(ContainerNode::Transform(errstack)==false)
                        {
                                //errstack->PushFrame(0, "BranchNode transform failed - step 1");
                                return false;
                        }
                        condition->SetParentNode(this->GetParentNode());
                        if(condition->Transform(errstack)==false)
                        {
                                //errstack->PushFrame(0, "BranchNode transform failed - step 2");
                                return false;
                        }
			return true;
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
