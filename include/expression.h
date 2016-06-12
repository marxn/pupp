#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <map>
#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include "constval.h"
#include "variable.h"
#include "node.h"
#include "function.h"
#include "closure.h"

using namespace std;

class Expression
{
public:
        Expression():lValue(false), islambda(false)
        {
        }
        virtual ~Expression(){}
        virtual ConstValue * Calculate(NodeContext * context) = 0;

        void SetParentNode(Node * node)
        {
                this->ParentNode = node;
        }

        virtual bool Provision() = 0;
        virtual bool Check() 
        {
                return true;
        }
        void LValue(bool flag)
        {
                this->lValue = flag;
        }
        bool isLValue()
        {
                return this->lValue;
        }
        bool IsLambdaExp()
        {
                return this->islambda;
        }
protected:
        bool lValue;
        bool islambda;
        Node * ParentNode;
};

class ConstValueExpression: public Expression
{
public:
        ConstValueExpression():Value(NULL){}
        ConstValueExpression(ConstValue * value):Value(value){}
        ~ConstValueExpression()
        {
        }
        ConstValue * Calculate(NodeContext * context)
        {
                return Value->DupValue();
        }
        bool Provision()
        {
                return true;
        }
private:
        ConstValue * Value;
};

class BinaryExpression: public Expression
{
public:
        BinaryExpression(Expression * arg1, Expression * arg2):left(arg1),right(arg2)
        {
        }

        ConstValue * Calculate(NodeContext * context)
        {
                ConstValue * left_store = this->left->Calculate(context);
                ConstValue * right_store = this->right->Calculate(context);

                ConstValue * result = this->CarryOut(left_store, right_store);

                delete left_store;
                delete right_store;

                return result;
        }

        virtual ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store) = 0;

        bool Provision()
        {
                left->SetParentNode(this->ParentNode);
                right->SetParentNode(this->ParentNode);

                if(left->Provision()==false)
                {
                        return false;
                }
                if(right->Provision()==false)
                {
                        return false;
                }

                return true;
        }
        bool Check()
        {
                if(left->Check() && right->Check())
                {
                        return true;
                }
                return false;
        }

protected:
        Expression * left;
        Expression * right;
};

class KVExpression: public BinaryExpression
{
public:
        KVExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2)
        {
        }

        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                ValueBox * vb = new ValueBox(right_store);
                KVValue * result = new KVValue(left_store, vb);
                vb->Destroy();
                return result;
        }
};

class SetExpression: public Expression
{
public:
        SetExpression(list<Expression*> * exp):ExprList(exp){}
        ConstValue * Calculate(NodeContext * context)
        {
                long index = 0;
                SetValue * result = new SetValue;
                list<KVValue*> tobedone;

                list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
                        ConstValue * value = (*i)->Calculate(context);
                        if(value->GetType()==KeyValue)
                        {
                                tobedone.push_back(static_cast<KVValue*>(value));
                        }
                        else
                        {
                                IntegerValue * key = new IntegerValue(index);
                                ValueBox * vb = new ValueBox(value);

                                KVValue * kv = new KVValue(key, vb);
                                result->AddKV(kv);

                                vb->Destroy();
                                delete kv;
                                delete key;
                                delete value;
                                index++;
                        }
                }

                list<KVValue*>::iterator j;
                for(j = tobedone.begin(); j!=tobedone.end(); j++)
                {
                        KVValue * value = *j;
                        result->AddKV(value);
                        delete value;
                }

                return result;
        }
        bool Provision()
        {
                list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                       {
                        (*i)->SetParentNode(this->ParentNode);
                               if((*i)->Provision()!=true)
                        {
                                       return false;
                               }
                }

                return true;
        }
        bool Check()
        {
                list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
                        if((*i)->Check()!=true)
                        {
                                return false;
                        }
                }

                return true;
        }

private:
        list<Expression*> * ExprList;
};

class VarExpression: public Expression
{
public:
        VarExpression(string * arg)
        {
                this->VarName = *arg;
                this->lValue = true;
        }

        ValueBox * GetVarRef(NodeContext * context)
        {
                Variable * var = context->GetVariable(VarName);

                if(var==NULL)
                {
                        cerr<<"puppy runtime error: cannot find variable:"<<this->VarName<<endl;
                        return NULL;
                }

                if(this->ExprList.size()==0)
                {
                        return var->GetVBox();
                }

                ValueBox * result = NULL;

                if(var->GetVarType()==Array)
                {
                        if(static_cast<ArrayValue*>(var->GetVBox()->GetVal())->GetDimensionNum()!=this->ExprList.size())
                        {
                                cerr<<"puppy runtime error: wrong dimension variable: "<<var->GetVarName()<<endl;
                                return NULL;
                        }

                        vector<long> desc;
                        list<Expression*>::iterator i;
                        for(i=this->ExprList.begin(); i!=this->ExprList.end(); i++)
                        {
                                ConstValue * value = (*i)->Calculate(context);
                                if(value->GetType()!=Integer)
                                {
                                        cerr<<"puppy runtime error: invalid index for variable: "<<var->GetVarName()<<endl;
                                        delete value;
                                        return NULL;
                                }
                                desc.push_back(static_cast<IntegerValue*>(value)->GetValue());
                                delete value;
                        }

                        ArrayValue * value = static_cast<ArrayValue*>(var->GetVBox()->GetVal());
                        result = value->GetElementBox(desc);
                        return result;
                }

                result = var->GetVBox();

                list<Expression*>::iterator i;
                for(i=this->ExprList.begin(); i!=this->ExprList.end(); i++)
                {
                        if(result->GetVal()->GetType()!=Set)
                        {
                                cerr<<"puppy runtime error: Expect a collection."<<endl;
                                return NULL;
                        }

                        SetValue * local = static_cast<SetValue*>(result->GetVal());
                        
                        ConstValue * key = (*i)->Calculate(context);

                        result = local->FindByKey(key->toString());
                        delete key;

                        if(result==NULL)
                        {
                                return NULL;
                        }
                }

                return result;
        }

        ConstValue * Calculate(NodeContext * context)
        {
                ValueBox * result = this->GetVarRef(context);
                if(result==NULL)
                {
                        return new NullValue;
                }

                return result->GetVal()->DupValue();
        }

        bool Provision()
        {
                list<Expression*>::iterator i;
                for(i=this->ExprList.begin(); i!=this->ExprList.end(); i++)
                {
                        (*i)->SetParentNode(this->ParentNode);
                        if((*i)->Provision()!=true)
                        {
                                return false;
                        }
                }

                return true;
        }

        bool Check()
        {
                VariableDef * vardef = this->ParentNode->FindVariable(this->VarName);
                if(vardef==NULL)
                {
                        cerr<<"puppy provision error: Variable "<<this->VarName<<" has not been defined"<<endl;
                        return false;
                }
                this->VarDef = vardef;

                list<Expression*>::iterator i;
                for(i=this->ExprList.begin(); i!=this->ExprList.end(); i++)
                {
                        if((*i)->Check()!=true)
                        {
                                return false;
                        }
                }

                return Expression::Check();
        }

        void AddOffsetExpr(Expression * expr)
        {
                this->ExprList.push_back(expr);
        }
private:
        string VarName;
        VariableDef * VarDef;
        list<Expression*> ExprList;
};

class FunctionExpression: public Expression
{
public:
        FunctionExpression(Expression * object, list<Expression*> * exprlist)
        {
                this->FuncObj = object;
                this->ExprList = exprlist;
        }
        ConstValue * Calculate(NodeContext * context)
        {
                ConstValue * result = NULL;
                FuncValue * func = static_cast<FuncValue*>(this->FuncObj->Calculate(context));

                if(func==NULL)
                {
                        cerr<<"puppy runtime error: Not a qualified function."<<endl;
                        return NULL;
                }

                if(func->GetType()==Null)
                {
                        cerr<<"puppy runtime error: Function object has not been initialized."<<endl;
                        return NULL;
                }

                FunctionNode * funcnode = func->GetFuncNode();

                if(funcnode)
                {
                        if(funcnode->GetArgList()->size()!=this->ExprList->size())
                        {
                                cerr<<"puppy runtime error: The number of arguments does not match"<<endl;
                                return NULL;
                        }

                        list<Expression*>::iterator i;
                        list<FuncArgDef*>::iterator j;

                        for(i = this->ExprList->begin(), j = funcnode->GetArgList()->begin();
                                i!=this->ExprList->end() && j!=funcnode->GetArgList()->end(); i++,j++)
                        {
                                if((*j)->isRef())
                                {
                                        if((*i)->isLValue()==false)
                                        {
                                                cerr<<"puppy runtime error: Cannot pass any expression other than an lvalue to a reference:"<<(*j)->GetName()<<endl;
                                                return NULL;
                                        }
                                }
                        }

                        int rtn = 0;

                        NodeContext * new_ctx = new NodeContext;
                        new_ctx->AddFrame(funcnode);

                        for(i = this->ExprList->begin(), j = funcnode->GetArgList()->begin(); 
                                i!=this->ExprList->end() && j!=funcnode->GetArgList()->end(); i++,j++)
                        {
                                if((*j)->isRef())
                                {
                                        VarExpression * exp = static_cast<VarExpression*>(*i);
                                        ValueBox * vbox = exp->GetVarRef(context);
                                        Variable * avatar = new_ctx->GetVariable((*j)->GetName());

                                        if(avatar)
                                        {
                                                if(vbox->GetVal()->GetType() != avatar->GetVarType())
                                                {
                                                        cerr<<"puppy runtime error: variable type mismatch when passing args to the function."<<endl;
                                                        return NULL;
                                                }

                                                delete avatar->GetVBox();
                                                avatar->SetVBox(vbox);
                                                avatar->GetVBox()->IncRefCount();
                                        }
                                }
                                else
                                {
                                        ConstValue * value = (*i)->Calculate(context);
                                        if(value->GetType()!=(*j)->GetType())
                                        {
                                                delete new_ctx;
                                                cerr<<"puppy runtime error: data type mismatch when calling a function."<<endl;
                                                return NULL;
                                        }

                                        string argname = (*j)->GetName();
                                        Variable * localvar = new_ctx->GetVariable(argname);
                                        if(localvar)
                                        {
                                                localvar->SetValue(value);
                                        }

                                        delete value;
                                }
                        }

                        list<Variable*> * closurevars = func->GetClosureVars();

                        if(closurevars!=NULL)
                        {
                                //Put closure variables into the new context.
                                list<Variable*>::iterator closurevarindex;
                                for(closurevarindex = closurevars->begin(); closurevarindex != closurevars->end(); closurevarindex++)
                                {
                                        Variable * framevar = (*closurevarindex)->CreateVarRef();
                                        new_ctx->AddVariableToCurrentFrame(framevar);
                                }
                        }

                        //Let's rock!
                        rtn = funcnode->Run(new_ctx);
                        if(rtn==NODE_RET_NEEDRETURN)
                        {
                                result = new_ctx->FunctionRet;
                                if(funcnode->GetRtnType()!=result->GetType())
                                {
                                        ConstValueCaster caster(result, funcnode->GetRtnType());
                                        ConstValue * ret = caster.Cast();
                                        delete result;
                                        result = ret;
                                }
                        }
                        else
                        {
                                result = new NullValue;
                        }

                        delete new_ctx;
//                        context->Rewind();
                }

                delete func;
                return result;
        }
        bool Provision()
        {
                this->FuncObj->SetParentNode(this->ParentNode);
                if(this->FuncObj->Provision()!=true)
                {
                        return false;
                }

                list<Expression*>::iterator i;
                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
                        (*i)->SetParentNode(this->ParentNode);
                        if((*i)->Provision()!=true)
                        {
                                return false;
                        }
                }

                return true;
        }
        bool Check()
        {
                if(this->FuncObj->Check()!=true)
                {
                        return false;
                }

                list<Expression*>::iterator i;

                for(i = this->ExprList->begin(); i!=this->ExprList->end(); i++)
                {
                        if((*i)->Check()!=true)
                        {
                                return false;
                        }
                }

                return Expression::Check();
        }
private:
        Expression *  FuncObj;
        VariableDef * FuncDef;
        list<Expression*> * ExprList;
};

class LambdaExpression: public Expression
{
public:
        LambdaExpression(FunctionNode * node): FuncNode(node)
        {
                this->islambda = true;
        }
        ConstValue * Calculate(NodeContext * context)
        {
                list<ClosureVarDesc*> * CopyVars = this->FuncNode->GetCopyList();
                list<Variable *> * cv = new list<Variable *>;

                if(CopyVars!=NULL)
                {
                    list<ClosureVarDesc*>::iterator i;
                    for(i = CopyVars->begin(); i != CopyVars->end(); i++)
                    {
                        string varname = (*i)->GetVarName();
                        Variable * origin = context->GetVariableFromOuterLayer(varname);
                        if(origin==NULL)
                        {
                            cerr<<"Puppy runtime error: cannot find variable: "<<varname<<" in outer context"<<endl;
                            return NULL;
                        }

                        if((*i)->IsRef())
                        {
                            cv->push_back(origin->CreateVarRef());
                        }
                        else
                        {
                            if(origin->GetVarType()==Func)
                            {
                                ValueBox * vbox = origin->GetVBox();
                                if(vbox==NULL || vbox->GetVal()->GetType()==Null)
                                {
                                    return NULL;
                                }
                            }

                            Variable * cl_var = new Variable(origin->GetVarName(), origin->GetVarType());
                            cl_var->SetSource(origin->GetSource());
                            cl_var->SetRef(origin->GetValue());
                            cv->push_back(cl_var);
                        }
                    }
                }
                FuncValue * ret = new FuncValue(this->FuncNode, cv);

                return ret;
        }

        bool Provision()
        {
                this->FuncNode->SetParentNode(this->ParentNode);
                return this->FuncNode->Provision();
        }
        bool Check()
        {
                return this->FuncNode->Check();
        }
private:
        FunctionNode * FuncNode;
};

class PlusExpression: public BinaryExpression 
{
public:
        PlusExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~PlusExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::AddOperation(left_store, right_store);
        }
};

class SubtractExpression: public BinaryExpression
{
public:
        SubtractExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~SubtractExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::SubOperation(left_store, right_store);
        }
};

class MultiplicationExpression: public BinaryExpression
{
public:
        MultiplicationExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~MultiplicationExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::MulOperation(left_store, right_store);
        }
};

class DivisionExpression: public BinaryExpression
{
public:
        DivisionExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~DivisionExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::DivOperation(left_store, right_store);
        }
};

class GTExpression: public BinaryExpression
{
public:
        GTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~GTExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::GTOperation(left_store, right_store);
        }
};

class LTExpression: public BinaryExpression
{
public:
        LTExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~LTExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::LTOperation(left_store, right_store);
        }
};

class EQExpression: public BinaryExpression
{
public:
        EQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~EQExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::EQOperation(left_store, right_store);
        }
};

class NEQExpression: public BinaryExpression
{
public:
        NEQExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~NEQExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::NEQOperation(left_store, right_store);
        }
};

class GEExpression: public BinaryExpression
{
public:
        GEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~GEExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::GEOperation(left_store, right_store);
        }
};

class LEExpression: public BinaryExpression
{
public:
        LEExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~LEExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::LEOperation(left_store, right_store);
        }
};

class ANDExpression: public BinaryExpression
{
public:
        ANDExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ANDExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::ANDOperation(left_store, right_store);
        }
};

class ORExpression: public BinaryExpression
{
public:
        ORExpression(Expression * arg1, Expression * arg2):BinaryExpression(arg1, arg2){}
        ~ORExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::OROperation(left_store, right_store);
        }
};

class NOTExpression: public BinaryExpression
{
public:
        NOTExpression(Expression * arg):BinaryExpression(NULL, arg)
        {
                this->left = new ConstValueExpression(new BooleanValue(true));
        }
        ~NOTExpression(){}
        ConstValue * CarryOut(ConstValue * left_store, ConstValue * right_store)
        {
                return Operation::NOTOperation(right_store);
        }
};





#endif
