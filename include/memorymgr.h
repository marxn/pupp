#ifndef _P_MEMORY_MGR_H_
#define _P_MEMORY_MGR_H_

#include <list>

using namespace std;

enum PMemoryContext {Global, Session, Block, Statement};

struct PMemoryMgr
{
        list<void*> GlobalPointers;
        list<void*> SessionPointers;
        list<void*> BlockPointers;
        list<void*> StatementPointers;
};

extern void SwitchMemoryContext(PMemoryContext context);
extern void * MemoryContextAllocMemory(size_t size);
extern void MemoryContextDeallocMemory(void * p);
extern void RecycleStatement();
extern void RecycleBlock();
extern void RecycleSession();

extern __thread PMemoryMgr  *  __MMgr;
extern __thread PMemoryContext __MContext;

#endif
