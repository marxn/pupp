#ifndef _NODE_H_
#define _NODE_H_

#include <stack>
#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include "variable.h"
#include "constval.h"

#define NODE_RET_NORMAL       0
#define NODE_RET_ERROR       -1
#define NODE_RET_NEEDBREAK    1
#define NODE_RET_NEEDCONTINUE 2
#define NODE_RET_NEEDRETURN   3

#define NODE_RET_NEEDROLLBACK 4
#define NODE_RET_NEEDCOMMIT   5

#define EVA_TRUE 1
#define EVA_FALSE 0
#define EVA_ERROR -1

class VariableDef;
class Variable;
class NodeContext;

enum NodeType
{
        Generic,
        Function,
        IfElse,
        Loop,
        Transaction
};

class Node
{
public:
        Node();
        Node(NodeType type);
        virtual ~Node();

        int Execute(NodeContext * context);

        Node * GetParentNode();
        void SetParentNode(Node * node);

        virtual bool Provision() = 0;
        virtual bool Check();

        virtual int Invoke(NodeContext * context) = 0;
        virtual void Swipe(NodeContext * context) = 0;

        void AddVariable(VariableDef * var);
        VariableDef * FindVariable(std::string varname, unsigned long * layer);
        
        NodeType Type;
        Node * ParentNode;
        
        std::map<std::string, VariableDef*> VariableDefTable;
        std::vector<VariableDef*> VarDefArray;
};

struct ForeachLoopCtx 
{
        ConstValue * Keeper;
        std::map<std::string, ValueBox*> * SetValueHolder;
        std::map<std::string, ValueBox*>::iterator ValueHandle;
};

class NodeContext
{
public:
        NodeContext();
        ~NodeContext();

        void AddFrame(Node * snapshot);

        void PopFrame();
        void ReplaceVariable(unsigned long start_index, std::list<Variable*> * list);

        Variable * GetVariable(unsigned long layer, unsigned long index);
        
        Variable * GetVariableFromOuterLayer(std::string name);

        std::stack<ForeachLoopCtx*> ForeachCtx;
        ConstValue * FunctionRet;

private:
        std::vector<std::vector<Variable*> > VarTable;
};

#endif
