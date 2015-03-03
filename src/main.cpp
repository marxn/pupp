#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "puppybase.h"
#include "errstack.h"
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
                KVValue * kv = new KVValue(pair<ConstValue*, ConstValue*>(key, value));
                para->AddKV(kv);

                delete kv;
                delete value;
                delete key;
        }

        explist->push_back(new ConstValueExpression(para));

        string StartName = "main";
        FunctionExpression * payload = new FunctionExpression(&StartName, explist);

        CallStatement * callstmt = new CallStatement;
        callstmt->SetExpression(payload);
        callstmt->SetParentNode(bean);

        ErrorStack errstack;
        if(callstmt->Provision(&errstack) && callstmt->Check(&errstack))
        {
		Portal portal(bean);

                NodeContext * context = new NodeContext(&portal);
                context->AddFrame(bean);
                callstmt->Execute(context);
                ConstValue * retval = callstmt->GetRetVal();
                if(retval->GetType()==Integer)
                {
                        ret = static_cast<int>(static_cast<IntegerValue*>(retval)->GetValue());
                }
                delete retval;
        }
	else
        {
                cerr<<"Precompile failed."<<endl;
                errstack.PrintStack();
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

	ErrorStack errstack;

	ContainerNode * bean = parse();

	int ret = 0;

	if(bean == NULL)
	{
		fprintf(stderr, "Failed to parse the source code.");
		return -1;
	}
	if(bean->Provision(&errstack)==false || bean->Check(&errstack)==false)
	{
		errstack.PrintStack();
                return -2;
	}

	if(true)
	{
                ret = StartFromMain(bean, argv,argc);
	}
	fclose(fp);
	return ret;
}

