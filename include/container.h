#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <iostream>
#include "node.h"

using namespace std;

class ContainerNode :public Node
{
        public:
                bool Invoke()
                {
                        list<Node*>::iterator i;
                        for(i = subnodelist->begin(); i != subnodelist->end(); i++)
			{
                                if((*i)->Execute()==false)
				{
					return false;
				}
			}
			return true;
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
		virtual bool PreLoopStatement() = 0;
		virtual bool PerOnceStatement() = 0;

                bool Invoke()
                {
			if(this->PreLoopStatement()==false)
			{
				return false;
			}

                        bool localctl = true;
                        while(localctl)
                        {
				int runnable = this->Evaluate();
				if(runnable==-1)
				{
					return false;
				}
				else if(runnable==0)
				{
					break;
				}

                                this->SetNeedBreak(false);
                                this->SetNeedContinue(false);

                                list<Node*>::iterator i;

                                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                                {
                                        if((*i)->Execute()==false)
					{
						return false;
					}
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
				if(this->PerOnceStatement()==false)
				{
					return false;
				}
                        }
			return true;
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
			return true;
                }
        private:
		//1 - true, 0 - false, -1 - error
                virtual int Evaluate()
                {
			ConstValue * eva = this->condition->Calculate();
			if(eva->GetType()!=Boolean)
			{
				//TODO
				//errstack->PushFrame(0, "Wrong data type in while expression - expect a boolean expression.");
				cerr<<"Wrong data type in while expression - expect a boolean expression."<<endl;
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
public:
	bool PreLoopStatement()
	{
		return true;
	}
	bool PerOnceStatement()
	{
		return true;
	}
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
	bool PreLoopStatement()
	{
		list<Node*>::iterator i;
		for(i=this->PreLoop->begin(); i!= this->PreLoop->end(); i++)
		{
			if((*i)->Execute()==false)
			{
				return false;
			}
		}
		return true;
	}
        bool PerOnceStatement()
	{
		list<Node*>::iterator i;
		for(i=this->PerOnce->begin(); i!= this->PerOnce->end(); i++)
		{
			if((*i)->Execute()==false)
			{
				return false;
			}
		}
		return true;
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
	bool PreLoopStatement()
	{
		//ugly case: To reduce the memory cost, CollectionKeeper holds the memory 
		// allocated by CollectionExpr->Calculate(). So it needs to be freed later.
		this->CollectionKeeper = this->CollectionExpr->Calculate();
		if(this->CollectionKeeper->GetType()!=Set)
		{
			//errstack->PushFrame(0, "FOREACH need a collection as input.");
			//TODO
			cerr<<"FOREACH need a collection as input."<<endl;
			return false;
		}
		this->Collection = static_cast<SetValue*>(this->CollectionKeeper)->GetValue();

		this->Handle = this->Collection->begin();
		
		if(this->Handle!=this->Collection->end())
		{
			this->ProvisionKV();
		}
		return true;
	}
	bool PerOnceStatement()
	{
		this->Handle++;

		if(this->Handle!=this->Collection->end())
		{
			this->ProvisionKV();
		}
		return true;
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

	//1 - true, 0 - false, -1 - error
        int Evaluate()
        {
		bool ret = this->Handle!=this->Collection->end();
                return ret?1:0;
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
                value->SetType(Null);

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
                bool Invoke()
                {
                        if(this->Evaluate()==1)
                        {
                                list<Node*>::iterator i;
                                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
				{
                                        if((*i)->Execute()==false)
					{
						return false;
					}
				}
                        }
			else if(this->Evaluate()==0)
			{
				list<Node*>::iterator i;
                                for(i = elsenodelist->begin(); i != elsenodelist->end(); i++)
                                        if((*i)->Execute()==false)
					{
						return false;
					}
			}
			else
			{
				return false;
			}
			return true;
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
                int Evaluate()
                {
			ConstValue * eva = this->condition->Calculate();
			if(eva->GetType()!=Boolean)
                        {
				//TODO
                                //errstack->PushFrame(0, "Wrong data type in while expression - expect a boolean expression.");
				cerr<<"Wrong data type in if statement - expect a boolean expression."<<endl;
                                return -1;
                        }

			bool ret = static_cast<BooleanValue*>(eva)->GetValue();
                        delete eva;

                        return ret?1:0;
                }

                Expression * condition;
		list<Node*> * elsenodelist;
};


#endif
