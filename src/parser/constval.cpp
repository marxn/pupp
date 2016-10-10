#include "parser/constval.h"
#include "parser/node.h"
#include "parser/closure.h"

using namespace std;

ConstValue::ConstValue()
{
        this->Type = Null;
}

ConstValue::ConstValue(DataType type)
{
        this->Type = type;
}

ConstValue::~ConstValue()
{
}

DataType ConstValue::GetType() 
{
        return this->Type;
}

ValueBox::ValueBox()
{
        this->Value    = NULL;
        this->RefCount = 1;
}

ValueBox::ValueBox(ConstValue * value)
{
        this->Value    = NULL; 
        this->RefCount = 1;
        if(value!=NULL)
        {
                this->Value = value->DupValue();
        }
}

ValueBox::~ValueBox()
{
        this->Clear();
}

void ValueBox::IncRefCount()
{
        this->RefCount++;
}

void ValueBox::Destroy()
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

void ValueBox::SetVal(ConstValue * val)
{
        if(this->Value!=NULL)
        {
                delete this->Value;
        }
        this->Value = val;
}

ConstValue * ValueBox::GetVal()
{
        return this->Value;
}

void ValueBox::Clear()
{
        delete this->Value;
        this->Value = NULL;
}
ValueBox * ValueBox::Dup()
{
        return new ValueBox(this->Value);
}


NullValue::NullValue():ConstValue(Null)
{
}

ConstValue * NullValue::DupValue()
{
        return new NullValue;
}

string NullValue::toString()
{
        return "Null";
}


StringValue::StringValue():ConstValue(String)
{
}

StringValue::StringValue(string value):ConstValue(String)
{
        this->Value = value;
}

ConstValue * StringValue::DupValue()
{
        return new StringValue(this->Value);
}

bool StringValue::LooksLikeInteger()
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

bool StringValue::LooksLikeDecimal()
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

string StringValue::toString()
{
        return this->Value;
}



IntegerValue::IntegerValue(long value):ConstValue(Integer)
{
        this->Value = value;
}

IntegerValue::IntegerValue(string value): ConstValue(Integer)
{
        this->Value = atol(value.c_str());
}

IntegerValue::IntegerValue(IntegerValue& value): ConstValue(Integer)
{
        this->Value = value.GetValue();
}

long IntegerValue::GetValue()
{
        return this->Value;
}
void IntegerValue::SetValue(long n)
{
        this->Value = n;
}
ConstValue * IntegerValue::DupValue()
{
        return new IntegerValue(this->Value);
}
string IntegerValue::toString()
{
        char buf[256] = "0";
        snprintf(buf, sizeof(buf), "%ld", this->Value);
        return string(buf);
}

DecimalValue::DecimalValue():ConstValue(Decimal)
{
        mpf_init_set_ui(this->Value,0);
}

DecimalValue::DecimalValue(string valuestr):ConstValue(Decimal)
{
        StringValue temp(valuestr);
        mpf_init_set_str(this->Value,valuestr.c_str(),10);
}

DecimalValue::DecimalValue(long value):ConstValue(Decimal)
{
        mpf_init_set_si(this->Value,value);
}

DecimalValue::DecimalValue(IntegerValue * value):ConstValue(Decimal)
{
        mpf_init_set_si(this->Value,value->GetValue());
}

DecimalValue::DecimalValue(const DecimalValue& value):ConstValue(Decimal)
{
        mpf_init(this->Value);
        mpf_set(this->Value, value.Value);
}

DecimalValue::DecimalValue(DecimalValue * value):ConstValue(Decimal)
{
        mpf_init(this->Value);
        mpf_set(this->Value, value->Value);
}

DecimalValue& DecimalValue::operator = (const DecimalValue& value)
{
        mpf_set(this->Value, value.Value);
}

DecimalValue::~DecimalValue()
{
        mpf_clear(this->Value);
}

ConstValue * DecimalValue::DupValue()
{
        ConstValue * result = new DecimalValue(this);
        return result;
}

DecimalValue DecimalValue::operator + (DecimalValue& element)
{
        DecimalValue ret;
        mpf_add(ret.Value, this->Value, element.Value);
        return ret;
}

DecimalValue DecimalValue::operator - (DecimalValue& element)
{
        DecimalValue ret;
        mpf_sub(ret.Value, this->Value, element.Value);
        return ret;
}

DecimalValue DecimalValue::operator * (DecimalValue& element)
{
        DecimalValue ret;
        mpf_mul(ret.Value, this->Value, element.Value);
        return ret;
}

DecimalValue DecimalValue::operator / (DecimalValue& element)
{
        DecimalValue ret;
        mpf_div(ret.Value, this->Value, element.Value);
        return ret;
}

bool DecimalValue::operator > (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)>0?true:false;
}

bool DecimalValue::operator < (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)<0?true:false;
}

bool DecimalValue::operator == (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)==0?true:false;
}

bool DecimalValue::operator >= (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)>=0?true:false;
}

bool DecimalValue::operator <= (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)<=0?true:false;
}

bool DecimalValue::operator != (DecimalValue& element)
{
        return mpf_cmp(this->Value, element.Value)!=0?true:false;
}

string DecimalValue::toString()
{
        char result[1024];
        int prec = mpf_get_prec(this->Value);
        gmp_snprintf (result, sizeof(result), "%.*Ff", prec, this->Value);
        
        char * p = strstr(result, ".");
        if(p==NULL)
        {
            return result;
        }
        
        int len = strlen(p) - 1;
        
        char * q = p + len;
        
        while(*q == '0' && *(q-1)!='.')
        {
            q--;
        }
        
        *(q+1) = '\0';
        
        return result;
}

BooleanValue::BooleanValue():ConstValue(Boolean)
{
        this->Value = false;
}

BooleanValue::BooleanValue(bool value):ConstValue(Boolean)
{
        this->Value = value;
}

BooleanValue::BooleanValue(string value)
{
        this->Value = (value=="true"?true:false);
}

ConstValue * BooleanValue::DupValue()
{
        return new BooleanValue(this->Value);
}

bool BooleanValue::GetValue() 
{
        return this->Value;
}

string BooleanValue::toString()
{
        if(this->Value) return "true";
        return "false";
}

KVValue::KVValue():ConstValue(KeyValue)
{
}

KVValue::KVValue(ConstValue * key, ValueBox* valuebox):ConstValue(KeyValue)
{
        this->Key   = key->DupValue();
        this->Value = valuebox->Dup();
}

KVValue::~KVValue()
{
        delete this->Key;
        this->Value->Destroy();
}

ConstValue * KVValue::DupValue()
{
        return new KVValue(this->Key, this->Value);
}

ConstValue * KVValue::GetKey()
{
        return this->Key;
}

ValueBox * KVValue::GetValueBox()
{
        return this->Value;
}

string KVValue::toString()
{
        string ret = "<";
        ret.append(this->Key->toString());
        ret.append(",");
        ret.append(this->Value->GetVal()->toString());
        ret.append(">");
        return ret;
}


SetValue::SetValue():ConstValue(Set)
{
}

SetValue::SetValue(map<string, ValueBox*>& value):ConstValue(Set)
{
        map<string, ValueBox*>::iterator i;
        for(i=value.begin();i!=value.end();i++)
        {
                this->Value.insert(pair<string, ValueBox*>(i->first, i->second->Dup()));
        }
}

SetValue::~SetValue()
{
        map<string, ValueBox*>::iterator i;
        for(i=this->Value.begin();i!=this->Value.end();i++)
        {
                i->second->Destroy();
        }
        this->Value.clear();
}

void SetValue::AddKV(KVValue * kv)
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

void SetValue::RemoveKV(string key)
{
        this->Value.erase(key);
}

ValueBox * SetValue::FindByKey(string key)
{
        map<string, ValueBox*>::iterator i = this->Value.find(key);
        if(i!=this->Value.end())
        {
                return i->second;
        }
        return NULL;
}

map<string, ValueBox*> * SetValue::GetValue()
{
        return &this->Value;
}        

ConstValue * SetValue::DupValue()
{
        return new SetValue(this->Value);
}

string SetValue::toString()
{
        string ret="[";

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
        ret.append("]");
        return ret;
}

DefaultValueFactory::DefaultValueFactory(DataType type)
{
        this->Type = type;
}

ConstValue * DefaultValueFactory::GetValue()
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
                break;
                case Set:
                        result = new SetValue();
                break;
                case Func:
                        result = new FuncValue(NULL, NULL);
                break;
                default:
                        result = new NullValue;
        }
        return result;
}

ArrayValue::ArrayValue(DataType type, vector<long>& desc, long size):ConstValue(Array)
{
        DefaultValueFactory fac(type);

        this->ElementType = type;
        this->Value = new ValueBox*[size];
        this->Dim = desc;
        this->Size = size;

        for(int i = 0; i < size; i++)
        {
                ConstValue * val = fac.GetValue();
                
                this->Value[i] = new ValueBox;
                this->Value[i]->SetVal(val);
        }
}

ArrayValue::~ArrayValue()
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

ValueBox * ArrayValue::GetElementBox(vector<long>& keys)
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

ConstValue * ArrayValue::GetElementValue(vector<long>& keys)
{
        ValueBox * box = this->GetElementBox(keys);
        return box->GetVal()->DupValue();
}

void ArrayValue::SetElementValue(vector<long>& keys, ConstValue * value)
{
        ValueBox * box = this->GetElementBox(keys);
        box->SetVal(value);
}

void ArrayValue::SetValue(ValueBox ** values)
{
        for(int i = 0; i < this->Size; i++)
        {
                this->Value[i]->Destroy();
                this->Value[i] = values[i]->Dup();
        }
}

DataType ArrayValue::GetElementType()
{
        return this->ElementType;
}

string ArrayValue::toString()
{
        string ret="[";

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
        ret.append("]");
        return ret;
}

ConstValue * ArrayValue::DupValue()
{
        ArrayValue * result = new ArrayValue(this->ElementType, this->Dim, this->Size);
        result->SetValue(this->Value);
        return result;
}

long ArrayValue::GetDimensionNum()
{
        return this->Dim.size();
}

ConstValueCaster::ConstValueCaster(ConstValue * value, DataType type)
{
        this->Value = value;
        this->Type = type;
}

ConstValue * ConstValueCaster::Cast()
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
                        {
                                string s = this->Value->toString();
                                long value = 0;
                                if(sscanf(s.c_str(), "%ld", &value)==0)
                                {
                                        return NULL;
                                }
                        }
                        
                        return new IntegerValue(this->Value->toString());
                break;
                case Boolean:
                        if(this->Value->GetType()!=Boolean)
                        {
                                string s = this->Value->toString();
                                if(s=="0" || s=="")
                                {
                                        return new BooleanValue(false);
                                }
                                
                                return new BooleanValue(true);
                        }
                        
                        return this->Value->DupValue();
                break;
        }
        
        return NULL;
}
