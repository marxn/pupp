#ifndef _CONSTVAL_H_
#define _CONSTVAL_H_

#include <map>
#include <string>
#include <stdio.h>
#include "puppybase.h"

using namespace std;

enum DataType
{
        Null = 0, Integer, Float, Boolean, String, KeyValue, Set
};

class ConstValue: public PuppyObject
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

class IntegerValue: public ConstValue
{
public:
	IntegerValue():Value(0), ConstValue(Integer){}
	IntegerValue(long value):Value(value), ConstValue(Integer){}
	ConstValue * DupValue()
	{
		return new IntegerValue(this->Value);
	}
	long GetValue() {return this->Value;}
	string toString()
	{
		char buf[128] = "0";
		snprintf(buf, sizeof(buf), "%ld", this->Value);
		return string(buf);
	}
protected:
	long Value;
};

class FloatValue: public ConstValue
{
public:
	FloatValue():Value(0.0f), ConstValue(Float){}
	FloatValue(double value):Value(value), ConstValue(Float){}
        ConstValue * DupValue()
        {
                return new FloatValue(this->Value);
        }

	double GetValue() {return this->Value;}
        string toString()
        {
                char buf[128] = "0";
                snprintf(buf, sizeof(buf), "%lf", this->Value);
                return string(buf);
        }

protected:
	double Value;
};

class BooleanValue: public ConstValue
{
public:
	BooleanValue():Value(false), ConstValue(Boolean){}
	BooleanValue(bool value):Value(value), ConstValue(Boolean){}
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

class StringValue: public ConstValue
{
public:
	StringValue():ConstValue(String){}
	StringValue(string value):Value(value), ConstValue(String){}
        ConstValue * DupValue()
        {
                return new StringValue(this->Value);
        }

	string GetValue() {return this->Value;}
        string toString()
        {
                return this->Value;
        }

protected:
	string Value;
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
		string ret = "<'";
		ret.append(this->Value.first->toString());
		ret.append("',");
		if(Value.second->GetType()==String)
		{
			ret.append("'");
		}
		ret.append(this->Value.second->toString());
		if(Value.second->GetType()==String)
                {
                        ret.append("'");
                }
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
		map<string, ConstValue*>::iterator i;
		for(i=this->Value.begin(); i!=this->Value.end(); i++)
		{
			ret.append("<'");
			ret.append(i->first);
			ret.append("',");
			if((*i).second->GetType()==String)
			{
				ret.append("'");
			}
			ret.append(i->second->toString());
			if((*i).second->GetType()==String)
                        {
                                ret.append("'");
                        }
			ret.append(">");
		}
		ret.append("}");
                return ret;
        }

protected:
        map<string, ConstValue*> Value;
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
		else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() + static_cast<FloatValue*>(right)->GetValue());
                }
		else if(left->GetType()==Float && right->GetType()==Integer)
		{
			result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() + static_cast<IntegerValue*>(right)->GetValue());
		}
		else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() + static_cast<FloatValue*>(right)->GetValue());
                }
		else
		{
			//TODO
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() - static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() - static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() - static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() * static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() * static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() * static_cast<FloatValue*>(right)->GetValue());
                }
		else
		{
			result = new NullValue;
		}
                return result;

	}

	static ConstValue * DivOperation(ConstValue * left, ConstValue * right)
	{
	        ConstValue * result = NULL;
                if(left->GetType()==Integer && right->GetType()==Integer)
                {
                        result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() / static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() / static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() / static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() / static_cast<FloatValue*>(right)->GetValue());
                }
		else
		{
			result = new NullValue;
		}
                return result;
	}
	
	static ConstValue * GTOperation(ConstValue * left, ConstValue * right)
	{
		ConstValue * result = NULL;
                if(left->GetType()==Integer && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() > static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() > static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() > static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() > static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() < static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() < static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() < static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() == static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() == static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() == static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() != static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() != static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() != static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() >= static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() >= static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() >= static_cast<FloatValue*>(right)->GetValue());
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
                else if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() <= static_cast<FloatValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() <= static_cast<IntegerValue*>(right)->GetValue());
                }
                else if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() <= static_cast<FloatValue*>(right)->GetValue());
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
