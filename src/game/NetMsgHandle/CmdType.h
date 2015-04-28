#ifndef _CMD_TYPE_H_
#define _CMD_TYPE_H_

namespace Cmd
{
	enum EnByCmd
	{
		eNULL_USERCMD,			/// 空指令
		eOBJECT_USERCMD,		/// 道具指令
		eHERO_CARD_USERCMD,		/// 卡牌指令

		eByCmdTotal
	};

	enum EnObjectByParam
	{
		eOBJECT_BASIC_USERCMD,		/// 道具基本指令
		eObjectByParamTotal
	};
}

#endif