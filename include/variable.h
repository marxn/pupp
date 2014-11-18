#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"
#include "identifier.h"

using namespace std;

class Variable
{
public:
	Variable(Identifier * ident):value(NULL)
	{
		this->VarName = ident->GetName();
	}
	Variable(string VarName)
	{
		this->VarName = VarName;
	}
	void SetValue(ConstValue * value)
	{
		this->value = value;
	}
	ConstValue * GetValue()
	{
		return value;
	}
	string GetName()
	{
		return VarName;
	}
private:
	string VarName;
	ConstValue * value;
};

#endif
