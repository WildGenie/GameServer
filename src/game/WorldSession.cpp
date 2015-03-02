/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file
    \ingroup u2w
*/

#include "WorldSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "World.h"
#include "BattleGround/BattleGroundMgr.h"
#include "MapManager.h"
#include "SocialMgr.h"
#include "Auth/AuthCrypt.h"
#include "Auth/HMACSHA1.h"
#include "zlib/zlib.h"
#include "MByteBuffer.h"
#include "MNetClientBuffer.h"
#include "MClientProcessData.h"
#include "NetMsgHandle/NetMsgHandleManager.h"
#include "NetMsgHandle/ObjectCmd.h"
#include "NetMsgHandle/UtilMsg.h"

// select opcodes appropriate for processing in Map::Update context for current session state
static bool MapSessionFilterHelper(WorldSession* session, OpcodeHandler const& opHandle)
{
    // we do not process thread-unsafe packets
    if (opHandle.packetProcessing == PROCESS_THREADUNSAFE)
        return false;

    // we do not process not loggined player packets
    Player* plr = session->GetPlayer();
    if (!plr)
        return false;

    // in Map::Update() we do not process packets where player is not in world!
    return plr->IsInWorld();
}

bool MapSessionFilter::Process(WorldPacket* packet)
{
    OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
    if (opHandle.packetProcessing == PROCESS_INPLACE)
        return true;

    // let's check if our opcode can be really processed in Map::Update()
    return MapSessionFilterHelper(m_pSession, opHandle);
}

// we should process ALL packets when player is not in world/logged in
// OR packet handler is not thread-safe!
bool WorldSessionFilter::Process(WorldPacket* packet)
{
    OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
    // check if packet handler is supposed to be safe
    if (opHandle.packetProcessing == PROCESS_INPLACE)
        return true;

    // let's check if our opcode can't be processed in Map::Update()
    return !MapSessionFilterHelper(m_pSession, opHandle);
}

/// WorldSession constructor
WorldSession::WorldSession(uint32 id, const boost::shared_ptr<WorldSocket>& sock, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale) :
    m_muteTime(mute_time), _player(NULL), m_Socket(sock), _security(sec), _accountId(id), m_expansion(expansion), _logoutTime(0),
    m_inQueue(false), m_playerLoading(false), m_playerLogout(false), m_playerRecentlyLogout(false), m_playerSave(false),
    m_sessionDbcLocale(sWorld.GetAvailableDbcLocale(locale)), m_sessionDbLocaleIndex(sObjectMgr.GetIndexForLocale(locale)),
    m_latency(0), m_tutorialState(TUTORIALDATA_UNCHANGED)
{
    if (sock)
    {
        m_Address = sock->GetRemoteAddress();
    }
}

/// WorldSession destructor
WorldSession::~WorldSession()
{
    ///- unload player if not unloaded
    if (_player)
        LogoutPlayer(true);

    /// - If have unclosed socket, close it
    if (m_Socket)
    {
        m_Socket->CloseSocket();
        m_Socket.reset();
    }

    ///- empty incoming packet queue
    WorldPacket* packet = NULL;
    while (_recvQueue.next(packet))
        delete packet;
}

/// Send a packet to the client
void WorldSession::SendPacket(WorldPacket const* packet)
{
    if (!m_Socket)
        return;

#ifdef MANGOS_DEBUG

    // Code for network use statistic
    static uint64 sendPacketCount = 0;
    static uint64 sendPacketBytes = 0;

    static time_t firstTime = time(NULL);
    static time_t lastTime = firstTime;                     // next 60 secs start time

    static uint64 sendLastPacketCount = 0;
    static uint64 sendLastPacketBytes = 0;

    time_t cur_time = time(NULL);

    if ((cur_time - lastTime) < 60)
    {
        sendPacketCount += 1;
        sendPacketBytes += packet->size();

        sendLastPacketCount += 1;
        sendLastPacketBytes += packet->size();
    }
    else
    {
        uint64 minTime = uint64(cur_time - lastTime);
        uint64 fullTime = uint64(lastTime - firstTime);
        DETAIL_LOG("Send all time packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f time: %u", sendPacketCount, sendPacketBytes, float(sendPacketCount) / fullTime, float(sendPacketBytes) / fullTime, uint32(fullTime));
        DETAIL_LOG("Send last min packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f", sendLastPacketCount, sendLastPacketBytes, float(sendLastPacketCount) / minTime, float(sendLastPacketBytes) / minTime);

        lastTime = cur_time;
        sendLastPacketCount = 1;
        sendLastPacketBytes = packet->wpos();               // wpos is real written size
    }

#endif                                                  // !MANGOS_DEBUG

    if ( !m_Socket->SendPacket(*packet) )
        m_Socket->CloseSocket();
}

/// Update the WorldSession (triggered by World update)
bool WorldSession::Update(PacketFilter& updater)
{
    ///- Retrieve packets from the receive queue and call the appropriate handlers
    /// not process packets if socket already closed
    WorldPacket* packet = NULL;
    //while (m_Socket && !m_Socket->IsClosed() && _recvQueue.next(packet, updater))
	if (m_Socket && !m_Socket->IsClosed())
    {
		MByteBuffer* pMsgBA;
		pMsgBA = m_Socket->getNetClientBuffer()->getMsg();	// 获取一个消息
		while (pMsgBA)
		{
			// 进行处理消息
			sNetMsgHandleManager.m_pNetDispHandle->handleMsg(pMsgBA, this);

			// Test 测试发送一个消息
			Cmd::stObjectBasicCmd cmd;
			UtilMsg::sendMsg(m_Socket.get(), &cmd);

			pMsgBA = m_Socket->getNetClientBuffer()->getMsg();	// 获取下一个消息
		}


        /*#if 1
        sLog.outError( "MOEP: %s (0x%.4X)",
                        packet->GetOpcodeName(),
                        packet->GetOpcode());
        #endif*/

        //OpcodeHandler const& opHandle = opcodeTable[packet->GetOpcode()];
        //try
        //{
        //    switch (opHandle.status)
        //    {
        //        case STATUS_LOGGEDIN:
        //            if (!_player)
        //            {
        //                // skip STATUS_LOGGEDIN opcode unexpected errors if player logout sometime ago - this can be network lag delayed packets
        //                if (!m_playerRecentlyLogout)
        //                    LogUnexpectedOpcode(packet, "the player has not logged in yet");
        //            }
        //            else if (_player->IsInWorld())
        //                ExecuteOpcode(opHandle, packet);

        //            // lag can cause STATUS_LOGGEDIN opcodes to arrive after the player started a transfer
        //            break;
        //        case STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT:
        //            if (!_player && !m_playerRecentlyLogout)
        //            {
        //                LogUnexpectedOpcode(packet, "the player has not logged in yet and not recently logout");
        //            }
        //            else
        //                // not expected _player or must checked in packet hanlder
        //                ExecuteOpcode(opHandle, packet);
        //            break;
        //        case STATUS_TRANSFER:
        //            if (!_player)
        //                LogUnexpectedOpcode(packet, "the player has not logged in yet");
        //            else if (_player->IsInWorld())
        //                LogUnexpectedOpcode(packet, "the player is still in world");
        //            else
        //                ExecuteOpcode(opHandle, packet);
        //            break;
        //        case STATUS_AUTHED:
        //            // prevent cheating with skip queue wait
        //            if (m_inQueue)
        //            {
        //                LogUnexpectedOpcode(packet, "the player not pass queue yet");
        //                break;
        //            }

        //            // single from authed time opcodes send in to after logout time
        //            // and before other STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT opcodes.
        //            if (packet->GetOpcode() != CMSG_SET_ACTIVE_VOICE_CHANNEL)
        //                m_playerRecentlyLogout = false;

        //            ExecuteOpcode(opHandle, packet);
        //            break;
        //        case STATUS_NEVER:
        //            sLog.outError("SESSION: received not allowed opcode %s (0x%.4X)",
        //                          packet->GetOpcodeName(),
        //                          packet->GetOpcode());
        //            break;
        //        case STATUS_UNHANDLED:
        //            DEBUG_LOG("SESSION: received not handled opcode %s (0x%.4X)",
        //                      packet->GetOpcodeName(),
        //                      packet->GetOpcode());
        //            break;
        //        default:
        //            sLog.outError("SESSION: received wrong-status-req opcode %s (0x%.4X)",
        //                          packet->GetOpcodeName(),
        //                          packet->GetOpcode());
        //            break;
        //    }
        //}
        //catch (ByteBufferException&)
        //{
        //    sLog.outError("WorldSession::Update ByteBufferException occured while parsing a packet (opcode: %u) from client %s, accountid=%i.",
        //                  packet->GetOpcode(), GetRemoteAddress().c_str(), GetAccountId());
        //    if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
        //    {
        //        DEBUG_LOG("Dumping error causing packet:");
        //        packet->hexlike();
        //    }

        //    if (sWorld.getConfig(CONFIG_BOOL_KICK_PLAYER_ON_BAD_PACKET))
        //    {
        //        DETAIL_LOG("Disconnecting session [account id %u / address %s] for badly formatted packet.",
        //                   GetAccountId(), GetRemoteAddress().c_str());

        //        KickPlayer();
        //    }
        //}

        //delete packet;
    }

    ///- Cleanup socket pointer if need
    if (m_Socket && m_Socket->IsClosed())
    {
        m_Socket.reset();
    }

    // check if we are safe to proceed with logout
    // logout procedure should happen only in World::UpdateSessions() method!!!
    if (updater.ProcessLogout())
    {
        ///- If necessary, log the player out
        time_t currTime = time(NULL);
        if (!m_Socket || (ShouldLogOut(currTime) && !m_playerLoading))
            LogoutPlayer(true);

        if (!m_Socket)
            return false;                                   // Will remove this session from the world session map
    }

    return true;
}