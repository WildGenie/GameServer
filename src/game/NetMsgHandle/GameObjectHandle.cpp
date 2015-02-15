#include "GameNetHandle.h"
#include "CmdType.h"
#include "ObjectCmd.h"

GameObjectHandle::GameObjectHandle()
{
	initNetDispDelegateSize(Cmd::eObjectByParamTotal);

	OneNetDispDelegate* pDelegate;
	pDelegate = new OneNetDispDelegate();
	pDelegate->bind(this, &GameObjectHandle::psstObjectBasicCmd);
	addOneDelegate(Cmd::eOBJECT_BASIC_USERCMD, pDelegate);
}

void GameObjectHandle::handleObject(MByteBuffer* pMsgBA, int bCmd, int bParam, WorldSession* pWorldSession)
{

}

void GameObjectHandle::psstObjectBasicCmd(MByteBuffer* pMsgBA, WorldSession* pWorldSession)
{

}