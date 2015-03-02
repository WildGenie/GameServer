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

/// \addtogroup u2w
/// @{
/// \file

#ifndef __WORLDSESSION_H
#define __WORLDSESSION_H

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "Common.h"
#include "SharedDefines.h"
#include "ObjectGuid.h"
#include "AuctionHouseMgr.h"
#include "Item.h"

struct ItemPrototype;
struct AuctionEntry;
struct AuctionHouseEntry;
struct DeclinedName;

class ObjectGuid;
class Creature;
class Item;
class Object;
class Player;
class Unit;
class WorldPacket;
class WorldSocket;
class QueryResult;
class LoginQueryHolder;
class CharacterHandler;
class GMTicket;
class MovementInfo;
class WorldSession;

struct OpcodeHandler;

/// Player session in the World
class MANGOS_DLL_SPEC WorldSession
{
    friend class CharacterHandler;

public:
    WorldSession(uint32 id, const boost::shared_ptr<WorldSocket>& sock, AccountTypes sec, uint8 expansion, time_t mute_time, LocaleConstant locale);
    ~WorldSession();

    void SendPacket(WorldPacket const* packet);
    bool Update(PacketFilter& updater);

private:
    Player* _player;
    boost::shared_ptr<WorldSocket> m_Socket;
    std::string m_Address;

    time_t _logoutTime;
};
#endif