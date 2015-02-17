#include "UtilMsg.h"
#include "WorldSocketMgr.h"
#include "Socket.h"
#include "MClientProcessData.h"
#include "MByteBuffer.h"

void UtilMsg::sendMsg(Socket* pSocket, Cmd::stNullUserCmd* pMsg)
{
	pMsg->serialize(*(sWorldSocketMgr.getMClientProcessData()->m_sendClientBA));
	pSocket->getNetClientBuffer()->sendMsg(sWorldSocketMgr.getMClientProcessData()->m_sendClientBA);
}