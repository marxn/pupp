#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>
#include "node.h"
#include "constval.h"

class Node;
class ConstValue;
class Variable;

class VariableDef
{
public:
        VariableDef(std::string varname);

        Variable * GetInstance();

        void SetVarType(DataType type);

        void SetVarPrec(long prec);

        DataType GetVarType();

        std::string GetVarName();

        void SetAttachedNode(Node * node);

        Node * GetAttachedNode();
        
        void SetVarIndex(unsigned long index);
        
        unsigned long GetVarIndex();

        bool NeedInstance();

        void SetNeedInstance(bool b);

private:
        std::string VarName;
        DataType VarType;
        long VarPrec;
        Node * AttachedNode;
        unsigned long VarIndex;
        bool needInstance;
};

class Variable
{
public:
        Variable(std::string name, DataType type);
        ~Variable();

        std::string GetVarName();

        void SetSource(VariableDef * def);

        VariableDef * GetSource();

        void SetVBox(ValueBox * box);

        ValueBox * GetVBox();

        void SetRef(ConstValue * ref);

        void SetValue(ConstValue * value);

        ConstValue * GetValue();

        void SetVarType(DataType type);

        DataType GetVarType();

        DataType GetValueType();

        void SetPrecision(long prec);

        Variable * CreateVarRef();

private:
        VariableDef * Source;
        DataType VarType;
        std::string VarName;
        ValueBox * VBox;
        long Prec;
};

#endif

