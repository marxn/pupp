#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"
#include "identifier.h"
#include "puppybase.h"

using namespace std;

class Variable: public PuppyObject
{
public:
	Variable(Identifier * ident):Value(NULL), VarType(Any)
	{
		this->VarName = ident->GetName();
	}
	Variable(string VarName):Value(NULL), VarType(Any)
	{
		this->VarName = VarName;
	}
	void SetVarType(DataType type)
	{
		this->VarType = type;
	}
	DataType GetVarType()
        {
                return this->VarType;
        }
	DataType GetValueType()
	{
		return this->Value->GetType();
	}
	void SetValue(ConstValue * value)
	{
		if(this->Value)
		{
			delete this->Value;
		}
		this->Value = value->DupValue();
	}
	ConstValue * GetValue()
	{
		return Value->DupValue();
	}
	ConstValue * GetReference()
	{
		return this->Value;
	}
	string GetName()
	{
		return VarName;
	}
private:
	string VarName;
	DataType VarType;
	ConstValue * Value;
};

#endif
