#ifndef _PUPP_LOOP_H_
#define _PUPP_LOOP_H_

#include <list>
#include "expression.h"
#include "container.h"

class LoopNode :public ContainerNode
{
public:
        LoopNode();

        virtual bool PreLoopStatement(NodeContext * context);
        virtual bool PerOnceStatement(NodeContext * context);

        int Invoke(NodeContext * context);

        void SetCondition(Expression * condition);

        bool Provision();
        bool Check();

private:
        //1 - true, 0 - false, -1 - error
        virtual int Evaluate(NodeContext * context);
        Expression * condition;
};

class WhileLoopNode: public LoopNode
{
};

class ForLoopNode: public LoopNode
{
public:
        void SetPreLoopStatement(Node * preloop);
        void SetPerOnceStatement(Node * peronce);
        bool PreLoopStatement(NodeContext * context);
        bool PerOnceStatement(NodeContext * context);
        bool Provision();
        bool Check();

        void Swipe(NodeContext * context);
private:
        Node * PreLoop;
        Node * PerOnce;
};

class ForeachLoopNode: public ForLoopNode
{
public:
        bool PreLoopStatement(NodeContext * context);
        bool PerOnceStatement(NodeContext * context);
        void ProvisionKV(NodeContext * context);
        void Swipe(NodeContext * context);

        //1 - true, 0 - false, -1 - error
        int Evaluate(NodeContext * context);

        void SetCollectionExpr(SetExpression * expr);
        void SetKV(std::string key, std::string value);

        bool Provision();
        bool Check();
private:
        std::string Key;
        std::string Value;
        unsigned long KeyIndex;
        unsigned long ValueIndex;        
        SetExpression * CollectionExpr;
};

#endif
