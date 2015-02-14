#ifndef __INETMGR_H
#define __INETMGR_H

#include "MyProject.h"
#include <string>

class INetMgr
{
public:
	virtual ~INetMgr(){};
	virtual void openSocket(std::string ip, uint32 port) = 0;
	virtual void recAndSendMsg() = 0;
};

#endif		// __INETMGR_H