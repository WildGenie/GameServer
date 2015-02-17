#include "NetMsgHandleManagerBase.h"
#include "WorldSession.h"
#include "Platform/Define.h"
#include "CmdType.h"

NetMsgHandleManagerBase::NetMsgHandleManagerBase()
	: m_pNetDispHandle(new NetDispHandle<FourNetDispDelegate>())
{

}