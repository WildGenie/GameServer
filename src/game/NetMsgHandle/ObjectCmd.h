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
		DWORD id;  // 选择的国家id 
	};
};


#endif