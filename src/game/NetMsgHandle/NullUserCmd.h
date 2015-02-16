#ifndef _stNullUserCmd_h
#define _stNullUserCmd_h

#include "DataStruct/MByteBuffer.h"
#include "Network/MNetClientBuffer.h"
#include "Network/Socket.h"

namespace Cmd
{
	const BYTE NULL_USERCMD_PARA = 0;

	struct stNullUserCmd
	{
		stNullUserCmd()
		{
			dwTimestamp = 0;
		}
		union
		{
			struct 
			{
				BYTE  byCmd;
				BYTE  byParam;
			};
			struct 
			{
				BYTE  byCmdType;
				BYTE  byParameterType;
			};
		};
		DWORD  dwTimestamp;

		virtual void serialize(MByteBuffer& pMsgBA)
		{
			pMsgBA.writeUnsignedInt8(byCmd).writeUnsignedInt8(byParam);
		}

		virtual void derialize(MByteBuffer& pMsgBA)
		{

		}

		void sendMsg(Socket* pSocket, MByteBuffer* pMsgBA)
		{
			serialize(*pMsgBA);
			pSocket->getNetClientBuffer()->sendMsg(pMsgBA);
		}
	};
}

#endif