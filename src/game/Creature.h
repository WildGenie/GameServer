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

#ifndef MANGOSSERVER_CREATURE_H
#define MANGOSSERVER_CREATURE_H

#include "Common.h"
#include "Unit.h"
#include "UpdateMask.h"
#include "ItemPrototype.h"
#include "SharedDefines.h"
#include "LootMgr.h"
#include "DBCEnums.h"
#include "Database/DatabaseEnv.h"
#include "Cell.h"

#include <list>

struct SpellEntry;

class CreatureAI;
class Group;
class Quest;
class Player;
class WorldSession;

struct GameEventCreatureData;

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push,N), also any gcc version not support it at some platform
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

class MANGOS_DLL_SPEC Creature : public Unit
{
    CreatureAI* i_AI;

public:

    explicit Creature(CreatureSubtype subtype = CREATURE_SUBTYPE_GENERIC);
    virtual ~Creature();

    void AddToWorld() override;
    void RemoveFromWorld() override;

    bool Create(uint32 guidlow, CreatureCreatePos& cPos, CreatureInfo const* cinfo, Team team = TEAM_NONE, const CreatureData* data = NULL, GameEventCreatureData const* eventData = NULL);

    void Update(uint32 update_diff, uint32 time) override;  // overwrite Unit::Update

    CreatureAI* AI() { return i_AI; }

protected:
    //Cell m_currentCell;                                 // store current cell where creature listed

private:
    //GridReference<Creature> m_gridRef;
    //CreatureInfo const* m_creatureInfo;                 // in difficulty mode > 0 can different from ObjMgr::GetCreatureTemplate(GetEntry())
};

#endif