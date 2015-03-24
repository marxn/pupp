#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"

using namespace std;

class VariableDef;

class Variable
{
public:
	Variable(string name, DataType type)
	{
		this->Source = NULL;
		this->VBox = NULL;
		this->VarName = name;
		this->VarType = type;
	}
	~Variable()
	{
		if(this->VBox)
                {
                        this->VBox->Destroy();
                }
	}

	string GetVarName()
        {
                return VarName;
        }

	void SetSource(VariableDef * def)
	{
		this->Source = def;
	}
	VariableDef * GetSource()
	{
		return this->Source;
	}

	void SetVBox(ValueBox * box)
	{
		this->VBox = box;
	}

	ValueBox * GetVBox()
	{
		return this->VBox;
	}

	void SetValue(ConstValue * value)
        {
		if(this->VBox==NULL)
		{
			this->VBox = new ValueBox;
		}

		if(this->VarType==value->GetType() || this->VarType==Any)
		{
			this->VBox->SetVal(value->DupValue());
		}
		else
		{
			ConstValueCaster caster(value, this->VarType);
			ConstValue * newvalue = caster.Cast();
			this->VBox->SetVal(newvalue);
		}
        }
        ConstValue * GetValue()
        {
		ConstValue * value = NULL;

		if(this->VBox==NULL)
		{
			this->VBox = new ValueBox;
			DefaultValueFactory defvalue(this->VarType);
			value = defvalue.GetValue();
			this->VBox->SetVal(value);
		}
		else
		{
			value = this->VBox->GetVal()->DupValue();
		}

                return value;
        }

	DataType GetVarType()
	{
		return this->VarType;
	}
	DataType GetValueType()
	{
		if(this->VBox==NULL)
                {
                        this->VBox = new ValueBox;
                        DefaultValueFactory defvalue(this->VarType);
                        ConstValue * value = defvalue.GetValue();
                        this->VBox->SetVal(value);
                }

		return this->VBox->GetVal()->GetType();
	}
private:
	VariableDef * Source;
	DataType VarType;
	string VarName;
	ValueBox * VBox;
};

class VariableDef
{
public:
	VariableDef(string VarName):VarType(Any)
	{
		this->VarName = VarName;
	}
	Variable * GetInstance()
	{
		Variable * ret = new Variable(this->VarName, this->VarType);
		ret->SetSource(this);

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
