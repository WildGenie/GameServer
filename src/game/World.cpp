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
    \ingroup world
*/

#include "World.h"
#include "Database/DatabaseEnv.h"
#include "Config/Config.h"
#include "Platform/Define.h"
#include "SystemConfig.h"
#include "Log.h"
#include "WorldSession.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Policies/Singleton.h"
#include "Database/DatabaseImpl.h"
#include "Util.h"

INSTANTIATE_SINGLETON_1(World);

/// World constructor
World::World()
{
    
}

/// World destructor
World::~World()
{
    ///- Empty the kicked session set
    while (!m_sessions.empty())
    {
        // not remove from queue, prevent loading new sessions
        delete m_sessions.begin()->second;
        m_sessions.erase(m_sessions.begin());
    }
}

/// Find a session by its id
WorldSession* World::FindSession(uint32 id) const
{
    SessionMap::const_iterator itr = m_sessions.find(id);

    if (itr != m_sessions.end())
        return itr->second;                                 // also can return NULL for kicked session
    else
        return NULL;
}

/// Remove a given session
bool World::RemoveSession(uint32 id)
{
    ///- Find the session, kick the user, but we can't delete session at this moment to prevent iterator invalidation
    SessionMap::const_iterator itr = m_sessions.find(id);

    //if (itr != m_sessions.end() && itr->second)
    //{
    //    if (itr->second->PlayerLoading())
    //        return false;
    //    itr->second->KickPlayer();
    //}

    return true;
}

void World::AddSession(WorldSession* s)
{
    addSessQueue.add(s);
}

void
World::AddSession_(WorldSession* s)
{
    //MANGOS_ASSERT(s);

    //// NOTE - Still there is race condition in WorldSession* being used in the Sockets

    /////- kick already loaded player with same account (if any) and remove session
    /////- if player is in loading and want to load again, return
    //if (!RemoveSession(s->GetAccountId()))
    //{
    //    s->KickPlayer();
    //    delete s;                                           // session not added yet in session list, so not listed in queue
    //    return;
    //}

    //// decrease session counts only at not reconnection case
    //bool decrease_session = true;

    //// if session already exist, prepare to it deleting at next world update
    //// NOTE - KickPlayer() should be called on "old" in RemoveSession()
    //{
    //    SessionMap::const_iterator old = m_sessions.find(s->GetAccountId());

    //    if (old != m_sessions.end())
    //    {
    //        // prevent decrease sessions count if session queued
    //        if (RemoveQueuedSession(old->second))
    //            decrease_session = false;
    //        // not remove replaced session form queue if listed
    //        delete old->second;
    //    }
    //}

    //m_sessions[s->GetAccountId()] = s;
}

int32 World::GetQueuedSessionPos(WorldSession* sess)
{
    uint32 position = 1;

    for (Queue::const_iterator iter = m_QueuedSessions.begin(); iter != m_QueuedSessions.end(); ++iter, ++position)
        if ((*iter) == sess)
            return position;

    return 0;
}

void World::AddQueuedSession(WorldSession* sess)
{
    m_QueuedSessions.push_back(sess);
}

bool World::RemoveQueuedSession(WorldSession* sess)
{
    //// sessions count including queued to remove (if removed_session set)
    //uint32 sessions = GetActiveSessionCount();

    //uint32 position = 1;
    //Queue::iterator iter = m_QueuedSessions.begin();

    //// search to remove and count skipped positions
    //bool found = false;

    //for (; iter != m_QueuedSessions.end(); ++iter, ++position)
    //{
    //    if (*iter == sess)
    //    {
    //        sess->SetInQueue(false);
    //        iter = m_QueuedSessions.erase(iter);
    //        found = true;                                   // removing queued session
    //        break;
    //    }
    //}

    //// iter point to next socked after removed or end()
    //// position store position of removed socket and then new position next socket after removed

    //// if session not queued then we need decrease sessions count
    //if (!found && sessions)
    //    --sessions;

    //// accept first in queue
    //if ((!m_playerLimit || (int32)sessions < m_playerLimit) && !m_QueuedSessions.empty())
    //{
    //    WorldSession* pop_sess = m_QueuedSessions.front();
    //    pop_sess->SetInQueue(false);
    //    pop_sess->SendAuthWaitQue(0);
    //    pop_sess->SendAddonsInfo();

    //    WorldPacket pkt(SMSG_CLIENTCACHE_VERSION, 4);
    //    pkt << uint32(getConfig(CONFIG_UINT32_CLIENTCACHE_VERSION));
    //    pop_sess->SendPacket(&pkt);

    //    pop_sess->SendAccountDataTimes(GLOBAL_CACHE_MASK);
    //    pop_sess->SendTutorialsData();

    //    m_QueuedSessions.pop_front();

    //    // update iter to point first queued socket or end() if queue is empty now
    //    iter = m_QueuedSessions.begin();
    //    position = 1;
    //}

    //// update position from iter to end()
    //// iter point to first not updated socket, position store new position
    //for (; iter != m_QueuedSessions.end(); ++iter, ++position)
    //    (*iter)->SendAuthWaitQue(position);

    //return found;

	return true;
}

/// Update the World !
void World::Update(uint32 diff)
{
    /// <li> Handle session updates
    UpdateSessions(diff);
}

void World::UpdateSessions(uint32 diff)
{
    ///- Add new sessions
    WorldSession* sess;
    while (addSessQueue.next(sess))
        AddSession_(sess);

    ///- Then send an update signal to remaining ones
    for (SessionMap::iterator itr = m_sessions.begin(), next; itr != m_sessions.end(); itr = next)
    {
        next = itr;
        ++next;
        ///- and remove not active sessions from the list
        WorldSession* pSession = itr->second;
        //WorldSessionFilter updater(pSession);

        //if (!pSession->Update(updater))
        //{
        //    RemoveQueuedSession(pSession);
        //    m_sessions.erase(itr);
        //    delete pSession;
        //}
    }
}