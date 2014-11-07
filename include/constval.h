#ifndef _CONSTVAL_H_
#define _CONSTVAL_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>

using namespace std;

class ConstValue
{
public:
	enum ValueType{Unknown = 0, Integer, Float, String, Boolean};
	ConstValue():Type(Unknown)
	{
	}
	
	ConstValue(int value)
	{
		this->Type = Integer;
		this->IntegerValue = value;
	}
	ConstValue(float value)
	{
		this->Type = Float;
		this->FloatValue = value;
	}
	ConstValue(string value)
	{
		this->Type = String;
		this->StringValue = value;
	}
	ConstValue(bool value)
	{
		this->Type = Boolean;
		this->StringValue = value;
	}

	void Set(int value)
	{
		this->Type = Integer;
		this->IntegerValue = value;
	}
	void Set(float value)
	{
		this->Type = Float;
		this->FloatValue = value;
	}
	void Set(string value)
	{
		this->Type = String;
		this->StringValue = value;
	}
	void Set(bool value)
	{
		this->Type = Boolean;
		this->BoolValue = value;
	}

	ValueType GetType()
	{
		return this->Type;
	}
	int GetInteger()
	{
		return this->IntegerValue;
	}
	float GetFloat()
	{
		return this->FloatValue;
	}
	string GetString()
	{
		return this->StringValue;
	}
	bool GetBoolean()
	{
		return this->BoolValue;
	}

	string toString()
	{
		if(this->Type == Integer)
			return IntegerToString();
		
		else if(this->Type == Float)
			return FloatToString();
		
		else if(this->Type == String)
			return this->StringValue;
		
		else if(this->Type == Boolean)
			return BooleanToString();

		return "Unknown";
	}
	
	ConstValue operator + (ConstValue value)
	{
		ConstValue temp;

		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue + value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue + value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue + value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue + value.FloatValue);

		else if(this->Type == String && value.Type == String)
			temp.Set(this->StringValue + value.StringValue);

		return temp;
	}

	ConstValue operator - (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue - value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue - value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue - value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue - value.FloatValue);

		return temp;
	}

	ConstValue operator * (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue * value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue * value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue * value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue * value.FloatValue);

		return temp;
	}

	ConstValue operator / (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue / value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue / value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue / value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue / value.FloatValue);

		return temp;
	}

	ConstValue operator > (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue > value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue > value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue > value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue > value.FloatValue);

		return temp;
	}

	ConstValue operator < (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue < value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue < value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue < value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue < value.FloatValue);

		return temp;
	}

	ConstValue operator == (ConstValue value)
	{
		ConstValue temp;
		if(this->Type == Integer && value.Type == Integer)
			temp.Set(this->IntegerValue == value.IntegerValue);

		else if(this->Type == Integer && value.Type == Float)
			temp.Set(this->IntegerValue == value.FloatValue);

		else if(this->Type == Float && value.Type == Integer)
			temp.Set(this->FloatValue == value.IntegerValue);

		else if(this->Type == Float && value.Type == Float)
			temp.Set(this->FloatValue == value.FloatValue);

		return temp;
	}


private:
	ValueType Type;
	
	union
	{
		int IntegerValue;
		float FloatValue;
		bool BoolValue;
	};

	string StringValue;

	string IntegerToString()
	{
		ostringstream rtn;
		rtn<<this->IntegerValue;
		return rtn.str(); 
	}

	string FloatToString()
	{
		ostringstream rtn;
		rtn<<this->FloatValue;
		return rtn.str(); 
	}

	string BooleanToString()
	{
		ostringstream rtn;
		rtn<<this->BoolValue;
		return rtn.str(); 
	}

};

#endif
