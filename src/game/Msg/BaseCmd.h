#pragma once

#include "CmdType.h"
#include "NullUserCmd.h"

namespace Cmd
{
	class stHeroCardCmd : public stNullUserCmd
	{
	public:
		stHeroCardCmd()
		{
			byCmd = eHERO_CARD_USERCMD;
		}
	};
}