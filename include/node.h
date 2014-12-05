#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include "memorymgr.h"
#include "puppybase.h"
#include "variable.h"
#include "errstack.h"

using namespace std;


class Node: public PuppyObject
{
public:
		Node():ParentNode(NULL),NeedBreak(false),NeedContinue(false){}
		virtual ~Node(){}
		void Execute()
		{
			this->Invoke();
			this->Swipe();
		}
		virtual void Invoke() = 0;
		virtual void Swipe() = 0;

		virtual bool Transform(ErrorStack * errstack) = 0;

		void SetParentNode(Node * node)
		{
			this->ParentNode = node;
		}
		Node * GetParentNode()
		{
			return this->ParentNode;
		}
                void SetNeedBreak(bool val)
                {
                        this->NeedBreak = val;
                }
                void SetNeedContinue(bool val)
                {
                        this->NeedContinue = val;
                }
                bool GetNeedBreak()
                {
                        return this->NeedBreak;
                }
                bool GetNeedContinue()
                {
                        return this->NeedContinue;
                }
                Variable * GetVariable(string name)
                {
                        return this->Variables[name];
                }
                void AddVariable(Variable * var)
                {
                        // need optimize
			Variable * thevar = Variables[var->GetName()];
                        if(thevar != NULL)
			{	
				delete thevar;
				Variables.erase(var->GetName());
			}
			Variables[var->GetName()] = var;
                }
                Variable * FindVariable(string varname)
                {
                        Variable * result = this->Variables[varname];

                        if(result == NULL && this->ParentNode!=NULL)
                                return ParentNode->FindVariable(varname);

                        return result;
                }
private:
                map<string, Variable*> Variables;
                bool NeedBreak;
                bool NeedContinue;
		Node * ParentNode;
};

#endif
