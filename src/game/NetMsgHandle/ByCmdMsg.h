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
			byParam = eOBJECT_USERCMD;
			id = 0;
		}
		DWORD id;  // ѡ��Ĺ���id 
	};

};


#endif