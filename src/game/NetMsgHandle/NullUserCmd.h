#ifndef _stNullUserCmd_h
#define _stNullUserCmd_h

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
	};
}

#endif