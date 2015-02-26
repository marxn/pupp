#ifndef _PORTAL_H_
#define _PORTAL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "puppybase.h"
#include "variable.h"

class Portal
{
public:
	Portal(Node * bean)
	{
		this->Bean = bean;
	}
	~Portal()
	{
		map<VariableDef*, Variable*>::iterator i;
		for(i = this->SharedVar.begin(); i!= this->SharedVar.end(); i++)
		{
			delete i->second;
		}
	}
	Variable * GetSharedVariable(VariableDef * def)
	{
		Variable * result = NULL;
		map<VariableDef*, Variable*>::iterator pos = this->SharedVar.find(def);
		if(pos!=this->SharedVar.end())
		{
			return pos->second;
		}
		else
		{
			result = def->GetInstance();
			this->SharedVar.insert(pair<VariableDef*, Variable*>(def, result));
		}
		return result;
	}
private:
	Node * Bean;
	map<VariableDef*, Variable*> SharedVar;
};

#endif
