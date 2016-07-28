#include "parser/node.h"
#include "parser/variable.h"

using namespace std;

Variable::Variable(string name, DataType type)
{
        this->Source  = NULL;
        this->VBox    = NULL;
        this->VarName = name;
        this->VarType = type;
        this->Prec    = -1;
}

Variable::~Variable()
{
        if(this->VBox)
        {
                this->VBox->Destroy();
        }
}

string Variable::GetVarName()
{
        return this->VarName;
}

void Variable::SetSource(VariableDef * def)
{
        this->Source = def;
}

VariableDef * Variable::GetSource()
{
        return this->Source;
}

void Variable::SetVBox(ValueBox * box)
{
        this->VBox = box;
}

ValueBox * Variable::GetVBox()
{
        return this->VBox;
}

void Variable::SetRef(ConstValue * ref)
{
        if(this->VBox==NULL)
        {
                this->VBox = new ValueBox;
        }

        this->VBox->SetVal(ref);
}

void Variable::SetValue(ConstValue * value)
{
        if(this->VBox==NULL)
        {
                this->VBox = new ValueBox;
        }

        if(this->VarType==value->GetType())
        {
                ConstValue * val = value->DupValue();

                if(this->VarType==Decimal && this->Prec!=-1)
                {
                        static_cast<DecimalValue*>(val)->SetPrec(this->Prec);
                }

                this->VBox->SetVal(val);
        }
        else
        {
                ConstValueCaster caster(value, this->VarType);
                ConstValue * newvalue = caster.Cast();

                if(this->VarType==Decimal && this->Prec!=-1)
                {
                        static_cast<DecimalValue*>(newvalue)->SetPrec(this->Prec);
                }

                this->VBox->SetVal(newvalue);
        }
}

ConstValue * Variable::GetValue()
{
        ConstValue * value = NULL;

        if(this->VBox==NULL)
        {
                this->VBox = new ValueBox;
                DefaultValueFactory defvalue(this->VarType, this->Prec);
                value = defvalue.GetValue();
                this->VBox->SetVal(value);
        }
        else
        {
                value = this->VBox->GetVal()->DupValue();
        }

        return value;
}

void Variable::SetVarType(DataType type)
{
        this->VarType = type;
}

DataType Variable::GetVarType()
{
        return this->VarType;
}

DataType Variable::GetValueType()
{
        if(this->VBox==NULL)
        {
                this->VBox = new ValueBox;
                DefaultValueFactory defvalue(this->VarType, this->Prec);
                ConstValue * value = defvalue.GetValue();
                this->VBox->SetVal(value);
        }

        return this->VBox->GetVal()->GetType();
}

void Variable::SetPrecision(long prec)
{
        this->Prec = prec;
}

Variable * Variable::CreateVarRef()
{
        Variable * ret = new Variable(this->VarName, this->VarType);
        ret->SetSource(this->Source);
        ret->SetPrecision(this->Prec);

        if(this->VBox!=NULL)
        {
                this->VBox->IncRefCount();
                ret->SetVBox(this->VBox);
        }
        return ret;
}

VariableDef::VariableDef(string varname)
{
        this->VarName = varname;
        this->VarType = Null;
        this->VarPrec = -1;
}

Variable * VariableDef::GetInstance()
{
        Variable * ret = new Variable(this->VarName, this->VarType);
        ret->SetSource(this);
        ret->SetPrecision(this->VarPrec);

        ConstValue * value = new NullValue;
        ret->SetRef(value);

        return ret;
}

void VariableDef::SetVarType(DataType type)
{
        this->VarType = type;
}

void VariableDef::SetVarPrec(long prec)
{
        this->VarPrec = prec;
}

DataType VariableDef::GetVarType()
{
        return this->VarType;
}

string VariableDef::GetVarName()
{
        return VarName;
}

void VariableDef::SetAttachedNode(Node * node)
{
        this->AttachedNode = node;
}

Node * VariableDef::GetAttachedNode()
{
        return this->AttachedNode;
}


