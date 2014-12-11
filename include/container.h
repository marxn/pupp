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
                                (*i)->Execute();
                }
		void Swipe()
		{
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
                                        (*i)->Execute();
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
			if(condition->GetDataType()!=Boolean)
			{
				errstack->PushFrame(0, "Boolean expression expected in while statement");
				return false;
			}
			return true;
                }
        private:
                bool Evaluate()
                {
			bool ret = static_cast<BooleanValue*>(condition->GetValue())->GetValue();
			condition->Swipe();
                        return ret;;
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
                                        (*i)->Execute();
                        }
			else
			{
				list<Node*>::iterator i;
                                for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
                                        (*i)->Execute();
			}
                }
		void SetElseNodeList(list<Node*> * nodelist)
                {
                        elsenodelist = nodelist;
                }

                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
		bool TransformElseStmt(ErrorStack * errstack)
		{
			list<Node*>::iterator i;
                        for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Transform(errstack)!=true)
                                {
                                        //errstack->PushFrame(0, "BranchNode transform failed.");
                                        return false;
                                }
                        }
                        return true;

		}
                bool Transform(ErrorStack * errstack)
                {
			condition->SetParentNode(this->GetParentNode());
                        if(condition->Transform(errstack)==false)
                        {
                                //errstack->PushFrame(0, "BranchNode transform failed - step 2");
                                return false;
                        }

                        if(ContainerNode::Transform(errstack)==false)
                        {
                                //errstack->PushFrame(0, "BranchNode transform failed - step 1");
                                return false;
                        }
			if(this->elsenodelist!=NULL && TransformElseStmt(errstack)==false)
			{
				return false;
			}

			return true;
                }
        private:
                bool Evaluate()
                {
			bool ret = static_cast<BooleanValue*>(condition->GetValue())->GetValue();
                        condition->Swipe();
                        return ret;;
                }

                Expression * condition;
		list<Node*> * elsenodelist;
};


#endif
