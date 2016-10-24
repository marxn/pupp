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
        this->ExpsList = new list<list<Expression*> *>;
}

string LValue::GetVarName()
{
        return this->Var;
}

list<list<Expression*> *> * LValue::GetExpsList()
{
        return this->ExpsList;
}

void LValue::AddOffsetExprList(list<Expression *> * expr_list)
{
        this->ExpsList->push_back(expr_list);
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

        if(var->GetVarType()!=Array && var->GetVarType()!=Map && this->Reference->GetExpsList()->size()>0)
        {
                cerr<<"pupp runtime error: Variable "<<this->Reference->GetVarName()<<" is neither an array nor a set."<<endl;
                return false;
        }

        ValueBox * result = var->GetVBox();
        
        if(this->Reference->GetExpsList()->size()==0 && var->GetVarType()==Array)
        {
                cerr<<"pupp runtime error: cannot assign for an array"<<var->GetVarName()<<endl;
                return NODE_RET_ERROR;
        }
        
        bool need_convert = true;
        list<list<Expression*>*> * exprslist = this->Reference->GetExpsList();
        
        for(list<list<Expression*>*>::iterator gitor = exprslist->begin(); gitor!=exprslist->end(); gitor++)
        {
                list<Expression*>* exprlist = *gitor;
                
                if(result==NULL)
                {
                        cerr<<"pupp runtime error: cannot find key for variable "<<var->GetVarName()<<endl;
                        return NODE_RET_ERROR;
                }
                else if(result->GetVal()->GetType()==Array)
                {
                        if(static_cast<ArrayValue*>(result->GetVal())->GetDimensionNum()!=exprlist->size())
                        {
                                cerr<<"pupp runtime error: wrong dimension variable: "<<var->GetVarName()<<endl;
                                return NODE_RET_ERROR;
                        }

                        vector<long> desc;
                        
                        for(list<Expression*>::iterator i = exprlist->begin(); i!=exprlist->end(); i++)
                        {
                                ConstValue * value = (*i)->Calculate(context);
                                if(value->GetType()!=Integer)
                                {
                                        ConstValue * thevalue = ConstValueCast(value, Integer);
                                        delete value;

                                        if(thevalue==NULL)
                                        {
                                                cerr<<"pupp runtime error: invalid index type for variable: "<<var->GetVarName()<<endl;
                                                return NODE_RET_ERROR;
                                        }
                                        
                                        value = thevalue;
                                        
                                }
                                
                                desc.push_back(static_cast<IntegerValue*>(value)->GetValue());
                                delete value;
                        }

                        ArrayValue * val = static_cast<ArrayValue*>(result->GetVal());
                        
                        result = val->GetElementBox(desc);
                }
                else if(result->GetVal()->GetType()==Map)
                {
                        need_convert = false;
                        
                        if(exprlist->size() > 1)
                        {
                                cerr<<"pupp runtime error: invalid key list for a collection"<<endl;
                                return NODE_RET_ERROR;
                        }
                        
                        ConstValue * key = exprlist->front()->Calculate(context);
                        if(key==NULL)
                        {
                                cerr<<"pupp runtime error: invalid key for a collection"<<endl;
                                return NODE_RET_ERROR;
                        }
                        
                        MapValue * val = static_cast<MapValue*>(result->GetVal());
                        result = val->FindByKey(key->toString());
                        
                        if(result==NULL)
                        {
                                MapValue * value = new MapValue();
                                ValueBox * vb = new ValueBox(value);
                                KVValue * kv = new KVValue(key, vb);
                                val->AddKV(kv);

                                delete kv;
                                delete vb;
                                delete value;
                                
                                result = val->FindByKey(key->toString());
                        }
                        
                        delete key;
                }
                else
                {
                        cerr<<"pupp runtime error: cannot accept a non-collection variable: "<<var->GetVarName()<<endl;
                        return NODE_RET_ERROR;
                }
        }
        
        ConstValue * value = Expr->Calculate(context);
        
        if(value==NULL)
        {
                cerr<<"pupp runtime error: cannot calculate the value for the expression"<<endl;
                return NODE_RET_ERROR;
        }

        if(result->GetVal()->GetType()!=value->GetType() && need_convert)
        {
                ConstValue * thevalue = ConstValueCast(value, var->GetVarType());
                delete value;
                value = thevalue;

                if(value==NULL)
                {
                        cerr<<"pupp runtime error: data type mismatch while assigning "<<endl;
                        return NODE_RET_ERROR;
                }
        }
        
        this->AdjustValue(result, value, this->OperType);
                
        return NODE_RET_NORMAL;
}

void AssignStatement::AdjustValue(ValueBox * vbox, ConstValue * ad, int oper)
{
        if(oper==0)
        {
                vbox->SetVal(ad);
                return;
        }
        
        ConstValue * holder = vbox->GetVal();
        
        switch(oper)
        {
                case 1:
                        vbox->SetVal(Operation::AddOperation(holder, ad));
                        break;
                case 2:
                        vbox->SetVal(Operation::SubOperation(holder, ad));
                        break;
                case 3:
                        vbox->SetVal(Operation::MulOperation(holder, ad));
                        break;
                case 4:
                        vbox->SetVal(Operation::DivOperation(holder, ad));
                        break;
        }
        
        delete ad;
}

void AssignStatement::SetExpression(Expression * expr)
{
        this->Expr = expr;
}

void AssignStatement::SetOperType(int type)
{
        this->OperType = type;
}

bool AssignStatement::Provision()
{
        list<list<Expression*>*>* exprslist = this->Reference->GetExpsList();

        for(list<list<Expression*>*>::iterator gitor = exprslist->begin(); gitor!=exprslist->end(); gitor++)
        {
                list<Expression*>* exprlist = *gitor;
                for(list<Expression*>::iterator i = exprlist->begin(); i!=exprlist->end(); i++)
                {
                        (*i)->SetParentNode(this);
                        if((*i)->Provision()==false)
                        {
                                return false;
                        }
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
        
        list<list<Expression*>*>* exprslist = this->Reference->GetExpsList();
        
        for(list<list<Expression*>*>::iterator gitor = exprslist->begin(); gitor!=exprslist->end(); gitor++)
        {
                list<Expression*>* exprlist = *gitor;
                for(list<Expression*>::iterator i = exprlist->begin(); i!=exprlist->end(); i++)
                {
                        if((*i)->Check()==false)
                        {
                                return false;
                        }
                }
        }
        
        if(this->Expr->Check()==false)
        {
                return false;
        }

        return true;

}


VariableType::VariableType(DataType vartype, DataType elementtype)
{
        this->VarType     = vartype;
        this->ElementType = elementtype;
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

                ArrayValue * val = new ArrayValue(this->VarType->GetElementType(), desc, size);
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
        this->RetVal     = NULL;
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

void CallStatement::SetThreadNum(long n)
{
        this->ThreadNum = n;
}
