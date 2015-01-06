#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "puppybase.h"
#include "errstack.h"
#include "node.h"
#include "constval.h"
#include "expression.h"

extern Node * parse();
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

	Node * tree = parse();

	if(tree && tree->Provision(&errstack))
	{
		if(tree->Execute()==false)
		{
			cerr<<"puppy: abnornal exit."<<endl;
		}
	}
	else if(tree)
	{
		errstack.PrintStack();
	}
		
	fclose(fp);
	return 0;
}
