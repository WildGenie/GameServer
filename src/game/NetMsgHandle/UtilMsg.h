#ifndef _UTIL_MSG_H_
#define _UTIL_MSG_H_

#include "NullUserCmd.h"

class Socket;

class UtilMsg
{
public:
	static void sendMsg(Socket* pSocket, Cmd::stNullUserCmd* pMsg);
};

#endif