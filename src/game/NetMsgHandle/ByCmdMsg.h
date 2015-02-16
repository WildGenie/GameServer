#ifndef __BYCMDMSG_H
#define __BYCMDMSG_H

#include "CmdType.h"
#include "NullUserCmd.h"

namespace Cmd
{
	struct stObjectBaseCmd : public stNullUserCmd
	{
		stObjectBaseCmd()
		{
			byCmd = eOBJECT_USERCMD;
			id = 0;
		}
		DWORD id;

		virtual void serialize(MByteBuffer& pMsgBA)
		{
			stNullUserCmd::serialize(pMsgBA);
			pMsgBA.writeUnsignedInt32(id);
		}

		virtual void derialize(MByteBuffer& pMsgBA)
		{

		}
	};
};

#endif