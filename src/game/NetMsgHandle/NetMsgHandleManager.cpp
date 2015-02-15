#include "NetMsgHandleManager.h"
#include "WorldSession.h"
#include "GameObjectHandle.h"
#include "Platform/Define.h"
#include "CmdType.h"

NetMsgHandleManager::NetMsgHandleManager()
	: m_pNetDispHandle(new NetDispHandle<FourNetDispDelegate>())
{
	m_pNetDispHandle->initNetDispDelegateSize(Cmd::eByCmdTotal);

	FourNetDispDelegate* pDelegate;
	pDelegate = new FourNetDispDelegate();
	pDelegate->bind(new GameObjectHandle(), &GameObjectHandle::handleMsg);
	m_pNetDispHandle->addOneDelegate(Cmd::eOBJECT_USERCMD, pDelegate);
}