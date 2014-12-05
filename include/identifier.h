#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_

#include <iostream>
#include <string>
#include "puppybase.h"

using namespace std;

class Identifier:public PuppyObject
{
public:
	Identifier(string name)
	{
		this->Name = name;
	}
	string GetName()
	{
		return Name;
	}
	void SetName(string name)
	{
		this->Name = name;
	}
private:
	string Name;
};
#endif
