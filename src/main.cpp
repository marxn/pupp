#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "errstack.h"
#include "node.h"
#include "constval.h"
#include "expression.h"

extern Node * parse();

extern FILE * yyin;

using namespace std;
/*
int main()
{
	ConstValue * a = new ConstValue("12");
	ConstValue * b = new ConstValue(3.1f);
	FinalExpression *f1 = new FinalExpression(a);
	FinalExpression *f2 = new FinalExpression(b);

	DivisionExpression * e = new DivisionExpression(f1,f2);

	cout<<e->GetValue().toString()<<endl;
	delete e;
	getchar();
	return 0;
}
*/

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

	if(tree && tree->Transform(&errstack))
	{
		tree->Invoke();
	}
	else if(tree)
	{
		errstack.PrintStack();
	}
		
	fclose(fp);
	return 0;
}
