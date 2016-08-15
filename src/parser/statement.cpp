#include "parser/statement.h"

using namespace std;


StatementNode::StatementNode()
{
}

StatementNode::~StatementNode()
{
}

void StatementNode::Swipe(NodeContext * context)
{
}

int BreakStatement::Invoke(NodeContext * context)
{
        return NODE_RET_NEEDBREAK;
}

bool BreakStatement::Provision()
{
        return true;
}

bool BreakStatement::Check()
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

        cerr<<"pupp provision error: break statement must appear in a loop."<<endl;
        return false;
}


int ContinueStatement::Invoke(NodeContext * context)
{
        return NODE_RET_NEEDCONTINUE;
}

bool ContinueStatement::Provision()
{
        return true;
}

bool ContinueStatement::Check()
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

        cerr<<"pupp provision error: continue statement must appear in a loop."<<endl;
        return false;
}


int RollbackStatement::Invoke(NodeContext * context)
{
        return NODE_RET_NEEDROLLBACK;
}

bool RollbackStatement::Provision()
{
        return true;
}

bool RollbackStatement::Check()
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

        cerr<<"pupp provision error: rollback statement must appear in a transaction."<<endl;
        return false;
}

int CommitStatement::Invoke(NodeContext * context)
{
        return NODE_RET_NEEDCOMMIT;
}

bool CommitStatement::Provision()
{
        return true;
}

bool CommitStatement::Check()
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

        cerr<<"pupp provision error: commit statement must appear in a transaction block."<<endl;
        return false;
}

int ReturnStatement::Invoke(NodeContext * context)
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

void ReturnStatement::SetExpression(Expression * expr)
{
        this->Expr = expr;
}

bool ReturnStatement::Provision()
{
        this->Expr->SetParentNode(this);
        return this->Expr->Provision();
}

bool ReturnStatement::Check()
{
        return this->Expr->Check();
}

LValue::LValue(string * id)
{
        this->Var = *id;
        this->ExpList = new list<Expression*>;
}

string LValue::GetVarName()
{
        return this->Var;
}

list<Expression*> * LValue::GetExpList()
{
        return this->ExpList;
}

void LValue::AddOffsetExpr(Expression * expr)
{
        this->ExpList->push_back(expr);
}

AssignStatement::AssignStatement(LValue * ref)
{
        this->Reference = ref;
}

int AssignStatement::Invoke(NodeContext * context)
{
        Variable * var = context->GetVariable(this->VarLayer, this->VarDef->GetVarIndex());
        if(var==NULL)
        {
                cerr<<"pupp runtime error: cannot find variable: "<<this->VarDef->GetVarName()<<endl;
                return NODE_RET_ERROR;
        }

        if(this->Reference->GetExpList()->size()==0)
        {
                if(var->GetVarType()==Array)
                {
                        cerr<<"pupp runtime error: invalid l-value:"<<var->GetVarName()<<endl;
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
                                cerr<<"pupp runtime error: cannot convert data type:"<<var->GetVarName()<<endl;
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
                        cerr<<"pupp runtime error: wrong dimension variable: "<<var->GetVarName()<<endl;
                        return NODE_RET_ERROR;
                }

                vector<long> desc;
                list<Expression*>::iterator i;
                for(i=exprlist->begin(); i!=exprlist->end(); i++)
                {
                        ConstValue * value = (*i)->Calculate(context);
                        if(value->GetType()!=Integer)
                        {
                                cerr<<"pupp runtime error: invalid index for variable: "<<var->GetVarName()<<endl;
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
                        cerr<<"pupp runtime error: data type mismatch for variable: "<<var->GetVarName()<<endl;
                        return NODE_RET_ERROR;
                }

                if(val->GetElementType()==Decimal && val->GetPrecision()!=-1)
                {
                        static_cast<DecimalValue*>(target_value)->SetPrec(val->GetPrecision());
                }

                val->SetElementValue(desc, target_value);
                return NODE_RET_NORMAL;
        }

        if(var->GetVarType()!=Set)
        {
                cerr<<"pupp runtime error: cannot accept a non-collection variable: "<<var->GetVarName()<<endl;
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
                                SetValue * value_stub = new SetValue();
                                ValueBox * stub = new ValueBox(value_stub);

                                KVValue * kv = new KVValue(last_offset_value, stub);
                                static_cast<SetValue*>(last_set->GetVal())->AddKV(kv);

                                delete kv;
                                delete stub;
                                delete value_stub;

                                vref = static_cast<SetValue*>(last_set->GetVal())->FindByKey(last_offset_value->toString());
                        }
                        else
                        {
                                cerr<<"pupp runtime warning: Cannot use a scalar value as a collection."<<endl;

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
                                SetValue * value_stub = new SetValue();
                                ValueBox * stub = new ValueBox(value_stub);
                                KVValue * kv = new KVValue(offset_value, stub);
                                static_cast<SetValue*>(vref->GetVal())->AddKV(kv);

                                vref = static_cast<SetValue*>(vref->GetVal())->FindByKey(offstr);

                                delete kv;
                                delete value;
                                delete stub;
                                delete value_stub;
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

void AssignStatement::SetExpression(Expression * expr)
{
        this->Expr = expr;
}

bool AssignStatement::Provision()
{
        list<Expression*>* exprlist = this->Reference->GetExpList();

        list<Expression*>::iterator i;
        for(i=exprlist->begin(); i!=exprlist->end(); i++)
        {
                (*i)->SetParentNode(this);
                if((*i)->Provision()==false)
                {
                        return false;
                }
        }

        this->Expr->SetParentNode(this);
        if(this->Expr->Provision()==false)
        {
                return false;
        }

        return true;
}

bool AssignStatement::Check()
{
        unsigned long layer = 1;
        
        Node * parent = this->GetParentNode();
        this->VarDef = parent->FindVariable(this->Reference->GetVarName(), &layer);
        
        if(this->VarDef==NULL)
        {
                cerr<<"pupp provision error: Variable "<<this->Reference->GetVarName()<<"has not defined"<<endl;
                return false;
        }
        
        this->VarLayer = layer;

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


VariableType::VariableType(DataType vartype, DataType elementtype, long prec)
{
        this->VarType     = vartype;
        this->ElementType = elementtype;
        this->Prec        = prec;
}

void VariableType::SetVarType(DataType type)
{
        this->VarType = type;
}

DataType VariableType::GetVarType()
{
        return this->VarType;
}

DataType VariableType::GetElementType()
{
        return this->ElementType;
}

void VariableType::SetElementType(DataType type)
{
        this->ElementType = type;
}

void VariableType::AddDimention(Expression * exp)
{
        this->Dimentions.push_back(exp);
}

list<Expression*> * VariableType::GetDimentions()
{
        return &Dimentions;
}

void VariableType::SetPrecision(long prec)
{
        this->Prec = prec;
}

long VariableType::GetPrecision()
{
        return this->Prec;
}

VarDefinitionStatement::VarDefinitionStatement(string * ident, VariableType * vartype)
{
        this->Ident    = ident;
        this->VarType  = vartype;
        this->InitExpr = NULL;
}

int VarDefinitionStatement::Invoke(NodeContext * context)
{
        string name = *(this->Ident);
        Variable * var = context->GetVariable(1, this->VarDef->GetVarIndex());

        if(var==NULL)
        {
                cerr<<"pupp runtime error: cannot find variable:"<<name<<endl;
                return NODE_RET_ERROR;
        }
        
        //var->SetPrecision(this->VarType->GetPrecision());

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
                                cerr<<"pupp runtime error: Size of an array must be a positive integer."<<endl;
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

        if(this->InitExpr!=NULL)
        {
                ConstValue * value = this->InitExpr->Calculate(context);
                if(value==NULL)
                {
                        return NODE_RET_ERROR;
                }

                if(value->GetType()==Null)
                {
                        cerr<<"pupp runtime error: variable type cannot be determined."<<endl;
                        delete value;
                        return NODE_RET_ERROR;
                }
                var->SetVarType(value->GetType());
                var->SetRef(value);
        }

        return NODE_RET_NORMAL;
}

void VarDefinitionStatement::SetInitExpr(Expression * expr)
{
        this->InitExpr = expr;
}

bool VarDefinitionStatement::Provision()
{
        Node * parent = this->GetParentNode();

        if(parent)
        {
                string varname = *(this->Ident);
                if(parent->FindVariable(varname, NULL))
                {
                        cerr<<"pupp provision error: Duplicated variable: "<<varname<<endl;
                        return false;
                }

                this->VarDef = new VariableDef(varname);
                this->VarDef->SetVarType(this->VarType->GetVarType());
                this->VarDef->SetVarPrec(this->VarType->GetPrecision());

                //Variable type has to be determined before Lambda expression defined.
                if(this->InitExpr!=NULL && this->InitExpr->IsLambdaExp())
                {
                        this->VarDef->SetVarType(Func);
                }

                parent->AddVariable(this->VarDef);
        }

        if(this->InitExpr)
        {
                this->InitExpr->SetParentNode(this);
                return this->InitExpr->Provision();
        }

        return true;
}

bool VarDefinitionStatement::Check()
{
        if(this->InitExpr==NULL)
        {
                return true;
        }

        return this->InitExpr->Check();
}

int PrintStatement::Invoke(NodeContext * context)
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

void PrintStatement::SetExpressionList(list<Expression*> * exprlist)
{
        this->ExprList = exprlist;
}

bool PrintStatement::Provision()
{
        list<Expression*>::iterator i;
        for(i = ExprList->begin(); i!= ExprList->end(); i++)
        {
                (*i)->SetParentNode(this);
                if((*i)->Provision()==false)
                {
                        return false;
                }
        }
        return true;
}

bool PrintStatement::Check()
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

int SleepStatement::Invoke(NodeContext * context)
{
        ConstValue * value = this->Expr->Calculate(context);
        if(value==NULL)
        {
                return NODE_RET_ERROR;
        }

        if(value->GetType()!=Integer)
        {
                //TODO
                cerr<<"pupp runtime error: SLEEP Statement need a Integer parameter. "<<endl;
                return NODE_RET_ERROR;
        }

        usleep(static_cast<IntegerValue*>(value)->GetValue());

        delete value;
        return NODE_RET_NORMAL;
}

void SleepStatement::SetExpression(Expression * expr)
{
        this->Expr = expr;
}

bool SleepStatement::Provision()
{
        this->Expr->SetParentNode(this);
        return this->Expr->Provision();
}

bool SleepStatement::Check()
{
        return this->Expr->Check();
}


CallStatement::CallStatement()
{
        this->RetVal = NULL;
}

int CallStatement::Invoke(NodeContext * context)
{
        this->RetVal = this->Expr->Calculate(context);
        if(this->RetVal==NULL)
        {
                return NODE_RET_ERROR;
        }

        return NODE_RET_NORMAL;
}

void CallStatement::Swipe(NodeContext * context)
{
        delete this->RetVal;
}

void CallStatement::SetExpression(Expression * expr)
{
        this->Expr = expr;
}

bool CallStatement::Provision()
{
        this->Expr->SetParentNode(this);
        return this->Expr->Provision();
}

bool CallStatement::Check()
{
        return this->Expr->Check();
}

ConstValue * CallStatement::GetRetVal()
{
        return this->RetVal;
}

