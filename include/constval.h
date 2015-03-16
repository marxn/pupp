#ifndef _CONSTVAL_H_
#define _CONSTVAL_H_

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

using namespace std;

enum DataType
{
        Null = 0, Any, Integer, Decimal, Boolean, String, KeyValue, Set
};

class ConstValue
{
public:
	ConstValue():Type(Null){}
	ConstValue(DataType type):Type(type){}
	virtual ~ConstValue(){}
	virtual ConstValue * DupValue() = 0;
	DataType GetType() {return this->Type;}
	virtual string toString() = 0;
protected:
	DataType Type;
};

class NullValue: public ConstValue
{
public:
	NullValue():ConstValue(Null){}
	ConstValue * DupValue()
	{
		return new NullValue;
	}
	string toString()
	{
		return "Null";
	}
};

class StringValue: public ConstValue
{
public:
        StringValue():ConstValue(String){}
        StringValue(string value):Value(value), ConstValue(String){}
        ConstValue * DupValue()
        {
                return new StringValue(this->Value);
        }
        bool LooksLikeInteger()
        {
                char buf[256];
                long value;
                int n = sscanf(this->Value.c_str(), "%ld", &value);
                if(n>0)
                {
                        sprintf(buf, "%ld", value);

                        if(strlen(buf)==strlen(this->Value.c_str()))
                        {
                                return true;
                        }
                }
                return false;
        }

        bool LooksLikeDecimal()
        {
                mpf_t temp;
                mpf_init(temp);
                if(mpf_set_str(temp, this->Value.c_str(), 10)==0)
                {
			mpf_clear(temp);
                        return true;
                }
		mpf_clear(temp);
                return false;
        }

        int GetNumPrec()
        {
                char * str = strdup(this->Value.c_str());
                char * p = strstr(str, ".");
                if(p!=NULL)
                {
			int ret = strlen(p) - 1;
			free(str);
                        return ret;
                }
                free(str);
                return 0;
        }

        string toString()
        {
                return this->Value;
        }

protected:
        string Value;
};

class IntegerValue: public ConstValue
{
public:
	IntegerValue(long value):Value(value), ConstValue(Integer){}
	IntegerValue(string value):  ConstValue(Integer)
	{
		this->Value = atol(value.c_str());
	}
	IntegerValue(IntegerValue& value): ConstValue(Integer)
	{
		this->Value = value.GetValue();
	}
	long GetValue()
	{
		return this->Value;
	}
	void SetValue(long n)
	{
		this->Value = n;
	}
	ConstValue * DupValue()
	{
		return new IntegerValue(this->Value);
	}
	string toString()
	{
		char buf[256] = "0";
		snprintf(buf, sizeof(buf), "%ld", this->Value);
		return string(buf);
	}

protected:
	long Value;
};

class DecimalValue: public ConstValue
{
public:
	DecimalValue():ConstValue(Decimal)
        {
		this->Prec = 2;
                mpf_init_set_ui(this->Value,0);
        }
	DecimalValue(string valuestr):ConstValue(Decimal)
	{
		StringValue temp(valuestr);
		mpf_init_set_str(this->Value,valuestr.c_str(),10);
                this->Prec = temp.GetNumPrec();
	}
	DecimalValue(long value):ConstValue(Decimal)
        {
                this->Prec = 2;
		mpf_init_set_si(this->Value,value);
	}
	DecimalValue(IntegerValue * value):ConstValue(Decimal)
        {
                this->Prec = 2;
                mpf_init_set_si(this->Value,value->GetValue());
        }
	DecimalValue(const DecimalValue& value):ConstValue(Decimal)
	{
                this->Prec = value.GetPrec();
		mpf_init(this->Value);
		mpf_set(this->Value, value.Value);
	}
	DecimalValue(DecimalValue * value):ConstValue(Decimal)
        {
                this->Prec = value->GetPrec();
                mpf_init(this->Value);
                mpf_set(this->Value, value->Value);
        }
	DecimalValue& operator = (const DecimalValue& value)
	{
                this->Prec = value.GetPrec();
		mpf_set(this->Value, value.Value);
	}
	~DecimalValue()
	{
		mpf_clear(this->Value);
	}
        ConstValue * DupValue()
        {
		ConstValue * result = new DecimalValue(this);
                return result;
        }
	void SetPrec(int n)
	{
		this->Prec = n;
	}
	int GetPrec() const
	{
		return this->Prec;
	}
	DecimalValue operator + (DecimalValue& element)
	{
		DecimalValue ret;
		mpf_add(ret.Value, this->Value, element.Value);
		return ret;
	}
	DecimalValue operator - (DecimalValue& element)
        {
                DecimalValue ret;
                mpf_sub(ret.Value, this->Value, element.Value);
                return ret;
        }
	DecimalValue operator * (DecimalValue& element)
        {
                DecimalValue ret;
                mpf_mul(ret.Value, this->Value, element.Value);
                return ret;
        }
	DecimalValue operator / (DecimalValue& element)
        {
                DecimalValue ret;
                mpf_div(ret.Value, this->Value, element.Value);
                return ret;
        }
	bool operator > (DecimalValue& element)
        {
		return mpf_cmp(this->Value, element.Value)>0?true:false;
        }
	bool operator < (DecimalValue& element)
        {
                return mpf_cmp(this->Value, element.Value)<0?true:false;
        }
	bool operator == (DecimalValue& element)
        {
                return mpf_cmp(this->Value, element.Value)==0?true:false;
        }
	bool operator >= (DecimalValue& element)
        {
                return mpf_cmp(this->Value, element.Value)>=0?true:false;
        }
        bool operator <= (DecimalValue& element)
        {
                return mpf_cmp(this->Value, element.Value)<=0?true:false;
        }
	bool operator != (DecimalValue& element)
        {
                return mpf_cmp(this->Value, element.Value)!=0?true:false;
        }


        string toString()
        {
		char result[256];
                gmp_sprintf (result,"%.*Ff", this->Prec, this->Value);
                return result;
        }
	mpf_t Value;

protected:
	int Prec;
};

class BooleanValue: public ConstValue
{
public:
	BooleanValue():Value(false), ConstValue(Boolean){}
	BooleanValue(bool value):Value(value), ConstValue(Boolean){}
	BooleanValue(string value)
	{
		this->Value = (value=="true"?true:false);
	}
        ConstValue * DupValue()
        {
                return new BooleanValue(this->Value);
        }

	bool GetValue() {return this->Value;}
        string toString()
        {
                if(this->Value) return "true";
		return "false";
        }

protected:
	bool Value;
};

class KVValue: public ConstValue
{
public:
	KVValue():ConstValue(KeyValue){}
	KVValue(pair<ConstValue*, ConstValue*> kv):ConstValue(KeyValue)
	{
		this->Value.first = kv.first->DupValue();
		this->Value.second = kv.second->DupValue();
	}
	~KVValue()
	{
		delete this->Value.first;
		delete this->Value.second;
	}
	ConstValue * DupValue()
        {
                return new KVValue(this->Value);
        }

        pair<ConstValue*, ConstValue*> GetValue() {return this->Value;}
        string toString()
        {
		string ret = "<";
		ret.append(this->Value.first->toString());
		ret.append(",");
		ret.append(this->Value.second->toString());
		ret.append(">");
                return ret;
        }

protected:
	pair<ConstValue*, ConstValue*> Value;
};

class SetValue: public ConstValue
{
public:
        SetValue():ConstValue(Set){}
        SetValue(map<string, ConstValue*>& value):ConstValue(Set)
	{
		map<string, ConstValue*>::iterator i;
		for(i=value.begin();i!=value.end();i++)
		{
			this->Value.insert(pair<string, ConstValue*>(i->first, i->second->DupValue()));
		}
	}
	~SetValue()
	{
		map<string, ConstValue*>::iterator i;
                for(i=this->Value.begin();i!=this->Value.end();i++)
                {
			delete i->second;
                }
		this->Value.clear();
	}
	void AddKV(KVValue * kv)
	{
		string key = kv->GetValue().first->toString();
		map<string, ConstValue*>::iterator i = this->Value.find(key);

        	if(i!=this->Value.end())
                {
			delete i->second;	
                        this->Value.erase(key);
                }
                this->Value.insert(pair<string, ConstValue*>(key, kv->GetValue().second->DupValue()));
	}
	void RemoveKV(string key)
	{
		this->Value.erase(key);
	}

	ConstValue * FindByKey(string key)
	{
		return this->Value[key];
	}
	map<string, ConstValue*> * GetValue()
	{
		return &this->Value;
	}	
	ConstValue * DupValue()
        {
                return new SetValue(this->Value);
        }

        string toString()
        {
		string ret="{";

		int size = this->Value.size();
		map<string, ConstValue*>::iterator i;
		for(i=this->Value.begin(); i!=this->Value.end(); i++, size--)
		{
			ret.append("<");
			ret.append(i->first);
			ret.append(",");
			ret.append(i->second->toString());
			ret.append(">");
			if(size>1)
			{
				ret.append(",");
			}
		}
		ret.append("}");
                return ret;
        }
protected:
        map<string, ConstValue*> Value;
};

class ConstValueCaster
{
public:
	ConstValueCaster(ConstValue * value, DataType type)
	{
		this->Value = value;
		this->Type = type;
	}
	ConstValue * Cast()
	{
		switch(this->Type)
		{
			case String:
				return new StringValue(this->Value->toString());
			break;
			case Decimal:
				return new DecimalValue(this->Value->toString());
			break;
			case Integer:
				return new IntegerValue(this->Value->toString());
			break;
			case Boolean:
				return new BooleanValue(this->Value->toString());
			break;
			case KeyValue:
				return new KVValue;
			break;
			case Set:
				return new SetValue;
			break;
		}
		return NULL;
	}
private:
	ConstValue * Value;
	DataType Type;
};

class Operation
{
public:
	static ConstValue * AddOperation(ConstValue * left, ConstValue * right)
	{
		ConstValue * result = NULL;
		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() + static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new DecimalValue(leftvalue + *static_cast<DecimalValue*>(right));
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new DecimalValue(*static_cast<DecimalValue*>(left) + rightvalue);
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

			int prec = value1->GetPrec();

                        if(static_cast<DecimalValue*>(right)->GetPrec() > prec)
                        {
				prec = static_cast<DecimalValue*>(right)->GetPrec();
                        }

                        result = new DecimalValue(*value1 + *value2);	
			static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new IntegerValue(value1->GetValue() + value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new DecimalValue(value1 + value2);
				static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
			}
			else
			{
				result = left->DupValue();
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new IntegerValue(value1.GetValue() + value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new DecimalValue(value1 + value2);
				static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new DecimalValue(*value1 + value2);
				static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				int prec = value1->GetPrec();

				if(static_cast<StringValue*>(right)->GetNumPrec() > prec)
				{
					prec = static_cast<StringValue*>(right)->GetNumPrec();
				}

				DecimalValue value2(right->toString());
                                result = new DecimalValue(*value1 + value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = left->DupValue();
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new DecimalValue(value1 + *value2);
				static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());

				int prec = leftvalue->GetNumPrec();

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
				if(value2->GetPrec() > prec)
                                {
                                        prec = value2->GetPrec();
                                }

                                result = new DecimalValue(value1 + *value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
                        result = new StringValue(left->toString() + right->toString());
		}
		else
		{
			result = new NullValue;
		}
		return result;
	}

	static ConstValue * SubOperation(ConstValue * left, ConstValue * right)
	{
		ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() - static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new DecimalValue(leftvalue - *static_cast<DecimalValue*>(right));
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new DecimalValue(*static_cast<DecimalValue*>(left) - rightvalue);
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

			int prec = value1->GetPrec();

                        if(static_cast<DecimalValue*>(right)->GetPrec() > prec)
                        {
				prec = static_cast<DecimalValue*>(right)->GetPrec();
                        }

                        result = new DecimalValue(*value1 - *value2);	
			static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new IntegerValue(value1->GetValue() - value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new DecimalValue(value1 - value2);
				static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
			}
			else
			{
				result = left->DupValue();
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new IntegerValue(value1.GetValue() - value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new DecimalValue(value1 - value2);
				static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new DecimalValue(*value1 - value2);
				static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				int prec = value1->GetPrec();

				if(static_cast<StringValue*>(right)->GetNumPrec() > prec)
				{
					prec = static_cast<StringValue*>(right)->GetNumPrec();
				}

				DecimalValue value2(right->toString());
                                result = new DecimalValue(*value1 - value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = left->DupValue();
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new DecimalValue(value1 - *value2);
				static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());

				int prec = leftvalue->GetNumPrec();

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
				if(value2->GetPrec() > prec)
                                {
                                        prec = value2->GetPrec();
                                }

                                result = new DecimalValue(value1 - *value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new IntegerValue(value1.GetValue() - value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new DecimalValue(value1 - value2);
                                static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new DecimalValue(value1 - value2);
                                static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new DecimalValue(value1 - value2);

				int prec = leftvalue->GetNumPrec();
				if(rightvalue->GetNumPrec() > prec)
				{
					prec = rightvalue->GetNumPrec();
				}
				static_cast<DecimalValue*>(result)->SetPrec(prec);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}

                return result;
	}

	static ConstValue * MulOperation(ConstValue * left, ConstValue * right)
	{
	        ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() * static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new DecimalValue(leftvalue * *static_cast<DecimalValue*>(right));
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new DecimalValue(*static_cast<DecimalValue*>(left) * rightvalue);
			static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

			int prec = value1->GetPrec() + value2->GetPrec();

                        result = new DecimalValue(*value1 * *value2);	
			static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new IntegerValue(value1->GetValue() * value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new DecimalValue(value1 * value2);
				static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
			}
			else
			{
				result = left->DupValue();
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new IntegerValue(value1.GetValue() * value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new DecimalValue(value1 * value2);
				static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new DecimalValue(*value1 * value2);
				static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				int prec = value1->GetPrec() + static_cast<StringValue*>(right)->GetNumPrec();

				DecimalValue value2(right->toString());
                                result = new DecimalValue(*value1 * value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = left->DupValue();
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new DecimalValue(value1 * *value2);
				static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

				int prec = leftvalue->GetNumPrec() + value2->GetPrec();

                                result = new DecimalValue(value1 * *value2);
				static_cast<DecimalValue*>(result)->SetPrec(prec);
                        }
                        else
                        {
                                result = right->DupValue();
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new IntegerValue(value1.GetValue() * value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new DecimalValue(value1 * value2);
                                static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new DecimalValue(value1 * value2);
                                static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new DecimalValue(value1 * value2);

				int prec = leftvalue->GetNumPrec() + rightvalue->GetNumPrec();
				static_cast<DecimalValue*>(result)->SetPrec(prec);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}

                return result;

	}

	static ConstValue * DivOperation(ConstValue * left, ConstValue * right)
	{
	        DecimalValue * result = NULL;

                DecimalValue value1(left->toString());
                DecimalValue value2(right->toString());

		result = new DecimalValue(value1 / value2);
		result->SetPrec(15);

                return result;
	}
	
	static ConstValue * GTOperation(ConstValue * left, ConstValue * right)
	{
		ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() > static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue > *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) > rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 > *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() > value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 > value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() > value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 > value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 > value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 > value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 > *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 > *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() > value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 > value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 > value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 > value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}

                return result;
	}
	static ConstValue * LTOperation(ConstValue * left, ConstValue * right)
	{
        	ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() < static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue < *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) < rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 < *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() < value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 < value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() < value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 < value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 < value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 < value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 < *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 < *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() < value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 < value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 < value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 < value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}


                return result;
	}
	static ConstValue * EQOperation(ConstValue * left, ConstValue * right)
	{
        	ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() == static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue == *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) == rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 == *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() == value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 == value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() == value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 == value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 == value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 == value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 == *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 == *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() == value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 == value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 == value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 == value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}


                return result;
	}
	static ConstValue * NEQOperation(ConstValue * left, ConstValue * right)
        {
                ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() != static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue != *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) != rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 != *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() != value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 != value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() != value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 != value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 != value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 != value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 != *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 != *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() != value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 != value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 != value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 != value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}


                return result;
        }
        static ConstValue * GEOperation(ConstValue * left, ConstValue * right)
        {
                ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() >= static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue >= *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) >= rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 >= *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() >= value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 >= value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() >= value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 >= value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 >= value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 >= value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 >= *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 >= *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() >= value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 >= value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 >= value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 >= value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}


                return result;
        }
        static ConstValue * LEOperation(ConstValue * left, ConstValue * right)
        {
                ConstValue * result = NULL;

		if(left->GetType()==Integer && right->GetType()==Integer)
		{
			result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() <= static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Integer && right->GetType()==Decimal)
                {
			DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                        result = new BooleanValue(leftvalue <= *static_cast<DecimalValue*>(right));
                }
		else if(left->GetType()==Decimal && right->GetType()==Integer)
		{
			DecimalValue rightvalue(static_cast<IntegerValue*>(right));
			result = new BooleanValue(*static_cast<DecimalValue*>(left) <= rightvalue);
		}
		else if(left->GetType()==Decimal && right->GetType()==Decimal)
                {
			DecimalValue * value1 = static_cast<DecimalValue*>(left);
			DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(*value1 <= *value2);	
                }
		else if(left->GetType()==Integer && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

			if(rightvalue->LooksLikeInteger())
                        {
				IntegerValue * value1 = static_cast<IntegerValue*>(left);
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1->GetValue() <= value2.GetValue());
                        }
			else if(rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(static_cast<IntegerValue*>(left));
				DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 <= value2);
			}
			else
			{
				result = new NullValue;
			}
		}
		else if(left->GetType()==String && right->GetType()==Integer)
                {
                        StringValue * leftvalue = static_cast<StringValue*>(left);
			if(leftvalue->LooksLikeInteger())
                        {
				IntegerValue value1(leftvalue->toString());
                                IntegerValue * value2 = static_cast<IntegerValue*>(right);
                                result = new BooleanValue(value1.GetValue() <= value2->GetValue());
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue value2(static_cast<IntegerValue*>(right));

                                result = new BooleanValue(value1 <= value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==Decimal && right->GetType()==String)
		{
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(rightvalue->LooksLikeInteger())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 <= value2);
                        }
                        else if(rightvalue->LooksLikeDecimal())
                        {
				DecimalValue * value1 = static_cast<DecimalValue*>(left);
				DecimalValue value2(right->toString());

                                result = new BooleanValue(*value1 <= value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
                }
		else if(left->GetType()==String && right->GetType()==Decimal)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);

                        if(leftvalue->LooksLikeInteger())
                        {
				DecimalValue value1(left->toString());

				DecimalValue * value2 = static_cast<DecimalValue*>(right);
                                result = new BooleanValue(value1 <= *value2);
                        }
                        else if(leftvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
				DecimalValue * value2 = static_cast<DecimalValue*>(right);

                                result = new BooleanValue(value1 <= *value2);
                        }
                        else
                        {
                                result = new NullValue;
                        }
		}
		else if(left->GetType()==String && right->GetType()==String)
		{
			StringValue * leftvalue = static_cast<StringValue*>(left);
			StringValue * rightvalue = static_cast<StringValue*>(right);

                        if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                        {
                                IntegerValue value1(leftvalue->toString());
                                IntegerValue value2(rightvalue->toString());
                                result = new BooleanValue(value1.GetValue() <= value2.GetValue());
                        }
                        else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 <= value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                        {
                                DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

                                result = new BooleanValue(value1 <= value2);
                        }
			else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
			{
				DecimalValue value1(leftvalue->toString());
                                DecimalValue value2(rightvalue->toString());

				result = new BooleanValue(value1 <= value2);
			}
                        else
                        {
                                result = new NullValue;
                        }
		}
		else
		{
			result = new NullValue;
		}


                return result;
        }
	static ConstValue * ANDOperation(ConstValue * left, ConstValue * right)
        {
                ConstValue * result = NULL;

                if(left->GetType()==Boolean && right->GetType()==Boolean)
                {
                        result = new BooleanValue(static_cast<BooleanValue*>(left)->GetValue() && static_cast<BooleanValue*>(right)->GetValue());
                }
		else
		{
			result = new NullValue;
		}
                return result;
        }
        static ConstValue * OROperation(ConstValue * left, ConstValue * right)
        {
                ConstValue * result = NULL;

                if(left->GetType()==Boolean && right->GetType()==Boolean)
                {
                        result = new BooleanValue(static_cast<BooleanValue*>(left)->GetValue() || static_cast<BooleanValue*>(right)->GetValue());
                }
		else
		{
			result = new NullValue;
		}
                return result;
        }
        static ConstValue * NOTOperation(ConstValue * value)
        {
                ConstValue * result = NULL;

                if(value->GetType()==Boolean)
                {
                        result = new BooleanValue(!static_cast<BooleanValue*>(value)->GetValue());
                }
		else
		{
			result = new NullValue;
		}
                return result;
        }
};
#endif
