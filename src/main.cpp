#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "container.h"
#include "constval.h"
#include "expression.h"
#include "statement.h"
#include "portal.h"

extern ContainerNode * parse();
extern FILE * yyin;

using namespace std;

int StartFromMain(Node * bean, char *argv[], int argc)
{
        int ret = 0;
        list<Expression*> * explist = new list<Expression*>;
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

        explist->push_back(new ConstValueExpression(para));

        string StartName = "main";
        FunctionExpression * payload = new FunctionExpression(new LValueExpression(&StartName), explist);

        CallStatement * callstmt = new CallStatement;
        callstmt->SetExpression(payload);
        callstmt->SetParentNode(bean);

        if(callstmt->Provision() && callstmt->Check())
        {
                Portal portal(bean);

                NodeContext * context = new NodeContext(&portal);
                context->AddFrame(bean);

                //To ensure all global variables to be initlialized.
                bean->Execute(context);

                //Start to run from the begining node.
                callstmt->Invoke(context);
                ConstValue * retval = callstmt->GetRetVal();
                if(retval->GetType()==Integer)
                {
                        ret = static_cast<int>(static_cast<IntegerValue*>(retval)->GetValue());
                }
                callstmt->Swipe(context);
                delete context;
        }
        else
        {
                ret = -1;
        }
        return ret;
}

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
                        fprintf(stderr, "can not open file\n");
                        return -1;
                }
        }

        yyin = fp;

        ContainerNode * bean = parse();

        int ret = 0;

        if(bean == NULL)
        {
                return -1;
        }
        if(bean->Provision()==false || bean->Check()==false)
        {
                return -2;
        }

        if(true)
        {
                ret = StartFromMain(bean, argv,argc);
        }
        fclose(fp);
        return ret;
}

