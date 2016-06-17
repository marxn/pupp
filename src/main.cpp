#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "container.h"
#include "constval.h"
#include "expression.h"
#include "statement.h"

extern ContainerNode * parse();
extern FILE * yyin;

using namespace std;

int main(int argc, char * argv[])
{
        FILE * fp = NULL;

        if(argc==1)
        {
                fp = stdin;
        }
        else
        {
                fp = fopen(argv[1],"r");
                if(fp==NULL)
                {
                        fprintf(stderr, "can not open file '%s'\n", argv[1]);
                        return -1;
                }
        }

        yyin = fp;

        ContainerNode * bean = parse();

        if(bean == NULL)
        {
                return -1;
        }

        SetValue * para = new SetValue;

        for(int i = 0; i<argc; i++)
        {
                IntegerValue * key = new IntegerValue(i);
                StringValue * value = new StringValue(string(argv[i]));

                ValueBox * vb = new ValueBox(value->DupValue());
                KVValue * kv = new KVValue(key, new ValueBox(value->DupValue()));
                para->AddKV(kv);

                delete vb;
                delete kv;
                delete value;
                delete key;
        }

        string   para_name = "args";
        VariableType para_type(Set, Null, -1);

        VarDefinitionStatement * defstmt = new VarDefinitionStatement(&para_name, &para_type);
        defstmt->SetParentNode(bean);
        defstmt->SetInitExpr(new ConstValueExpression(para));

        bean->AddNode(defstmt);

        if(bean->Provision()==false || bean->Check()==false)
        {
                return -2;
        }

        NodeContext context;
        context.AddFrame(bean);

        bean->Execute(&context);

        fclose(fp);

        int ret = 0;

        ConstValue * retval = context.FunctionRet;

        if(retval!=NULL && retval->GetType()==Integer)
        {
                ret = static_cast<int>(static_cast<IntegerValue*>(retval)->GetValue());
        }

        return ret;
}

