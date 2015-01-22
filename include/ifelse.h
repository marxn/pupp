#ifndef _PUPPY_IFELSE_H_
#define _PUPPY_IFELSE_H_

#include <list>
#include "expression.h"
#include "container.h"

using namespace std;

class BranchNode :public ContainerNode
{
        public:
                int Invoke(NodeContext * context)
                {
			int ret = this->Evaluate(context);
			
                        if(ret==EVA_TRUE)
                        {
                                list<Node*>::iterator i;
                                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                                {
					context->AddFrame(*i);
					int ret = (*i)->Execute(context);
					context->PopFrame();

                                        if(ret!=NODE_RET_NORMAL)
                                        {
                                                return ret;
                                        }
                                }
                        }
                        else if(ret==EVA_FALSE)
                        {
                                list<Node*>::iterator i;
                                for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
				{
					context->AddFrame(*i);
                                        int ret = (*i)->Execute(context);
					context->PopFrame();

					if(ret!=NODE_RET_NORMAL)
                                        {
                                                return ret;
                                        }
				}

                        }
                        else
                        {
                                return NODE_RET_ERROR;
                        }
                        return NODE_RET_NORMAL;
                }
                void SetElseNodeList(list<Node*> * nodelist)
                {
                        elsenodelist = nodelist;
                }

                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                bool ProvisionElseStmt(ErrorStack * errstack)
                {
                        list<Node*>::iterator i;
                        for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Provision(errstack)!=true)
                                {
                                        return false;
                                }
                        }
                        return true;

                }
                bool Provision(ErrorStack * errstack)
                {
                        condition->SetParentNode(this->GetParentNode());
                        if(condition->Provision(errstack)==false)
                        {
                                return false;
                        }

                        if(ContainerNode::Provision(errstack)==false)
                        {
                                return false;
                        }
                        if(this->elsenodelist!=NULL && ProvisionElseStmt(errstack)==false)
                        {
                                return false;
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
                list<Node*> * elsenodelist;
};

#endif
