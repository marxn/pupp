#ifndef _PUPPY_BASE_H_
#define _PUPPY_BASE_H_

#include <string>

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
private:
	string TypeName;
	int ObjectLoc;
};

#endif

