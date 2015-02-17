#ifndef __GAMENETHANDLE_H
#define __GAMENETHANDLE_H

#include "NetDispHandle.h"

class WorldSession;

class GameObjectHandle : public NetDispHandle<TWONetDispDelegate>
{
public:
	GameObjectHandle();
	//void handleObject(MByteBuffer* pMsgBA, int bCmd, int bParam, WorldSession* pWorldSession);
	void psstObjectBasicCmd(MByteBuffer* pMsgBA, WorldSession* pWorldSession);
};


#endif