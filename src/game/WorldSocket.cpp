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
#include "Database/DatabaseEnv.h"
#include "WorldSession.h"
#include "WorldSocketMgr.h"
#include "Log.h"

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
    m_Seed(static_cast<uint32>(rand32()))
{

}

WorldSocket::~WorldSocket(void)
{
	
}

void WorldSocket::CloseSocket(void)
{
    {
        GuardType Guard( m_SessionLock );

        m_Session = NULL;
    }

    Socket::CloseSocket();
}

bool WorldSocket::SendPacket()
{
    if (IsClosed())
        return false;

    start_async_send();

    return true;
}

bool WorldSocket::open()
{
   if( !Socket::open() )
       return false;

   return true;
}

int WorldSocket::HandlePing()
{
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