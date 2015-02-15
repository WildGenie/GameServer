#ifndef __NETDISPHANDLE_H
#define __NETDISPHANDLE_H

#include "FastDelegate.h"

using namespace fastdelegate;

class MByteBuffer;

typedef FastDelegate3<MByteBuffer*, int, int> ThreeNetDispDelegate;
typedef FastDelegate1<MByteBuffer*> OneNetDispDelegate;

template<class T>
class NetDispHandle
{
protected:
	T* m_pNetDispDelegateArr;

public:
	void initNetDispDelegateSize(size_t len)
	{
		m_pNetDispDelegateArr = new T[len];
	}

	void addOneDelegate(int idx, T dele)
	{
		m_pNetDispDelegateArr[idx] = dele;
	}
};


#endif