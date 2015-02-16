#ifndef _COMMAND_H
#define _COMMAND_H

#include "CmdType.h"
#include "NullUserCmd.h"
#include "ByCmdMsg.h"

namespace Cmd
{
	struct stObjectBasicCmd : public stObjectBaseCmd
	{
		stObjectBasicCmd()
		{
			byParam = eOBJECT_BASIC_USERCMD;
			id = 0;
		}
		DWORD id;

		virtual void serialize(MByteBuffer& pMsgBA)
		{
			stObjectBaseCmd::serialize(pMsgBA);
			pMsgBA.writeUnsignedInt32(id);
		}

		virtual void derialize(MByteBuffer& pMsgBA)
		{

		}
	};
};


#endif