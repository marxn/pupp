#ifndef _PUPP_FUNCTION_H_
#define _PUPP_FUNCTION_H_

#include "constval.h"
#include "container.h"

class FuncArgDef
{
public:
        FuncArgDef(std::string name, DataType type, bool isref);
        bool isRef();
        std::string GetName();
        DataType GetType();
        void SetElementType(DataType type);
        DataType GetElementType();
private:
        std::string Name;
        DataType Type;
        DataType ElementType;
        bool isref;
};

class ClosureVarDesc
{
public:
        ClosureVarDesc(std::string name, bool b);

        std::string GetVarName();
        bool IsRef();
private:
        std::string Name;
        bool isRef;
};

class FunctionNode:public ContainerNode
{
public:
        FunctionNode(std::string name);
        int Invoke(NodeContext * context);
        int Run(NodeContext * context);
        
        void SetRtnType(DataType type);
        void SetArgList(std::list<FuncArgDef*> * arglist);
        std::list<FuncArgDef*> * GetArgList();
        void SetCopyList(std::list<ClosureVarDesc*> * copylist);
        std::list<ClosureVarDesc*> * GetCopyList();
        std::string GetName();
        void SetName(std::string name);
        DataType GetRtnType();
        bool Provision();

private:
        std::string Name;
        ConstValue * ReturnVal;
        DataType RtnType;
        std::list<FuncArgDef*> * ArgList;
        std::list<ClosureVarDesc*> * CopyList;
};

#endif

