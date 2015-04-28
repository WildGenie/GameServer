#pragma once

#include "BaseCmd.h"
#include <string>

namespace Cmd
{
	enum EHeroCardCmd
	{
		eRET_MAGIC_POINT_INFO_CMD = 21,
		eADD_BATTLE_CARD_PROPERTY_CMD = 26,
	};

	struct t_MagicPoint
	{
	public:
		uint32 mp;
		uint32 maxmp;
		uint32 forbid;

		t_MagicPoint()
		{
			mp = 0;
			maxmp = 0;
			forbid = 0;
		}

		void serialize(MByteBuffer& pMsgBU)
		{
			pMsgBU.writeUnsignedInt32(mp);
			pMsgBU.writeUnsignedInt32(maxmp);
			pMsgBU.writeUnsignedInt32(forbid);
		}
	};

	class stObjectLocation
	{
	public:
		uint32 dwLocation;
		uint32 dwTableID;
		uint16 x;
		uint16 y;

		virtual void serialize(MByteBuffer& pMsgBU)
		{
			pMsgBU.writeUnsignedInt32(dwLocation);
			pMsgBU.writeUnsignedInt32(dwTableID);
			pMsgBU.writeUnsignedInt16(x);
			pMsgBU.writeUnsignedInt16(y);
		}
	};

	class t_Card
	{
	public:
		uint32 qwThisID;		    //物品唯一id
		uint32 dwObjectID;		    //物品表中的编号
		stObjectLocation pos;	// 位置

		uint32 dwNum;		//这个属性需要删除
		std::string strName; //名称
		uint32 mpcost;		    //蓝耗
		uint32 damage;		    //攻击力
		uint32 hp;		        //血量
		uint32 maxhp;		    //血量上限
		uint32 dur;		    //耐久度
		uint32 maxdur;		    //耐久度上限

		uint8 magicDamAdd;	//法术伤害增加(X)
		uint8 overload;		//过载(num)
		uint32 armor;          //护甲值

		char* state;

		virtual void serialize(MByteBuffer& pMsgBU)
		{
			pMsgBU.writeUnsignedInt32(qwThisID);
			pMsgBU.writeUnsignedInt32(dwObjectID);
			pos.serialize(pMsgBU);

			pMsgBU.writeUnsignedInt32(dwNum);
			pMsgBU.writeMultiByte(strName, strName.length());
			pMsgBU.writeUnsignedInt32(mpcost);
			pMsgBU.writeUnsignedInt32(damage);
			pMsgBU.writeUnsignedInt32(hp);
			pMsgBU.writeUnsignedInt32(maxhp);
			pMsgBU.writeUnsignedInt32(dur);
			pMsgBU.writeUnsignedInt32(maxdur);

			pMsgBU.writeUnsignedInt8(magicDamAdd);
			pMsgBU.writeUnsignedInt8(overload);
			pMsgBU.writeUnsignedInt32(armor);

			uint32 len = (8 + 7) / 8;
			state = new char[len];
			pMsgBU.writeBytes(state, 0, len);
			delete state;
		}
	};

	class stRetMagicPointInfoUserCmd : public stHeroCardCmd
	{
	public:
		t_MagicPoint self;
		t_MagicPoint other;

	public:
		stRetMagicPointInfoUserCmd()
		{
			byParam = eRET_MAGIC_POINT_INFO_CMD;
		}

		virtual void serialize(MByteBuffer& pMsgBU)
		{
			stHeroCardCmd::serialize(pMsgBU);
			self.serialize(pMsgBU);
			other.serialize(pMsgBU);
		}
	};

	class stAddBattleCardPropertyUserCmd : public stHeroCardCmd
	{
	public:
		uint8 slot;	    //哪个槽
		uint8 who;	    //1,自己 2,对方
		uint8 byActionType;  //
		t_Card mobject;

		uint8 attackType;    //攻击类型
		// 只有攻击刷新属性的时候
		uint32 pAttThisID;   //攻击者
		uint32 pDefThisID;   //防御者

		stAddBattleCardPropertyUserCmd()
		{
			byParam = eADD_BATTLE_CARD_PROPERTY_CMD;
			slot = 0;
			who = 0;
			byActionType = 0;
		}

		virtual void serialize(MByteBuffer& pMsgBU)
		{
			stHeroCardCmd::serialize(pMsgBU);
			
			pMsgBU.writeUnsignedInt8(slot);
			pMsgBU.writeUnsignedInt8(who);
			pMsgBU.writeUnsignedInt8(byActionType);
			mobject.serialize(pMsgBU);

			pMsgBU.writeUnsignedInt8(attackType);
			pMsgBU.writeUnsignedInt32(pAttThisID);
			pMsgBU.writeUnsignedInt32(pDefThisID);
		}
	};
}