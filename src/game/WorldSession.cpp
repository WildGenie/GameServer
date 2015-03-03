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
#include "WorldSession.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Auth/AuthCrypt.h"
#include "Auth/HMACSHA1.h"
#include "zlib/zlib.h"
#include "MByteBuffer.h"
#include "MNetClientBuffer.h"
#include "MClientProcessData.h"
#include "NetMsgHandle/NetMsgHandleManager.h"
#include "NetMsgHandle/ObjectCmd.h"
#include "NetMsgHandle/UtilMsg.h"

/// WorldSession constructor
WorldSession::WorldSession(uint32 id, const boost::shared_ptr<WorldSocket>& sock, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale) :
     _player(NULL), m_Socket(sock),  _logoutTime(0)
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
    //if (_player)
    //    LogoutPlayer(true);

    /// - If have unclosed socket, close it
    if (m_Socket)
    {
        m_Socket->CloseSocket();
        m_Socket.reset();
    }
}

/// Send a packet to the client
void WorldSession::SendPacket()
{
    if (!m_Socket)
        return;

    if ( !m_Socket->SendPacket() )
        m_Socket->CloseSocket();
}

/// Update the WorldSession (triggered by World update)
bool WorldSession::Update()
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

    return true;
}