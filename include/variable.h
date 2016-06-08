#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "constval.h"

using namespace std;

class Node;
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
                this->Prec = -1;
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
                return this->VarName;
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

        void SetRef(ConstValue * ref)
        {
                if(this->VBox==NULL)
                {
                        this->VBox = new ValueBox;
                }

                this->VBox->SetVal(ref);
        }

        void SetValue(ConstValue * value)
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

        ConstValue * GetValue()
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

        void SetVarType(DataType type)
        {
                this->VarType = type;
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
                        DefaultValueFactory defvalue(this->VarType, this->Prec);
                        ConstValue * value = defvalue.GetValue();
                        this->VBox->SetVal(value);
                }

                return this->VBox->GetVal()->GetType();
        }

        void SetPrecision(long prec)
        {
                this->Prec = prec;
        }

        Variable * CreateVarRef()
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

private:
        VariableDef * Source;
        DataType VarType;
        string VarName;
        ValueBox * VBox;
        long Prec;
};

class VariableDef
{
public:
        VariableDef(string varname):VarName(varname), VarType(Null), VarPrec(-1), IsUsedByInnerNode(false), IsActualParameter(false)
        {
        }

        Variable * GetInstance()
        {
                Variable * ret = new Variable(this->VarName, this->VarType);
                ret->SetSource(this);

                ConstValue * value = new NullValue;
                ret->SetRef(value);

                return ret;
        }

        void SetVarType(DataType type)
        {
                this->VarType = type;
        }

        void SetVarPrec(long prec)
        {
                this->VarPrec = prec;
        }

        DataType GetVarType()
        {
                return this->VarType;
        }

        string GetVarName()
        {
                return VarName;
        }

        bool UsedByInnerNode()
        {
                return this->IsUsedByInnerNode;
        }

        void SetUsedByInnerNode(bool u)
        {
                this->IsUsedByInnerNode = u;
        }

        bool ActualParameter()
        {
                return this->IsActualParameter;
        }

        void SetIsActualParameter(bool u)
        {
                this->IsActualParameter = u;
        }

        void SetAttachedNode(Node * node)
        {
                this->AttachedNode = node;
        }

        Node * GetAttachedNode()
        {
                return this->AttachedNode;
        }
private:
        string VarName;
        DataType VarType;
        long VarPrec;
        bool IsUsedByInnerNode;
        bool IsActualParameter;
        Node * AttachedNode;
};

#endif

