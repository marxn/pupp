#ifndef _PUPPY_BASE_H_
#define _PUPPY_BASE_H_

#include <string>
#include "memorymgr.h"

using namespace std;

class PuppyObject
{
public:
	void SetTypeName(string name)
	{
		this->TypeName = name;
	}
	string GetTypeName()
	{
		return this->TypeName;
	}
	int GetObjLoc()
	{
		return this->ObjectLoc;
	}
	void SetObjLoc(int loc)
	{
		this->ObjectLoc = loc;
	}
	/*void * operator new(size_t size)
	{
		return MemoryContextAllocMemory(size);
	}
	void operator delete(void * p)
	{
		MemoryContextDeallocMemory(p);
	}*/
private:
	string TypeName;
	int ObjectLoc;
};

#endif

