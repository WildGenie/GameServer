#ifndef __NETDISPHANDLE_H
#define __NETDISPHANDLE_H

#include "FastDelegate.h"
#include "MByteBuffer.h"
#include "WorldSession.h"
#include "NetMsgHandle/CmdType.h"

using namespace fastdelegate;

typedef FastDelegate4<MByteBuffer*, int, int, WorldSession*> FourNetDispDelegate;
typedef FastDelegate2<MByteBuffer*, WorldSession*> TWONetDispDelegate;

template<class T>
class NetDispHandle
{
public:
	T** m_pNetDispDelegateArr;

public:
	void initNetDispDelegateSize(size_t len)
	{
		m_pNetDispDelegateArr = new T*[len];
	}

	void addOneDelegate(int idx, T* dele)
	{
		m_pNetDispDelegateArr[idx] = dele;
	}

	void handleMsg(MByteBuffer* pMsgBA, WorldSession* pWorldSession)
	{
		uint8 byCmd;
		uint8 byParam;
		pMsgBA->readUnsignedInt8(byCmd);
		pMsgBA->readUnsignedInt8(byParam);
		pMsgBA->pos(0);				// ÷ÿ÷√∂¡÷∏’Î

		if (byCmd < Cmd::eByCmdTotal && m_pNetDispDelegateArr[byCmd])
		{
			(*m_pNetDispDelegateArr[byCmd])(pMsgBA, byCmd, byParam, pWorldSession);
		}
	}

	void handleMsg(MByteBuffer* pMsgBA, int bCmd, int bParam, WorldSession* pWorldSession)
	{
		if (m_pNetDispDelegateArr[bParam])
		{
			(*m_pNetDispDelegateArr[bParam])(pMsgBA, pWorldSession);
		}
	}
};


#endif