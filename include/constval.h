#ifndef _CONSTVAL_H_
#define _CONSTVAL_H_

#include <string>
#include <stdio.h>

using namespace std;

enum DataType
{
        UnknownDataType = 0, Integer, Float, Boolean, String, DataTypeSize
};

class ConstValue
{
public:
	ConstValue():Type(UnknownDataType){}
	ConstValue(DataType type):Type(type){}
	DataType GetType() {return this->Type;}
	virtual string toString() = 0;
protected:
	DataType Type;
};

class IntegerValue: public ConstValue
{
public:
	IntegerValue():Value(0), ConstValue(Integer){}
	IntegerValue(long value):Value(value), ConstValue(Integer){}
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
	double GetValue() {return this->Value;}
        string toString()
        {
                //char buf[128] = "0";
                //snprintf(buf, sizeof(buf), "%lf", this->Value);
                return string("456");
        }

protected:
	double Value;
};

class BooleanValue: public ConstValue
{
public:
	BooleanValue():Value(false), ConstValue(Boolean){}
	BooleanValue(bool value):Value(value), ConstValue(Boolean){}
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
	string GetValue() {return this->Value;}
        string toString()
        {
                return this->Value;
        }

protected:
	string Value;
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
		if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() + static_cast<FloatValue*>(right)->GetValue());
                }
		if(left->GetType()==Float && right->GetType()==Integer)
		{
			result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() + static_cast<IntegerValue*>(right)->GetValue());
		}
		if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() + static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() - static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() - static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() - static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() * static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() * static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() * static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<IntegerValue*>(left)->GetValue() / static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() / static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new FloatValue(static_cast<FloatValue*>(left)->GetValue() / static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() > static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() > static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() > static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() < static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() < static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() < static_cast<FloatValue*>(right)->GetValue());
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
                if(left->GetType()==Integer && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() == static_cast<FloatValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Integer)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() == static_cast<IntegerValue*>(right)->GetValue());
                }
                if(left->GetType()==Float && right->GetType()==Float)
                {
                        result = new BooleanValue(static_cast<FloatValue*>(left)->GetValue() == static_cast<FloatValue*>(right)->GetValue());
                }
                return result;
	}
};
#endif
