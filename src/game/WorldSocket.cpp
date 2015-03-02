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

#include "WorldSocket.h"
#include "Common.h"

#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "SharedDefines.h"
#include "ByteBuffer.h"
#include "Opcodes.h"
#include "Database/DatabaseEnv.h"
#include "Auth/Sha1.h"
#include "WorldSession.h"
#include "WorldSocketMgr.h"
#include "Log.h"
#include "DBCStores.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

WorldSocket::WorldSocket( NetworkManager& socketMrg, 
                          NetworkThread& owner ) :
    Socket( socketMrg, owner ),
    //m_LastPingTime(ACE_Time_Value::zero),
    m_OverSpeedPings(0),
    m_Session(0),
    m_RecvWPct(0),
    m_RecvPct(),
    m_Header(sizeof(ClientPktHeader)),
    m_Seed(static_cast<uint32>(rand32()))
{

}

WorldSocket::~WorldSocket(void)
{
    delete m_RecvWPct;
}

void WorldSocket::CloseSocket(void)
{
    {
        GuardType Guard( m_SessionLock );

        m_Session = NULL;
    }

    Socket::CloseSocket();
}

bool WorldSocket::SendPacket(const WorldPacket& pct)
{
    if (IsClosed())
        return false;

    // Dump outgoing packet.
    sLog.outWorldPacketDump( native_handle(), pct.GetOpcode(), pct.GetOpcodeName(), &pct, false);

    ServerPktHeader header(pct.size() + 2, pct.GetOpcode());
    m_Crypt.EncryptSend((uint8*)header.header, header.getHeaderLength());

    GuardType Guard( m_OutBufferLock );

	// TEST
    //if (m_OutBuffer->space() >= pct.size() + header.getHeaderLength())
    //{
    //    // Put the packet on the buffer.
    //    if (m_OutBuffer->copy((char*) header.header, header.getHeaderLength()) == -1)
    //        MANGOS_ASSERT(false);

    //    if (!pct.empty() && (m_OutBuffer->copy((char*) pct.contents(), pct.size()) == -1))
    //        MANGOS_ASSERT(false);
    //}
    //else
    //{
    //    // Enqueue the packet.
    //    throw std::exception("network write buffer is too small to accommodate packet");
    //}

    start_async_send();

    return true;
}

bool WorldSocket::open()
{
   if( !Socket::open() )
       return false;

    // Send startup packet.
    WorldPacket packet(SMSG_AUTH_CHALLENGE, 40);
    packet << uint32(1);                                    // 1...31
    packet << m_Seed;

    BigNumber seed1;
    seed1.SetRand(16 * 8);
    packet.append(seed1.AsByteArray(16), 16);               // new encryption seeds

    BigNumber seed2;
    seed2.SetRand(16 * 8);
    packet.append(seed2.AsByteArray(16), 16);               // new encryption seeds

    return SendPacket(packet);
}

int WorldSocket::HandlePing(WorldPacket& recvPacket)
{
    uint32 ping;
    uint32 latency;

    // Get the ping packet content
    recvPacket >> ping;
    recvPacket >> latency;

    //if (m_LastPingTime == ACE_Time_Value::zero)
    //    m_LastPingTime = ACE_OS::gettimeofday();            // for 1st ping
	if (!m_bTimeInited)
	{
		m_LastPingTime = boost::posix_time::microsec_clock::universal_time();            // for 1st ping
	}
    else
    {
        //ACE_Time_Value cur_time = ACE_OS::gettimeofday();
        //ACE_Time_Value diff_time(cur_time);
        //diff_time -= m_LastPingTime;
        //m_LastPingTime = cur_time;

		boost::posix_time::ptime cur_time = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::millisec_posix_time_system_config::time_duration_type diff_time;
		diff_time = cur_time - m_LastPingTime;
		m_LastPingTime = cur_time;

        //if (diff_time < ACE_Time_Value(27))
		if (diff_time.total_microseconds() < 27 * 1000)
        {
            ++m_OverSpeedPings;

            uint32 max_count = sWorld.getConfig(CONFIG_UINT32_MAX_OVERSPEED_PINGS);

            if (max_count && m_OverSpeedPings > max_count)
            {
                GuardType Guard(  m_SessionLock );

                if (m_Session && m_Session->GetSecurity() == SEC_PLAYER)
                {
                    sLog.outError("WorldSocket::HandlePing: Player kicked for "
                                  "overspeeded pings address = %s",
                                  GetRemoteAddress().c_str());

                    return -1;
                }
            }
        }
        else
            m_OverSpeedPings = 0;
    }

    // critical section
    {
        GuardType Guard(  m_SessionLock );

        if (m_Session)
            m_Session->SetLatency(latency);
        else
        {
            sLog.outError("WorldSocket::HandlePing: peer sent CMSG_PING, "
                          "but is not authenticated or got recently kicked,"
                          " address = %s",
                          GetRemoteAddress().c_str());
            return -1;
        }
    }

    WorldPacket packet(SMSG_PONG, 4);
    packet << ping;
    
    if( !SendPacket(packet) )
        return -1;

    return 0;
}

// Test สนำร
void WorldSocket::addSession()
{
	WorldSocketPtr this_session = boost::static_pointer_cast<WorldSocket>(shared_from_this());
	// NOTE ATM the socket is single-threaded, have this in mind ...
	m_Session = new WorldSession(1, this_session, AccountTypes(1), 1, 1, LocaleConstant(1));

	//m_Session->LoadGlobalAccountData();
	//m_Session->LoadTutorialsData();

	boost::this_thread::sleep(boost::posix_time::milliseconds(10));

	sWorld.AddSession(m_Session);
}