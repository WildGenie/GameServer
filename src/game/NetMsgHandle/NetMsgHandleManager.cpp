#include "NetMsgHandleManager.h"
#include "WorldSession.h"
#include "GameNetHandle.h"
#include "Platform/Define.h"
#include "CmdType.h"

NetMsgHandleManager::NetMsgHandleManager()
	: m_pNetDispHandle(new NetDispHandle<FourNetDispDelegate>())
{
	m_pNetDispHandle->initNetDispDelegateSize(Cmd::eByCmdTotal);

	FourNetDispDelegate* pDelegate;
	pDelegate = new FourNetDispDelegate();
	pDelegate->bind(this, &GameNetHandle::handleObject);
	m_pNetDispHandle->addOneDelegate(Cmd::eOBJECT_USERCMD, pDelegate);
}

void NetMsgHandleManager::handleMsg(MByteBuffer* pMsgBA, WorldSession* pWorldSession)
{
	uint8 byCmd;
	uint8 byParam;
	pMsgBA->readUnsigneduint8(byCmd);
	pMsgBA->readUnsigneduint8(byParam);
	pMsgBA->rpos(0);				// ÷ÿ÷√∂¡÷∏’Î

	if (byCmd < Cmd::eByCmdTotal && m_pNetDispHandle->m_pNetDispDelegateArr[byCmd])
	{
		m_pNetDispHandle->m_pNetDispDelegateArr[byCmd]();
	}
}