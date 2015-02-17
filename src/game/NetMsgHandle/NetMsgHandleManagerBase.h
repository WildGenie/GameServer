#ifndef __NET_MSGHANDLEMANAGER_H
#define __NET_MSGHANDLEMANAGER_H

#include "Policies/Singleton.h"
#include "NetDispHandle.h"

class MByteBuffer;
class WorldSession;
template<class T> class NetDispHandle;

class NetMsgHandleManagerBase
{
public:
	NetDispHandle<FourNetDispDelegate>* m_pNetDispHandle;

public:
	NetMsgHandleManagerBase();
};

#endif