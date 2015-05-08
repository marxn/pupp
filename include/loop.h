#ifndef _PUPPY_LOOP_H_
#define _PUPPY_LOOP_H_

#include <list>
#include "expression.h"
#include "container.h"

using namespace std;

class LoopNode :public ContainerNode
{
public:
                LoopNode():ContainerNode(Loop){}

                virtual bool PreLoopStatement(NodeContext * context)
                {
                        return true;
                }
                virtual bool PerOnceStatement(NodeContext * context)
                {
                        return true;
                }

                int Invoke(NodeContext * context)
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
                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                bool Provision()
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
                bool Check()
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
        private:
                //1 - true, 0 - false, -1 - error
                virtual int Evaluate(NodeContext * context)
                {
                        ConstValue * eva = this->condition->Calculate(context);
                        if(eva==NULL)
                        {
                                return -1;
                        }
                        if(eva->GetType()!=Boolean)
                        {
                                //TODO
                                cerr<<"puppy runtime error: Wrong data type in while expression - expect a boolean expression."<<endl;
                                return -1;
                        }

                        bool ret = static_cast<BooleanValue*>(eva)->GetValue();
                        delete eva;

                        return ret?1:0;
                }
                Expression * condition;
};

class WhileLoopNode: public LoopNode
{
};

class ForLoopNode: public LoopNode
{
public:
        void SetPreLoopStatement(Node * preloop)
        {
                this->PreLoop = preloop;
        }
        void SetPerOnceStatement(Node * peronce)
        {
                this->PerOnce = peronce;
        }
        bool PreLoopStatement(NodeContext * context)
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
        bool PerOnceStatement(NodeContext * context)
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
        bool Provision()
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
        bool Check()
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

        void Swipe(NodeContext * context)
        {
        }
private:
        Node * PreLoop;
        Node * PerOnce;
};

class ForeachLoopNode: public ForLoopNode
{
public:
        bool PreLoopStatement(NodeContext * context)
        {
                ConstValue * keeper = this->CollectionExpr->Calculate(context);
                if(keeper==NULL)
                {
                        return false;
                }

                if(keeper->GetType()!=Set)
                {
                        cerr<<"puppy runtime warning: FOREACH need a collection as input."<<endl;
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
        bool PerOnceStatement(NodeContext * context)
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
        void ProvisionKV(NodeContext * context)
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
        void Swipe(NodeContext * context)
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

        //1 - true, 0 - false, -1 - error
        int Evaluate(NodeContext * context)
        {
                ForeachLoopCtx * ctx = context->ForeachCtx.top();
                if(ctx==NULL)
                {
                        return EVA_FALSE;
                }

                bool ret = ctx->ValueHandle!=ctx->SetValueHolder->end();
                return ret?EVA_TRUE:EVA_FALSE;
        }

        void SetCollectionExpr(SetExpression * expr)
        {
                this->CollectionExpr = expr;
        }
        void SetKV(string key, string value)
        {
                this->Key = key;
                this->Value = value;
        }

        bool Provision()
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
        bool Check()
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
private:
        string Key;
        string Value;
        SetExpression * CollectionExpr;
};

#endif
