#include <stdio.h>
#include <stdlib.h>
#include "memorymgr.h"

using namespace std;

__thread PMemoryContext __MContext;
__thread PMemoryMgr   * __MMgr;

void SwitchMemoryContext(PMemoryContext context)
{
	__MContext = context;
}

void * MemoryContextAllocMemory(size_t size)
{
	void * result = malloc(size);
	printf("allocing %p\n", result);
	return result;
}

void MemoryContextDeallocMemory(void * p)
{
	printf("deleting pointer %p\n", p);
	free(p);
}
/*
void RecycleStatement()
{
	list<void*>::iterator i;
	for(i=__MMgr->StatementPointers.begin(); i!= __MMgr->StatementPointers.end(); i++)
	{
		printf("deleting statement pointer %p\n", *i);
		delete *i;
	}
	__MMgr->StatementPointers.clear();
}

void RecycleBlock()
{
	list<void*>::iterator i;
	for(i=__MMgr->BlockPointers.begin(); i!= __MMgr->BlockPointers.end(); i++)
	{
		printf("deleting block pointer %p\n", *i);
		delete *i;
	}
	__MMgr->BlockPointers.clear();
}

void RecycleSession()
{
	list<void*>::iterator i;
	for(i=__MMgr->SessionPointers.begin(); i!= __MMgr->SessionPointers.end(); i++)
	{
		printf("deleting session pointer %p\n", *i);
		delete *i;
	}
	__MMgr->SessionPointers.clear();
}
*/
