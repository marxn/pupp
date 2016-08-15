#include "parser/transaction.h"

using namespace std;

TransNode::TransNode():ContainerNode(Transaction)
{
}

int TransNode::Invoke(NodeContext * context)
{
        list<TransIdentDesc>::iterator vd;
                
        for(vd = this->VarDefList.begin(); vd!= this->VarDefList.end(); vd++)
        {
                string varname = vd->SrcVardef->GetVarName();
                
                unsigned long src_layer = vd->SrcLayer;
                unsigned long src_index = vd->SrcVardef->GetVarIndex();
                unsigned long dst_index = vd->DstVarDef->GetVarIndex();
                
                Variable * var = context->GetVariable(0, dst_index);
                if(var==NULL)
                {
                        cerr<<"pupp runtime error: cannot find variable "<<varname<<" in current context."<<endl;
                        return NODE_RET_ERROR;
                }
                
                Variable * origin = context->GetVariable(src_layer, src_index);
                if(origin==NULL)
                {
                        cerr<<"pupp runtime error: cannot find variable: "<<varname<<" in outer context"<<endl;
                        return NODE_RET_ERROR;
                }

                //Copy the value of source variable to local transaction variable.
                //Note: var type must be figure out because any var type is determined in run-time.
                ConstValue * value = origin->GetValue();
                DataType type = origin->GetVarType();
                var->SetRef(value);
                var->SetVarType(type);
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
                        list<TransIdentDesc>::iterator vd;
                        for(vd = this->VarDefList.begin(); vd!= this->VarDefList.end(); vd++)
                        {
                                string varname = vd->SrcVardef->GetVarName();
                                
                                unsigned long src_layer = vd->SrcLayer;
                                unsigned long src_index = vd->SrcVardef->GetVarIndex();
                                unsigned long dst_index = vd->DstVarDef->GetVarIndex();
                
                                Variable * var = context->GetVariable(0, dst_index);
                                
                                if(var==NULL)
                                {
                                        cerr<<"pupp runtime error: cannot find variable "<<varname<<" in current context."<<endl;
                                        return NODE_RET_ERROR;
                                }

                                Variable * origin = context->GetVariable(src_layer, src_index);
                                if(origin==NULL)
                                {
                                        cerr<<"pupp runtime error: cannot find variable: "<<varname<<" in outer context"<<endl;
                                        return NODE_RET_ERROR;
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

void TransNode::SetIdentList(list<string*> * idlist)
{
        this->IdList = idlist;
}

bool TransNode::Provision()
{
        Node * parent = this->GetParentNode();

        list<string*>::iterator i;

        for(i = this->IdList->begin(); i!= this->IdList->end(); i++)
        {
                unsigned long layer = 1;
                
                VariableDef * vardef = parent->FindVariable(**i, &layer);
                if(vardef==NULL)
                {
                        cerr<<"pupp provision error: Variable "<<**i<<" has not been defined"<<endl;
                        return false;
                }

                VariableDef * newvardef = new VariableDef(**i);
                newvardef->SetVarType(vardef->GetVarType());
                this->AddVariable(newvardef);
                
                TransIdentDesc desc = {vardef, newvardef, layer};

                this->VarDefList.push_back(desc);
        }

        return ContainerNode::Provision();
}

