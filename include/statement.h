#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include <unistd.h>
#include <iostream>
#include "node.h"
#include "expression.h"
#include "portal.h"

using namespace std;

class StatementNode :public Node
{
public:
        StatementNode(){}
        ~StatementNode(){}
        void Swipe(NodeContext * context){}
};

class BreakStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NEEDBREAK;
        }
        bool Provision()
        {
                return true;
        }
        bool Check()
        {
                Node * node = this->ParentNode;
                while(node!=NULL)
                {
                        if(node->Type==Loop)
                        {
                                return true;
                        }
                        node = node->ParentNode;
                }

                cerr<<"puppy provision error: break statement must appear in a loop."<<endl;
                return false;
        }
};

class ContinueStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NEEDCONTINUE;
        }
        bool Provision()
        {
                return true;
        }
        bool Check()
        {
                Node * node = this->ParentNode;
                while(node!=NULL)
                {
                        if(node->Type==Loop)
                        {
                                return true;
                        }
                        node = node->ParentNode;
                }

                cerr<<"puppy provision error: continue statement must appear in a loop."<<endl;
                return false;
        }

};

class LValue 
{
public:
        LValue(string * id)
        {
                this->Var = *id;
                this->ExpList = new list<Expression*>;
        }
        string GetVarName()
        {
                return this->Var;
        }
        list<Expression*> * GetExpList()
        {
                return this->ExpList;
        }
        void AddOffsetExpr(Expression * expr)
        {
                this->ExpList->push_back(expr);
        }
private:
        string Var;
        list<Expression*> * ExpList;
};

class AssignStatement: public StatementNode
{
public:
        AssignStatement(LValue * ref):Reference(ref){}
        int Invoke(NodeContext * context)
        {
                Variable * var = context->GetVariable(this->VarDef->GetVarName());
                if(var==NULL)
                {
                        var = context->GetPortal()->GetSharedVariable(this->VarDef);
                        if(var==NULL)
                        {
                                cerr<<"puppy runtime error: cannot find variable: "<<this->VarDef->GetVarName()<<endl;
                                return NODE_RET_ERROR;
                        }
                }

                if(this->Reference->GetExpList()->size()==0)
                {
                        if(var->GetVarType()==Array)
                        {
                                cerr<<"puppy runtime error: invalid l-value:"<<var->GetVarName()<<endl;
                                return NODE_RET_ERROR;
                        }

                        ConstValue * value = Expr->Calculate(context);
                        if(value==NULL)
                        {
                                return NODE_RET_ERROR;
                        }

                        if(var->GetVarType()!=value->GetType())
                        {
                                ConstValueCaster caster(value, var->GetVarType());
                                ConstValue * thevalue = caster.Cast();
                                delete value;
                                value = thevalue;

                                if(value==NULL)
                                {
                                        cerr<<"Puppy runtime error: cannot convert data type:"<<var->GetVarName()<<endl;
                                        return NODE_RET_ERROR;
                                }

                        }

                        var->SetValue(value);
                        delete value;
                        return NODE_RET_NORMAL;
                }

                list<Expression*>* exprlist = this->Reference->GetExpList();

                if(var->GetVarType()==Array)
                {
                        if(static_cast<ArrayValue*>(var->GetVBox()->GetVal())->GetDimensionNum()!=exprlist->size())
                        {
                                cerr<<"puppy runtime error: wrong dimension variable: "<<var->GetVarName()<<endl;
                                return NODE_RET_ERROR;
                        }

                        vector<long> desc;
                        list<Expression*>::iterator i;
                        for(i=exprlist->begin(); i!=exprlist->end(); i++)
                        {
                                ConstValue * value = (*i)->Calculate(context);
                                if(value->GetType()!=Integer)
                                {
                                        cerr<<"puppy runtime error: invalid index for variable: "<<var->GetVarName()<<endl;
                                        delete value;
                                        return NODE_RET_ERROR;
                                }
                                desc.push_back(static_cast<IntegerValue*>(value)->GetValue());
                                delete value;
                        }

                        ArrayValue * val = static_cast<ArrayValue*>(var->GetVBox()->GetVal());

                        ConstValue * target_value = Expr->Calculate(context);
                        if(target_value==NULL)
                        {
                                return NODE_RET_ERROR;
                        }
                        if(val->GetElementType()!=target_value->GetType())
                        {
                                cerr<<"puppy runtime error: data type mismatch for variable: "<<var->GetVarName()<<endl;
                                return NODE_RET_ERROR;
                        }

                        if(val->GetElementType()==Decimal && val->GetPrecision()!=-1)
                        {
                                static_cast<DecimalValue*>(target_value)->SetPrec(val->GetPrecision());
                        }

                        val->SetElementValue(desc, target_value);
                        return NODE_RET_NORMAL;
                }

                if(var->GetValueType()!=Set)
                {
                        cerr<<"puppy runtime error: cannot accept a non-collection variable: "<<var->GetVarName()<<endl;
                        return NODE_RET_ERROR;
                }

                if(var->GetValueType()==Null)
                {
                        var->SetValue(new SetValue);
                }

                ValueBox * vref = var->GetVBox();
                int listsize = exprlist->size();

                bool need_clear = false;
                ValueBox * last_set = NULL;
                ConstValue * last_offset_value = NULL;

                list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++, listsize--)
                {
                        ConstValue * offset_value = (*i)->Calculate(context);
                        if(offset_value==NULL)
                        {
                                return NODE_RET_ERROR;
                        }

                        string offstr = offset_value->toString();

                        if(vref->GetVal()->GetType()!=Set)
                        {
                                if(vref->GetVal()->GetType()==Null)
                                {
                                        ValueBox * stub = new ValueBox(new SetValue);

                                        KVValue * kv = new KVValue(last_offset_value, stub);
                                        static_cast<SetValue*>(last_set->GetVal())->AddKV(kv);

                                        delete kv;
                                        delete stub;

                                        vref = static_cast<SetValue*>(last_set->GetVal())->FindByKey(last_offset_value->toString());
                                }
                                else
                                {
                                        cerr<<"puppy runtime warning: Cannot use a scalar value as a collection."<<endl;

                                        if(need_clear)
                                        {
                                                delete last_offset_value;
                                                need_clear = false;
                                        }

                                        delete offset_value;
                                        return NODE_RET_NORMAL;
                                }
                        }

                        if(need_clear)
                        {
                                delete last_offset_value;
                                need_clear = false;
                        }

                        ValueBox * value = static_cast<SetValue*>(vref->GetVal())->FindByKey(offstr);

                        if(value==NULL)
                        {
                                if(listsize>1)
                                {
                                        ValueBox * stub = new ValueBox(new SetValue);
                                        KVValue * kv = new KVValue(offset_value, stub);
                                        static_cast<SetValue*>(vref->GetVal())->AddKV(kv);

                                        vref = static_cast<SetValue*>(vref->GetVal())->FindByKey(offstr);

                                        delete kv;
                                        delete value;
                                        delete stub;
                                }
                                else
                                {
                                        ConstValue * target_value = Expr->Calculate(context);
                                        if(target_value==NULL)
                                        {
                                                return NODE_RET_ERROR;
                                        }

                                        ValueBox * vb = new ValueBox(target_value);
                                        KVValue * kv = new KVValue(offset_value, vb);
                                        static_cast<SetValue*>(vref->GetVal())->AddKV(kv);

                                        delete vb;
                                        delete kv;
                                        delete target_value;
                                }
                        }
                        else
                        {
                                if(listsize>1)
                                {
                                        last_set = vref;
                                        last_offset_value = offset_value->DupValue();
                                        vref = value;
                                        need_clear = true;
                                }
                                else
                                {
                                        ConstValue * target_value = Expr->Calculate(context);
                                        if(target_value==NULL)
                                        {
                                                return NODE_RET_ERROR;
                                        }

                                        ValueBox * vb = new ValueBox(target_value);
                                        KVValue * kv = new KVValue(offset_value, vb);
                                        static_cast<SetValue*>(vref->GetVal())->AddKV(kv);

                                        delete vb;
                                        delete kv;
                                        delete target_value;
                                }
                        }
                        
                        delete offset_value;
                }

                return NODE_RET_NORMAL;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                list<Expression*>* exprlist = this->Reference->GetExpList();

                list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++)
                {
                        (*i)->SetParentNode(this->GetParentNode());
                        if((*i)->Provision()==false)
                        {
                                return false;
                        }
                }

                this->Expr->SetParentNode(this->GetParentNode());
                if(this->Expr->Provision()==false)
                {
                        return false;
                }

                return true;
        }
        bool Check()
        {
                this->VarDef = this->FindVariable(this->Reference->GetVarName());
                if(this->VarDef==NULL)
                {
                        cerr<<"puppy provision error: Variable "<<this->Reference->GetVarName()<<"has not defined"<<endl;
                        return false;
                }

                list<Expression*>* exprlist = this->Reference->GetExpList();

                list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++)
                {
                        if((*i)->Check()==false)
                        {
                                return false;
                        }
                }

                if(this->Expr->Check()==false)
                {
                        return false;
                }

                return true;

        }
private:
        LValue * Reference;
        Expression * Expr;
        VariableDef * VarDef;
};

class VariableType
{
public:
        VariableType(DataType vartype, DataType elementtype, long prec):VarType(vartype), ElementType(elementtype), Prec(prec){}

        void SetVarType(DataType type)
        {
                this->VarType = type;
        }
        DataType GetVarType()
        {
                return this->VarType;
        }
        DataType GetElementType()
        {
                return this->ElementType;
        }
        void SetElementType(DataType type)
        {
                this->ElementType = type;
        }
        void AddDimention(Expression * exp)
        {
                this->Dimentions.push_back(exp);
        }
        list<Expression*> * GetDimentions()
        {
                return &Dimentions;
        }
        void SetPrecision(long prec)
        {
                this->Prec = prec;
        }
        long GetPrecision()
        {
                return this->Prec;
        }
private:
        DataType VarType;
        DataType ElementType;
        list<Expression*> Dimentions;
        long Prec;
};

class VarDefinitionStatement: public StatementNode
{
public:
        VarDefinitionStatement(string * ident, VariableType * vartype):Ident(ident), VarType(vartype), InitValue(NULL), InitExpr(NULL)
        {
        }
        int Invoke(NodeContext * context)
        {
                string name = *(this->Ident);
                Variable * var = context->GetVariable(name);

                if(var==NULL)
                {
                        cerr<<"puppy runtime error: cannot find variable:"<<name<<endl;
                        return NODE_RET_ERROR;
                }
                
                var->SetPrecision(this->VarType->GetPrecision());

                if(this->VarType->GetVarType()==Array)
                {
                        list<Expression*> * dim = this->VarType->GetDimentions();
                        vector<long> desc;
                        long size = 1;

                        list<Expression*>::iterator i;
                        for(i = dim->begin(); i!= dim->end(); i++)
                        {
                                ConstValue * dsize = (*i)->Calculate(context);
                                if(dsize->GetType()!=Integer || static_cast<IntegerValue*>(dsize)->GetValue()<=0)
                                {
                                        cerr<<"puppy runtime error: Size of an array must be a positive integer."<<endl;
                                        delete dsize;
                                        return NODE_RET_ERROR;
                                }

                                delete dsize;

                                long n = static_cast<IntegerValue*>(dsize)->GetValue();
                                size *= n;
                                desc.push_back(n);
                        }

                        ArrayValue * val = new ArrayValue(this->VarType->GetElementType(), desc, size, this->VarType->GetPrecision());
                        var->SetRef(val);
                        return NODE_RET_NORMAL;
                }

                if(this->InitValue!=NULL)
                {
                        var->SetValue(this->InitValue);
                }
                else if(this->InitExpr!=NULL)
                {
                        ConstValue * value = this->InitExpr->Calculate(context);
                        if(value==NULL)
                        {
                                return NODE_RET_ERROR;
                        }

                        if(value->GetType()==Null)
                        {
                                cerr<<"puppy runtime error: variable type cannot be determined."<<endl;
                                delete value;
                                return NODE_RET_ERROR;
                        }

                        var->SetVarType(value->GetType());
                        var->SetRef(value);
                }
                else
                {
                        DefaultValueFactory defvalue(this->VarType->GetVarType(), this->VarType->GetPrecision());
                        ConstValue * value = defvalue.GetValue();
                        var->SetRef(value);
                }

                return NODE_RET_NORMAL;
        }
        void SetInitValue(ConstValue * value)
        {
                this->InitValue = value;
        }
        void SetInitExpr(Expression * expr)
        {
                this->InitExpr = expr;
        }
        bool Provision()
        {
                Node * parent = this->GetParentNode();
                if(parent)
                {
                        string varname = *(this->Ident);
                        if(parent->FindVariable(varname))
                        {
                                cerr<<"puppy provision error: Duplicated variable: "<<varname<<endl;
                                return false;
                        }

                        this->VarDef = new VariableDef(varname);
                        this->VarDef->SetVarType(this->VarType->GetVarType());
                        parent->AddVariable(this->VarDef);
                }

                if(this->InitExpr==NULL)
                {
                        return true;
                }

                this->InitExpr->SetParentNode(parent);
                return this->InitExpr->Provision();
        }
        bool Check()
        {
                if(this->InitExpr==NULL)
                {
                        return true;
                }

                return this->InitExpr->Check();
        }
private:
        VariableDef * VarDef;
        ConstValue * InitValue;
        Expression * InitExpr;
        string * Ident;
        VariableType * VarType;
};

class PrintStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        ConstValue * value = (*i)->Calculate(context);
                        if(value==NULL)
                        {
                                return NODE_RET_ERROR;
                        }

                        fprintf(stdout, "%s", value->toString().c_str());
                        fflush(stdout);
                        
                        delete value;
                }
                return NODE_RET_NORMAL;
        }
        void SetExpressionList(list<Expression*> * exprlist)
        {
                this->ExprList = exprlist;
        }
        bool Provision()
        {
                list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        (*i)->SetParentNode(this->GetParentNode());
                        if((*i)->Provision()==false)
                        {
                                return false;
                        }
                }
                return true;
        }
        bool Check()
        {
                list<Expression*>::iterator i;
                for(i = ExprList->begin(); i!= ExprList->end(); i++)
                {
                        if((*i)->Check()==false)
                        {
                                return false;
                        }
                }
                return true;
        }

private:
        list<Expression*> * ExprList;
};

class SleepStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                ConstValue * value = this->Expr->Calculate(context);
                if(value==NULL)
                {
                        return NODE_RET_ERROR;
                }

                if(value->GetType()!=Integer)
                {
                        //TODO
                        cerr<<"puppy runtime error: SLEEP Statement need a Integer parameter. "<<endl;
                        return NODE_RET_ERROR;
                }

                usleep(static_cast<IntegerValue*>(value)->GetValue());

                delete value;
                return NODE_RET_NORMAL;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
        }
        bool Check()
        {
                return this->Expr->Check();
        }

private:
        Expression * Expr;
};

class ReturnStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                ConstValue * value = this->Expr->Calculate(context);
                if(value==NULL)
                {
                        return NODE_RET_ERROR;
                }

                context->FunctionRet = value->DupValue();

                delete value;
                return NODE_RET_NEEDRETURN;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
        }
        bool Check()
        {
                Node * node = this->ParentNode;
                while(node!=NULL)
                {
                        if(node->Type==Function)
                        {
                                return this->Expr->Check();
                        }
                        node = node->ParentNode;
                }

                cerr<<"puppy provision error: return statement must appear in a function."<<endl;
                return false;
        }

private:
        Expression * Expr;

};

class RollbackStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NEEDROLLBACK;
        }
        bool Provision()
        {
                return true;
        }
        bool Check()
        {
                Node * node = this->ParentNode;
                while(node!=NULL)
                {
                        if(node->Type==Transaction)
                        {
                                return true;
                        }
                        node = node->ParentNode;
                }

                cerr<<"puppy provision error: rollback statement must appear in a transaction."<<endl;
                return false;
        }

};

class CommitStatement: public StatementNode
{
public:
        int Invoke(NodeContext * context)
        {
                return NODE_RET_NEEDCOMMIT;
        }
        bool Provision()
        {
                return true;
        }
        bool Check()
        {
                Node * node = this->ParentNode;
                while(node!=NULL)
                {
                        if(node->Type==Transaction)
                        {
                                return true;
                        }
                        node = node->ParentNode;
                }

                cerr<<"puppy provision error: commit statement must appear in a transaction."<<endl;
                return false;
        }

};

class CallStatement: public StatementNode
{
public:
        CallStatement():RetVal(NULL){}

        int Invoke(NodeContext * context)
        {
                this->RetVal = this->Expr->Calculate(context);
                if(this->RetVal==NULL)
                {
                        return NODE_RET_ERROR;
                }

                return NODE_RET_NORMAL;
        }
        void Swipe(NodeContext * context)
        {
                delete this->RetVal;
        }
        void SetExpression(Expression * expr)
        {
                this->Expr = expr;
        }
        bool Provision()
        {
                this->Expr->SetParentNode(this->GetParentNode());
                return this->Expr->Provision();
        }
        bool Check()
        {
                return this->Expr->Check();
        }

        ConstValue * GetRetVal()
        {
                return this->RetVal;
        }
private:
        ConstValue * RetVal;
        Expression * Expr;
};

#endif

