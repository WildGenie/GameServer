#include "GameObjectHandle.h"
#include "CmdType.h"
#include "ObjectCmd.h"

GameObjectHandle::GameObjectHandle()
{
	initNetDispDelegateSize(Cmd::eObjectByParamTotal);

	TWONetDispDelegate* pDelegate;
	pDelegate = new TWONetDispDelegate();
	pDelegate->bind(this, &GameObjectHandle::psstObjectBasicCmd);
	addOneDelegate(Cmd::eOBJECT_BASIC_USERCMD, pDelegate);
}

void GameObjectHandle::psstObjectBasicCmd(MByteBuffer* pMsgBA, WorldSession* pWorldSession)
{

}