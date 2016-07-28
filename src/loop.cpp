#include "parser/loop.h"

using namespace std;

LoopNode::LoopNode():ContainerNode(Loop)
{
}

bool LoopNode::PreLoopStatement(NodeContext * context)
{
        return true;
}

bool LoopNode::PerOnceStatement(NodeContext * context)
{
        return true;
}

int LoopNode::Invoke(NodeContext * context)
{
        if(this->PreLoopStatement(context)==false)
        {
                return NODE_RET_ERROR;
        }

        int runnable = this->Evaluate(context);
        if(runnable==-1)
        {
                return NODE_RET_ERROR;
        }

        while(runnable==1)
        {
                list<Node*>::iterator i;
                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                {
                        if((*i)->Type==Function)
                        {
                                continue;
                        }

                        context->AddFrame(*i);
                        int ret = (*i)->Execute(context);
                        context->PopFrame();

                        if(ret==NODE_RET_NEEDBREAK)
                        {
                                return NODE_RET_NORMAL;
                        }
                        else if(ret==NODE_RET_NEEDCONTINUE)
                        {
                                break;
                        }
                        else if(ret==NODE_RET_ERROR)
                        {
                                return ret;
                        }
                }

                if(this->PerOnceStatement(context)==false)
                {
                        return NODE_RET_ERROR;
                }

                runnable = this->Evaluate(context);
                if(runnable==-1)
                {
                        return NODE_RET_ERROR;
                }

        }

        return NODE_RET_NORMAL;
}

void LoopNode::SetCondition(Expression * condition)
{
        this->condition = condition;
}

bool LoopNode::Provision()
{
        if(ContainerNode::Provision()==false)
        {
                return false;
        }

        condition->SetParentNode(this->GetParentNode());
        if(condition->Provision()==false)
        {
                return false;
        }

        return true;
}

bool LoopNode::Check()
{
        if(ContainerNode::Check()==false)
        {
                return false;
        }

        if(condition->Check()==false)
        {
                return false;
        }
 
        return true;
}

int LoopNode::Evaluate(NodeContext * context)
{
        ConstValue * eva = this->condition->Calculate(context);
        if(eva==NULL)
        {
                return -1;
        }

        if(eva->GetType()!=Boolean)
        {
                //TODO
                cerr<<"pupp runtime error: Wrong data type in while expression - expect a boolean expression."<<endl;
                return -1;
        }

        bool ret = static_cast<BooleanValue*>(eva)->GetValue();
        delete eva;

        return ret?1:0;
}

void ForLoopNode::SetPreLoopStatement(Node * preloop)
{
        this->PreLoop = preloop;
}

void ForLoopNode::SetPerOnceStatement(Node * peronce)
{
        this->PerOnce = peronce;
}

bool ForLoopNode::PreLoopStatement(NodeContext * context)
{
        if(this->PreLoop==NULL)
        {
                return true;
        }

        int ret = this->PreLoop->Execute(context);
        if(ret!=NODE_RET_NORMAL)
        {
                return false;
        }

        return true;
}

bool ForLoopNode::PerOnceStatement(NodeContext * context)
{
        if(this->PerOnce==NULL)
        {
                return true;
        }

        int ret = this->PerOnce->Execute(context);
        if(ret!=NODE_RET_NORMAL)
        {
                return false;
        }

        return true;
}

bool ForLoopNode::Provision()
{
        if(LoopNode::Provision()!=true)
        {
                return false;
        }

        if(this->PreLoop)
        {
                this->PreLoop->SetParentNode(this->GetParentNode());
                if(this->PreLoop->Provision()!=true)
                {
                        return false;
                }
        }

        if(this->PerOnce)
        {
                this->PerOnce->SetParentNode(this->GetParentNode());
                if(this->PerOnce->Provision()!=true)
                {
                        return false;
                }
        }

        return true;
}

bool ForLoopNode::Check()
{
        if(LoopNode::Check()!=true)
        {
                return false;
        }

        if(this->PreLoop)
        {
                if(this->PreLoop->Check()!=true)
                {
                        return false;
                }
        }

        if(this->PerOnce)
        {
                if(this->PerOnce->Check()!=true)
                {
                        return false;
                }
        }

        return true;
}

void ForLoopNode::Swipe(NodeContext * context)
{
}

bool ForeachLoopNode::PreLoopStatement(NodeContext * context)
{
        ConstValue * keeper = this->CollectionExpr->Calculate(context);
        if(keeper==NULL)
        {
                return false;
        }

        if(keeper->GetType()!=Set)
        {
                cerr<<"pupp runtime warning: FOREACH need a collection as input."<<endl;
                context->ForeachCtx.push(NULL);
                return true;
        }

        ForeachLoopCtx * ctx = new ForeachLoopCtx;
        ctx->Keeper         = keeper;
        ctx->SetValueHolder = static_cast<SetValue*>(keeper)->GetValue();
        ctx->ValueHandle    = ctx->SetValueHolder->begin();

        context->ForeachCtx.push(ctx);

        if(ctx->ValueHandle!=ctx->SetValueHolder->end())
        {
                this->ProvisionKV(context);
        }

        return true;
}

bool ForeachLoopNode::PerOnceStatement(NodeContext * context)
{
        ForeachLoopCtx * ctx = context->ForeachCtx.top();
        if(ctx==NULL)
        {
                return true;
        }

        ctx->ValueHandle++;

        if(ctx->ValueHandle!=ctx->SetValueHolder->end())
        {
                this->ProvisionKV(context);
        }

        return true;
}

void ForeachLoopNode::ProvisionKV(NodeContext * context)
{
        ForeachLoopCtx * ctx = context->ForeachCtx.top();

        Variable * key = context->GetVariable(this->Key);
        Variable * value = context->GetVariable(this->Value);

        ConstValue * tmpkey = new StringValue(ctx->ValueHandle->first);
        key->SetValue(tmpkey);
        delete tmpkey;

        ValueBox * tmpvalue = ctx->ValueHandle->second;

        value->SetVarType(tmpvalue->GetVal()->GetType());
        value->SetValue(tmpvalue->GetVal());
}

void ForeachLoopNode::Swipe(NodeContext * context)
{
        ForeachLoopCtx * ctx = context->ForeachCtx.top();
        if(ctx!=NULL)
        {
                delete ctx->Keeper;
                delete ctx;
        }

        context->ForeachCtx.pop();
        ForLoopNode::Swipe(NULL);
}

int ForeachLoopNode::Evaluate(NodeContext * context)
{
        ForeachLoopCtx * ctx = context->ForeachCtx.top();
        if(ctx==NULL)
        {
                return EVA_FALSE;
        }

        bool ret = ctx->ValueHandle!=ctx->SetValueHolder->end();
        return ret?EVA_TRUE:EVA_FALSE;
}

void ForeachLoopNode::SetCollectionExpr(SetExpression * expr)
{
        this->CollectionExpr = expr;
}

void ForeachLoopNode::SetKV(string key, string value)
{
        this->Key = key;
        this->Value = value;
}

bool ForeachLoopNode::Provision()
{
        VariableDef * key = new VariableDef(this->Key);
        VariableDef * value = new VariableDef(this->Value);

        key->SetVarType(String);
        value->SetVarType(Null);

        this->AddVariable(key);
        this->AddVariable(value);

        if(ForLoopNode::Provision()!=true)
        {
                return false;
        }

        CollectionExpr->SetParentNode(this->GetParentNode());
        if(CollectionExpr->Provision()==false)
        {
                return false;
        }

        return true;
}

bool ForeachLoopNode::Check()
{
        if(ForLoopNode::Check()!=true)
        {
                return false;
        }

        if(CollectionExpr->Check()==false)
        {
                return false;
        }

        return true;
}
