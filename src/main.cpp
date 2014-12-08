#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "memorymgr.h"
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
	__MMgr = new PMemoryMgr;
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

	if(tree && tree->Transform(&errstack))
	{
		tree->Execute();
	}
	else if(tree)
	{
		errstack.PrintStack();
	}
		
	fclose(fp);
	return 0;
}
