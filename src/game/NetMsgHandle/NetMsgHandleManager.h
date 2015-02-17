#ifndef __NETMSGHANDLEMANAGER_H
#define __NETMSGHANDLEMANAGER_H

#include "NetMsgHandleManagerBase.h"

class NetMsgHandleManager : public NetMsgHandleManagerBase
{
public:
	NetMsgHandleManager();
};

#define sNetMsgHandleManager MaNGOS::Singleton<NetMsgHandleManager>::Instance()

#endif