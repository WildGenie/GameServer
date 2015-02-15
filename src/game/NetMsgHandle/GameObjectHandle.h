#ifndef __GAMENETHANDLE_H
#define __GAMENETHANDLE_H

#include "NetDispHandle.h"

class WorldSession;

class GameObjectHandle : public NetDispHandle<OneNetDispDelegate>
{
public:
	GameObjectHandle();
	void handleObject(MByteBuffer* pMsgBA, int bCmd, int bParam, WorldSession* pWorldSession);
};


#endif