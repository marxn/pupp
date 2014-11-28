#ifndef _ERR_STACK_H_
#define _ERR_STACK_H_

#include <list>
#include <string>
#include <iostream>

using namespace std;

class ErrorFrame
{
public:
	ErrorFrame(int err, string errmsg):ErrNo(err), ErrMsg(errmsg){}
	int ErrNo;
	string ErrMsg;
};
class ErrorStack
{
public:
	void PushFrame(int err, string errmsg)
	{
		this->ErrorList.push_back(ErrorFrame(err, errmsg));
	}
	list<ErrorFrame>& GetErrorList()
	{
		return this->ErrorList;
	}
	void PrintStack()
	{
		list<ErrorFrame>::iterator i;
		for(i=ErrorList.begin(); i!=ErrorList.end(); i++)
		{
			cerr<<"Puppy error: "<<(*i).ErrMsg<<endl;
		}
	}
private:
	list<ErrorFrame> ErrorList;
};
#endif
