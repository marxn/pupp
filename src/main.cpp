#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "puppybase.h"
#include "errstack.h"
#include "container.h"
#include "constval.h"
#include "expression.h"
#include "bean.h"
#include "statement.h"

extern PuppyBean * parse();
extern FILE * yyin;

using namespace std;

int main(int argc, char * argv[])
{
	FILE * fp = NULL;

	if(argc!=2)
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

	PuppyBean * bean = parse();

	int ret = -1;

	if(bean == NULL)
	{
		fprintf(stderr, "Failed to parse the source code.");
		return -1;
	}

	if(true)
	{
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
		bean->subnodelist->push_back(callstmt);

		if(bean->Provision(&errstack))
	        {
			NodeContext * context = new NodeContext;
			context->AddFrame(bean);
			ret = callstmt->Execute(context);
        	}
	        else
        	{
			cerr<<"provision failed."<<endl;
	                errstack.PrintStack();
        	}
	}
	fclose(fp);
	return ret;
}

