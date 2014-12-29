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
		void Swipe(){}
                void SetNodeList(list<Node*> * nodelist)
                {
                        subnodelist = nodelist;
                }
                bool Provision(ErrorStack * errstack)
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Provision(errstack)!=true)
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
		virtual void PreLoopStatement() = 0;
		virtual void PerOnceStatement() = 0;

                void Invoke()
                {
			this->PreLoopStatement();

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
					{
                                                 break;
					}
                                }
				this->PerOnceStatement();
                        }
                }
                void SetCondition(Expression * condition)
                {
                        this->condition = condition;
                }
                bool Provision(ErrorStack * errstack)
                {
                        if(ContainerNode::Provision(errstack)==false)
			{
				errstack->PushFrame(0, "Failed to transform container.");
				return false;
			}
                        condition->SetParentNode(this->GetParentNode());
                        if(condition->Provision(errstack)==false)
			{
				errstack->PushFrame(0, "Failed to transform condition expression.");
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
                virtual bool Evaluate()
                {
			ConstValue * eva = this->condition->Calculate();
			bool ret = static_cast<BooleanValue*>(eva)->GetValue();
			delete eva;

                        return ret;;
                }
                Expression * condition;
};

class WhileLoopNode: public LoopNode
{
public:
	void PreLoopStatement(){}
	void PerOnceStatement(){}
};

class ForLoopNode: public LoopNode
{
public:
	void SetPreLoopStatement(list<Node*> * preloop)
	{
		this->PreLoop = preloop;
	}
	void SetPerOnceStatement(list<Node*> * peronce)
	{
		this->PerOnce = peronce;
	}
	void PreLoopStatement()
	{
		list<Node*>::iterator i;
		for(i=this->PreLoop->begin(); i!= this->PreLoop->end(); i++)
		{
			(*i)->Execute();
		}
	}
        void PerOnceStatement()
	{
		list<Node*>::iterator i;
		for(i=this->PerOnce->begin(); i!= this->PerOnce->end(); i++)
		{
			(*i)->Execute();
		}
	}
	bool Provision(ErrorStack * errstack)
	{
		if(LoopNode::Provision(errstack)!=true)
		{
			return false;
		}

		list<Node*>::iterator i;
		for(i=this->PreLoop->begin(); i!= this->PreLoop->end(); i++)
                {
			(*i)->SetParentNode(this->GetParentNode());
                	if((*i)->Provision(errstack)!=true)
        	        {
	                        errstack->PushFrame(0, "Cannot identify pre-loop statement");
                        	return false;
                	}
		}

                for(i=this->PerOnce->begin(); i!= this->PerOnce->end(); i++)
                {
			(*i)->SetParentNode(this->GetParentNode());
			if((*i)->Provision(errstack)!=true)
                	{
                	        errstack->PushFrame(0, "Cannot identify per-once statement");
        	                return false;
	                }

		}
		return true;
	}
	void Swipe()
	{
	}
private:
	list<Node*> * PreLoop;
	list<Node*> * PerOnce;
};

class ForeachLoopNode: public ForLoopNode
{
public:
	void PreLoopStatement()
	{
		//ugly case: To reduce the memory cost, CollectionKeeper holds the memory 
		// allocated by CollectionExpr->Calculate(). So it needs to be freed later.
		this->CollectionKeeper = this->CollectionExpr->Calculate();
		this->Collection = static_cast<SetValue*>(this->CollectionKeeper)->GetValue();

		this->Handle = this->Collection->begin();
		
		if(this->Handle!=this->Collection->end())
		{
			this->ProvisionKV();
		}
	}
	void PerOnceStatement()
	{
		this->Handle++;

		if(this->Handle!=this->Collection->end())
		{
			this->ProvisionKV();
		}
	}
	void ProvisionKV()
	{
		Variable * key = this->FindVariable(this->Key);
                Variable * value = this->FindVariable(this->Value);

                ConstValue * tmpkey = new StringValue(this->Handle->first);
                key->SetValue(tmpkey);
		delete tmpkey;

                ConstValue * tmpvalue = this->Handle->second;
                value->SetValue(tmpvalue);
                value->SetType(tmpvalue->GetType());
	}
	void Swipe()
	{
		delete this->CollectionKeeper;
		ForLoopNode::Swipe();
	}
	bool Evaluate()
	{
		return this->Handle!=this->Collection->end();
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
	
	bool Provision(ErrorStack * errstack)
        {
		Variable * key = new Variable(this->Key);
                Variable * value = new Variable(this->Value);

                key->SetType(String);
                value->SetType(UnknownDataType);

                this->AddVariable(key);
                this->AddVariable(value);

                if(ForLoopNode::Provision(errstack)!=true)
                {
                        return false;
                }
		CollectionExpr->SetParentNode(this->GetParentNode());
		if(CollectionExpr->Provision(errstack)==false)
                {
                        errstack->PushFrame(0, "Failed to transform condition expression.");
                        return false;
                }
		if(CollectionExpr->GetDataType()!=Set)
		{
			errstack->PushFrame(0, "FOREACH need a collection as input.");
			return false;
		}

		return true;
	}
private:
	string Key;
	string Value;
	ConstValue * CollectionKeeper;
	SetExpression * CollectionExpr;
	map<string, ConstValue*> * Collection;
	map<string, ConstValue*>::iterator Handle;
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
		bool ProvisionElseStmt(ErrorStack * errstack)
		{
			list<Node*>::iterator i;
                        for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
                        {
                                (*i)->SetParentNode(this);
                                if((*i)->Provision(errstack)!=true)
                                {
                                        //errstack->PushFrame(0, "BranchNode transform failed.");
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
                                //errstack->PushFrame(0, "BranchNode transform failed - step 2");
                                return false;
                        }

                        if(ContainerNode::Provision(errstack)==false)
                        {
                                //errstack->PushFrame(0, "BranchNode transform failed - step 1");
                                return false;
                        }
			if(this->elsenodelist!=NULL && ProvisionElseStmt(errstack)==false)
			{
				return false;
			}

			return true;
                }
        private:
                bool Evaluate()
                {
			ConstValue * eva = this->condition->Calculate();
			bool ret = static_cast<BooleanValue*>(eva)->GetValue();
                        delete eva;

                        return ret;;
                }

                Expression * condition;
		list<Node*> * elsenodelist;
};


#endif
