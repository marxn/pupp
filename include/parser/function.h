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
        void SetArgIndex(unsigned long index);
        unsigned long GetArgIndex();
private:
        std::string Name;
        DataType Type;
        DataType ElementType;
        bool isref;
        unsigned long ArgIndex;
};

class ClosureVarDesc
{
public:
        ClosureVarDesc(std::string name, bool b);

        std::string GetVarName();
        bool IsRef();
        
        void SetSrcLayer(unsigned long layer);
        void SetSrcDef(VariableDef * def);
        void SetDstDef(VariableDef * def);
        
        unsigned long GetSrcLayer();
        VariableDef * GetSrcDef();
        VariableDef * GetDstDef();
        
private:
        std::string Name;
        bool isRef;
        VariableDef * SrcDef;
        VariableDef * DstDef;
        unsigned long SrcLayer;
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
        unsigned long GetClosureVarStartIndex();
        bool Provision();

private:
        std::string Name;
        ConstValue * ReturnVal;
        DataType RtnType;
        std::list<FuncArgDef*> * ArgList;
        std::list<ClosureVarDesc*> * CopyList;
        unsigned long ClosureVarStartIndex;
};

#endif

