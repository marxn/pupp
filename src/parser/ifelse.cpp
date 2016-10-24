#include "parser/ifelse.h"

using namespace std;

BranchNode::BranchNode():ContainerNode(IfElse)
{
        this->ifnode   = NULL;
        this->elsenode = NULL;
}

int BranchNode::Invoke(NodeContext * context)
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
                if(this->elsenode!=NULL)
                {
                        context->AddFrame(this->elsenode);
                        int ret = this->elsenode->Execute(context);
                        context->PopFrame();
                        return ret;
                }

                return NODE_RET_NORMAL;
        }

        return NODE_RET_ERROR;
}

void BranchNode::SetIfNode(Node * node)
{
        this->ifnode = node;
}

void BranchNode::SetElseNode(Node * node)
{
        this->elsenode = node;
}

void BranchNode::SetCondition(Expression * condition)
{
        this->condition = condition;
}

bool BranchNode::Provision()
{
        condition->SetParentNode(this);
        if(condition->Provision()==false)
        {
               return false;
        }

        this->ifnode->SetParentNode(this);
        if(this->ifnode->Provision()==false)
        {
               return false;
        }

        if(this->elsenode!=NULL)
        {
               this->elsenode->SetParentNode(this);
               if(this->elsenode->Provision()==false)
               {
                        return false;
               }
        }

        return true;
}

bool BranchNode::Check()
{
        if(condition->Check()==false)
        {
                return false;
        }

        if(this->ifnode->Check()==false)
        {
                return false;
        }

        if(this->elsenode!=NULL)
        {
                if(this->elsenode->Check()==false)
                {
                        return false;
                }
        }

        return true;
}

int BranchNode::Evaluate(NodeContext * context)
{
        ConstValue * eva = this->condition->Calculate(context);
        if(eva==NULL)
        {
                return EVA_ERROR;
        }

        if(eva->GetType()!=Boolean)
        {
                ConstValue * thevalue = ConstValueCast(eva, Boolean);
                
                if(thevalue==NULL)
                {
                        cerr<<"pupp runtime error: Wrong data type in if statement - expect a boolean value."<<endl;
                        return EVA_ERROR;
                }
                
                delete eva;
                eva = thevalue;
                
        }

        bool ret = static_cast<BooleanValue*>(eva)->GetValue();
        delete eva;

        return ret?EVA_TRUE:EVA_FALSE;
}

