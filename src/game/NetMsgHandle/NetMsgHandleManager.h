#ifndef __NETMSGHANDLEMANAGER_H
#define __NETMSGHANDLEMANAGER_H

#include "Policies/Singleton.h"

class MByteBuffer;
class WorldSession;
class NetDispHandle;

class NetMsgHandleManager
{
public:
	NetDispHandle<FourNetDispDelegate>* m_pNetDispHandle;

public:
	NetMsgHandleManager();
	void handleMsg(MByteBuffer* pMsgBA, WorldSession* pWorldSession);
};

#define sNetMsgHandleManager MaNGOS::Singleton<NetMsgHandleManager>::Instance()

#endif