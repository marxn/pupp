#ifndef _PUPPY_IFELSE_H_
#define _PUPPY_IFELSE_H_

#include <list>
#include "expression.h"
#include "container.h"

using namespace std;

class BranchNode :public ContainerNode
{
        public:
		BranchNode():ifnode(NULL),elsenode(NULL){}
                int Invoke(NodeContext * context)
                {
			int ret = this->Evaluate(context);
			
                        if(ret==EVA_TRUE)
                        {
				context->AddFrame(this->ifnode);
				int ret = this->ifnode->Execute(context);
				context->PopFrame();
				return ret;
                        }
                        else if(ret==EVA_FALSE)
                        {
				context->AddFrame(this->elsenode);
				int ret = this->elsenode->Execute(context);
				context->PopFrame();
				return ret;
                        }
                        return NODE_RET_ERROR;
                }
		void SetIfNode(Node * node)
		{
			this->ifnode = node;
		}
                void SetElseNode(Node * node)
                {
                        this->elsenode = node;
                }

                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                bool Provision(ErrorStack * errstack)
                {
                        condition->SetParentNode(this->GetParentNode());
                        if(condition->Provision(errstack)==false)
                        {
                                return false;
                        }
			this->ifnode->SetParentNode(this->GetParentNode());
                        if(this->ifnode->Provision(errstack)==false)
                        {
                                return false;
                        }
                        if(this->elsenode!=NULL)
			{
				this->elsenode->SetParentNode(this->GetParentNode());
				if(this->elsenode->Provision(errstack)==false)
                        	{
                                	return false;
	                        }
			}
                        return true;
                }
		bool Check(ErrorStack * errstack)
		{
			if(condition->Check(errstack)==false)
                        {
                                return false;
                        }
			if(this->ifnode->Check(errstack)==false)
                        {
                                return false;
                        }
			if(this->elsenode!=NULL)
			{
				if(this->elsenode->Check(errstack)==false)
                        	{
                                	return false;
	                        }
			}
			return true;
		}
        private:
                int Evaluate(NodeContext * context)
                {
                        ConstValue * eva = this->condition->Calculate(context);
                        if(eva->GetType()!=Boolean)
                        {
                                //TODO
                                cerr<<"Wrong data type in if statement - expect a boolean expression."<<endl;
                                return EVA_ERROR;
                        }

                        bool ret = static_cast<BooleanValue*>(eva)->GetValue();
                        delete eva;

                        return ret?EVA_TRUE:EVA_FALSE;
                }

                Expression * condition;
		Node * ifnode;
		Node * elsenode;
};

#endif
