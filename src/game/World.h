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

/// \addtogroup world The World
/// @{
/// \file

#ifndef __WORLD_H
#define __WORLD_H

#include "Common.h"
#include "Timer.h"
#include "Policies/Singleton.h"

#include <boost/thread/mutex.hpp>

#include <map>
#include <set>
#include <list>

class Object;
class WorldPacket;
class WorldSession;
class Player;
class Weather;
class SqlResultQueue;
class QueryResult;
class WorldSocket;

/// The World
class World
{
public:
    World();
    ~World();

    WorldSession* FindSession(uint32 id) const;
    void AddSession(WorldSession* s);
    bool RemoveSession(uint32 id);

    // player Queue
    typedef std::list<WorldSession*> Queue;
    void AddQueuedSession(WorldSession*);
    bool RemoveQueuedSession(WorldSession* session);
    int32 GetQueuedSessionPos(WorldSession*);

    void Update(uint32 diff);

    void UpdateSessions(uint32 diff);

    typedef UNORDERED_MAP<uint32, WorldSession*> SessionMap;
    SessionMap m_sessions;

    // Player Queue
    Queue m_QueuedSessions;

    // sessions that are added async
    void AddSession_(WorldSession* s);
    MaNGOS::LockedQueue<WorldSession*, boost::mutex> addSessQueue;
};

extern uint32 realmID;

#define sWorld MaNGOS::Singleton<World>::Instance()
#endif
/// @}
