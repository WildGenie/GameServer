#ifndef __NETMSGHANDLEMANAGER_H
#define __NETMSGHANDLEMANAGER_H

#include "Policies/Singleton.h"
#include "NetDispHandle.h"

class MByteBuffer;
class WorldSession;
template<class T> class NetDispHandle;

class NetMsgHandleManager
{
public:
	NetDispHandle<FourNetDispDelegate>* m_pNetDispHandle;

public:
	NetMsgHandleManager();
};

#define sNetMsgHandleManager MaNGOS::Singleton<NetMsgHandleManager>::Instance()

#endif