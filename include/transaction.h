#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <list>
#include "container.h"

using namespace std;

class TransNode: public ContainerNode
{
public:
        TransNode():ContainerNode(Transaction){}

        int Invoke(NodeContext * context)
        {
                list<VariableDef*>::iterator vd;
                for(vd = this->VarDefList.begin(); vd!= this->VarDefList.end(); vd++)
                {
                        string varname = (*vd)->GetVarName();
                        Variable * var = context->GetVariable(varname);
                        if(var==NULL)
                        {
                                cerr<<"puppy runtime error: cannot find variable "<<varname<<" in current context."<<endl;
                                return NODE_RET_ERROR;
                        }

                        Variable * origin = context->GetVariableFromOuterLayer(varname);
                        if(origin==NULL)
                        {
                                origin = context->GetPortal()->GetSharedVariable(*vd);
                                if(origin==NULL)
                                {
                                        cerr<<"Puppy runtime error: cannot find variable: "<<varname<<" in outer context"<<endl;
                                        return NODE_RET_ERROR;
                                }
                        }

                        ConstValue * value = origin->GetValue();
                        var->SetRef(value);
                        //delete value;
                }

                list<Node*>::iterator i;
                for(i = subnodelist->begin(); i != subnodelist->end(); i++)
                {
                        context->AddFrame(*i);
                        int cond = (*i)->Execute(context);
                        context->PopFrame();

                        if(cond==NODE_RET_NEEDROLLBACK)
                        {
                                return NODE_RET_NORMAL;
                        }
                        else if(cond==NODE_RET_NEEDCOMMIT)
                        {
                                list<VariableDef*>::iterator vd;
                                for(vd = this->VarDefList.begin(); vd!= this->VarDefList.end(); vd++)
                                {
                                        string varname = (*vd)->GetVarName();
                                        Variable * var = context->GetVariable(varname);
                                        if(var==NULL)
                                        {
                                                cerr<<"puppy runtime error: cannot find variable "<<varname<<" in current context."<<endl;
                                                return NODE_RET_ERROR;
                                        }

                                        Variable * origin = context->GetVariableFromOuterLayer(varname);
                                        if(origin==NULL)
                                        {
                                                origin = context->GetPortal()->GetSharedVariable(*vd);
                                                if(origin==NULL)
                                                {
                                                        cerr<<"puppy runtime error: cannot find variable: "<<varname<<" in outer context"<<endl;
                                                        return NODE_RET_ERROR;
                                                }
                                        }

                                        ConstValue * value = var->GetValue();
                                        origin->SetRef(value);
                                        //delete value;
                                }

                                return NODE_RET_NORMAL;
                        }
                        else if(cond!=NODE_RET_NORMAL)
                        {
                                return cond;
                        }
                }

                return NODE_RET_NORMAL;
        }

        void SetIdentList(list<string*> * idlist)
        {
                this->IdList = idlist;
        }

        bool Provision()
        {
                Node * parent = this->GetParentNode();

                list<string*>::iterator i;

                for(i = this->IdList->begin(); i!= this->IdList->end(); i++)
                {
                        VariableDef * vardef = parent->FindVariable(**i);
                        if(vardef==NULL)
                        {
                                cerr<<"puppy provision error: Variable "<<**i<<" has not been defined"<<endl;
                                return false;
                        }

                        this->VarDefList.push_back(vardef);

                        VariableDef * newvardef = new VariableDef(**i);
                        newvardef->SetVarType(vardef->GetVarType());
                        this->AddVariable(newvardef);
                }

                return ContainerNode::Provision();
        }

private:
        list<string*> * IdList;
        list<VariableDef*> VarDefList;
};

#endif
