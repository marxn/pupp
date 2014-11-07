#ifndef _NODE_CONTEXT_H_
#define _NODE_CONTEXT_H_
#include <map>
#include "variable.h"

using namespace std;

class NodeContext
{
	public:
		NodeContext():NeedBreak(false),NeedContinue(false),PrevNodeContext(NULL){}
		virtual ~NodeContext()
		{ 
		}
		void SetNeedBreak(bool val)
		{
			this->NeedBreak = val;
		}
		void SetNeedContinue(bool val)
		{
			this->NeedContinue = val;
		}
		bool& GetNeedBreak()
		{
			return this->NeedBreak;
		}
		bool& GetNeedContinue()
		{
			return this->NeedContinue;
		}
		Variable * GetVariable(string name)
		{
			return Variables[name];
		}
		void AddVariable(Variable * var)
		{
			// need optimize
			if(Variables[var->GetName()] == NULL)
				Variables[var->GetName()] = var;
		}
		void SetPrevNodeContext(NodeContext * nodecontext)
		{
			this->PrevNodeContext = nodecontext;
		}
		NodeContext * GetPrevNodeContext()
		{
			return this->PrevNodeContext;
		}
		Variable * FindVariable(string varname)
		{
			NodeContext * target = NULL;
			Variable * result = NULL;
			
			for(target = this; target != NULL; target = target->GetPrevNodeContext())
			{
				result = target->Variables[varname];
				if(result!=NULL) break;
			}

			return result;
		}
private:
		NodeContext * PrevNodeContext;
		map<string, Variable*> Variables;
		bool NeedBreak;
		bool NeedContinue;
};

#endif