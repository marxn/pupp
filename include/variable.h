#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"
#include "identifier.h"
#include "puppybase.h"

using namespace std;

class Variable
{
public:
	Variable(string name, DataType type)
	{
		this->Value = NULL;
		this->VarName = name;
		this->VarType = type;
	}
	~Variable()
	{
		if(this->Value)
                {
                        delete this->Value;
                }
	}

	string GetVarName()
        {
                return VarName;
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
	DataType GetVarType()
	{
		return this->VarType;
	}
	DataType GetValueType()
        {
                return this->Value->GetType();
        }
        ConstValue * GetReference()
        {
                return this->Value;
        }

private:
	ConstValue * Value;
	DataType VarType;
	string VarName;
};

class VariableDef: public PuppyObject
{
public:
	VariableDef(Identifier * ident):VarType(Any)
	{
		this->VarName = ident->GetName();
	}
	VariableDef(string VarName):VarType(Any)
	{
		this->VarName = VarName;
	}
	Variable * GetInstance()
	{
		Variable * ret = new Variable(this->VarName, this->VarType);
		NullValue * value = new NullValue;
		ret->SetValue(value);
		delete value;

		return ret;
	}
	void SetVarType(DataType type)
	{
		this->VarType = type;
	}
	DataType GetVarType()
        {
                return this->VarType;
        }
	string GetVarName()
	{
		return VarName;
	}
private:
	string VarName;
	DataType VarType;
};

#endif
