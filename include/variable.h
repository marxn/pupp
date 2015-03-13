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
		this->Value = NULL;
		this->VarName = name;
		this->VarType = type;
		this->Ref = NULL;
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

	void SetSource(VariableDef * def)
	{
		this->Source = def;
	}
	VariableDef * GetSource()
	{
		return this->Source;
	}

	void SetValue(ConstValue * value)
        {
		if(this->Ref)
                {
                        this->Ref->SetValue(value);
                }

                if(this->Value)
                {
                        delete this->Value;
                }
		if(this->VarType==value->GetType() || this->VarType==Any)
		{
			this->Value = value->DupValue();
		}
		else
		{
			ConstValueCaster caster(value, this->VarType);
			ConstValue * newvalue = caster.Cast();
			this->Value = newvalue;
		}
        }
        ConstValue * GetValue()
        {
		if(this->Ref)
                {
                        return this->Ref->GetValue();
                }

                return Value->DupValue();
        }
	DataType GetVarType()
	{
		if(this->Ref)
                {
                        return this->Ref->GetVarType();
                }

		return this->VarType;
	}
	DataType GetValueType()
        {
		if(this->Ref)
                {
                        return this->Ref->GetValueType();
                }

                return this->Value->GetType();
        }
        ConstValue * GetReference()
        {
		if(this->Ref)
                {
                        return this->Ref->GetReference();
                }

                return this->Value;
        }
	void SetReference(ConstValue * refvalue)
	{
		if(this->Value)
                {
                        delete this->Value;
                }
                this->Value = refvalue;
	}
	void SetRefVar(Variable * var)
	{
		this->Ref = var;
	}
	Variable * GetRealVar()
	{
		if(this->Ref)
		{
			return this->Ref;
		}
		return this;
	}
private:
	VariableDef * Source;
	ConstValue * Value;
	DataType VarType;
	string VarName;
	Variable * Ref;
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
		NullValue * value = new NullValue;
		ret->SetValue(value);
		ret->SetSource(this);

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
