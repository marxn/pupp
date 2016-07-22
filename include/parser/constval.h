#ifndef _CONSTVAL_H_
#define _CONSTVAL_H_

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

using namespace std;

enum DataType
{
        Null = 0, Integer, Decimal, Boolean, String, KeyValue, Set, Array, Func
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

class ValueBox
{
public:
        ValueBox():Value(NULL), RefCount(1){}
        ValueBox(ConstValue * value):Value(NULL), RefCount(1)
        {
                if(value!=NULL)
                {
                        this->Value = value->DupValue();
                }
        }
        ~ValueBox()
        {
                this->Clear();
        }
        void IncRefCount()
        {
                this->RefCount++;
        }
        void Destroy()
        {
                if(this->RefCount==1)
                {
                        delete this;
                }
                else
                {
                        this->RefCount--;
                }
        }
        void SetVal(ConstValue * val)
        {
                if(this->Value!=NULL)
                {
                        delete this->Value;
                }
                this->Value = val;
        }
        ConstValue * GetVal()
        {
                return this->Value;
        }
        void Clear()
        {
                delete this->Value;
                this->Value = NULL;
        }
        ValueBox * Dup()
        {
                return new ValueBox(this->Value);
        }
private:
        ConstValue * Value;
        unsigned long RefCount;
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

class Node;

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
        void SetPrec(long n)
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
                char result[1024];
                gmp_snprintf (result, sizeof(result), "%.*Ff", this->Prec, this->Value);
                return result;
        }
        mpf_t Value;

protected:
        long Prec;
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
        KVValue(ConstValue * key, ValueBox* valuebox):ConstValue(KeyValue)
        {
                this->Key   = key->DupValue();
                this->Value = valuebox->Dup();
        }
        ~KVValue()
        {
                delete this->Key;
                this->Value->Destroy();
        }
        ConstValue * DupValue()
        {
                return new KVValue(this->Key, this->Value);
        }

        ConstValue * GetKey()
        {
                return this->Key;
        }
        ValueBox * GetValueBox()
        {
                return this->Value;
        }

        string toString()
        {
                string ret = "<";
                ret.append(this->Key->toString());
                ret.append(",");
                ret.append(this->Value->GetVal()->toString());
                ret.append(">");
                return ret;
        }

protected:
        ConstValue * Key;
        ValueBox * Value;
};

class SetValue: public ConstValue
{
public:
        SetValue():ConstValue(Set){}
        SetValue(map<string, ValueBox*>& value):ConstValue(Set)
        {
                map<string, ValueBox*>::iterator i;
                for(i=value.begin();i!=value.end();i++)
                {
                        this->Value.insert(pair<string, ValueBox*>(i->first, i->second->Dup()));
                }
        }
        ~SetValue()
        {
                map<string, ValueBox*>::iterator i;
                for(i=this->Value.begin();i!=this->Value.end();i++)
                {
                        i->second->Destroy();
                }
                this->Value.clear();
        }
        void AddKV(KVValue * kv)
        {
                string key = kv->GetKey()->toString();
                map<string, ValueBox*>::iterator i = this->Value.find(key);

                if(i!=this->Value.end())
                {
                        i->second->Destroy();
                        this->Value.erase(key);
                }
                this->Value.insert(pair<string, ValueBox*>(key, kv->GetValueBox()->Dup()));
        }
        void RemoveKV(string key)
        {
                this->Value.erase(key);
        }

        ValueBox * FindByKey(string key)
        {
                map<string, ValueBox*>::iterator i = this->Value.find(key);
                if(i!=this->Value.end())
                {
                        return i->second;
                }
                return NULL;
        }
        map<string, ValueBox*> * GetValue()
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
                map<string, ValueBox*>::iterator i;
                for(i=this->Value.begin(); i!=this->Value.end(); i++, size--)
                {
                        ret.append("<");
                        ret.append(i->first);
                        ret.append(",");
                        ret.append(i->second->GetVal()->toString());
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
        map<string, ValueBox*> Value;
};

class DefaultValueFactory
{
public:
        DefaultValueFactory(DataType type, long prec):Type(type), Prec(prec)
        {
        }

        ConstValue * GetValue()
        {
                ConstValue * result = NULL;
                switch(this->Type)
                {
                        case Integer:
                                result = new IntegerValue(0);
                        break;
                        case String:
                                result = new StringValue("");
                        break;
                        case Boolean:
                                result = new BooleanValue(false);
                        break;
                        case Decimal:
                                result = new DecimalValue("0");
                                static_cast<DecimalValue*>(result)->SetPrec(this->Prec);
                        break;
                        case Set:
                                result = new SetValue;
                        break;
                        default:
                                result = new NullValue;
                }
                return result;
        }

private:
        DataType Type;
        long Prec;
};

class ArrayValue: public ConstValue
{
public:
        ArrayValue(DataType type, vector<long>& desc, long size, long prec):ConstValue(Array)
        {
                DefaultValueFactory fac(type, prec);

                this->ElementType = type;
                this->Value = new ValueBox*[size];
                this->Dim = desc;
                this->Size = size;
                this->Prec = prec;

                for(int i = 0; i < size; i++)
                {
                        this->Value[i] = new ValueBox;
                        this->Value[i]->SetVal(fac.GetValue());
                }
        }
        ~ArrayValue()
        {
                for(int i = 0; i < this->Size; i++)
                {
                        if(this->Value[i]!=NULL)
                        {
                                this->Value[i]->Destroy();
                        }
                }

                delete this->Value;
        }
        ValueBox * GetElementBox(vector<long>& keys)
        {
                int i = 0;
                int j = 0;

                if(keys.size()!=this->Dim.size())
                {
                        return NULL;
                }
                for(int i = 0; i < keys.size(); i++)
                {
                        if(keys[i]<0 || keys[i]>=this->Dim[i])
                        {
                                return NULL;
                        }
                }

                long index = 0;

                for(i = 0; i < keys.size() - 1; i++)
                {
                        long weight = 1;
                        for(j = i + 1; j< keys.size(); j++)
                        {
                                weight *= this->Dim[j];
                        }

                        index += keys[i] * weight;
                }

                index+=keys[i];
                return this->Value[index];
        }
        ConstValue * GetElementValue(vector<long>& keys)
        {
                ValueBox * box = this->GetElementBox(keys);
                return box->GetVal()->DupValue();
        }
        void SetElementValue(vector<long>& keys, ConstValue * value)
        {
                ValueBox * box = this->GetElementBox(keys);
                box->SetVal(value);
        }

        void SetValue(ValueBox ** values)
        {
                for(int i = 0; i < this->Size; i++)
                {
                        this->Value[i]->Destroy();
                        this->Value[i] = values[i]->Dup();
                }
        }
        DataType GetElementType()
        {
                return this->ElementType;
        }
        string toString()
        {
                string ret="{";

                int size = this->Size;
                for(int i = 0; i < size; i++)
                {
                        if(this->Value[i]->GetVal()!=NULL)
                        {
                                ret.append(this->Value[i]->GetVal()->toString());
                        }
                        else
                        {
                                ret.append("nil");
                        }
                        if(i<size-1)
                        {
                                ret.append(",");
                        }
                }
                ret.append("}");
                return ret;
        }
        ConstValue * DupValue()
        {
                ArrayValue * result = new ArrayValue(this->ElementType, this->Dim, this->Size, this->Prec);
                result->SetValue(this->Value);
                return result;
        }
        long GetDimensionNum()
        {
                return this->Dim.size();
        }
        long GetPrecision()
        {
                return this->Prec;
        }
private:
        DataType ElementType;
        ValueBox ** Value;
        vector<long> Dim;
        long Size;
        long Prec;
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
                }
                return NULL;
        }
private:
        ConstValue * Value;
        DataType Type;
};

#endif
