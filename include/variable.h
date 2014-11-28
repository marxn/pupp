#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"
#include "identifier.h"

using namespace std;

class Variable
{
public:
	Variable(Identifier * ident):value(NULL), VarType(UnknownDataType)
	{
		this->VarName = ident->GetName();
	}
	Variable(string VarName):value(NULL), VarType(UnknownDataType)
	{
		this->VarName = VarName;
	}
	void SetType(DataType type)
	{
		this->VarType = type;
	}
	DataType GetType()
	{
		return this->VarType;
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
	DataType VarType;
	ConstValue * value;
};

#endif
