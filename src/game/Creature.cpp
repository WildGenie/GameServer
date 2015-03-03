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

#include "Creature.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Log.h"
#include "Util.h"

// apply implementation of the singletons
#include "Policies/Singleton.h"

//Creature::Creature(CreatureSubtype subtype) : Unit(),
//    i_AI(NULL),
//    loot(this),
//    lootForPickPocketed(false), lootForBody(false), lootForSkin(false),
//    m_groupLootTimer(0), m_groupLootId(0),
//    m_lootMoney(0), m_lootGroupRecipientId(0),
//    m_corpseDecayTimer(0), m_respawnTime(0), m_respawnDelay(25), m_corpseDelay(60), m_respawnradius(5.0f),
//    m_subtype(subtype), m_defaultMovementType(IDLE_MOTION_TYPE), m_equipmentId(0),
//    m_AlreadyCallAssistance(false), m_AlreadySearchedAssistance(false),
//    m_regenHealth(true), m_AI_locked(false), m_isDeadByDefault(false),
//    m_temporaryFactionFlags(TEMPFACTION_NONE),
//    m_meleeDamageSchoolMask(SPELL_SCHOOL_MASK_NORMAL), m_originalEntry(0),
//    m_creatureInfo(NULL)
//{
//    
//}

Creature::~Creature()
{
    delete i_AI;
    i_AI = NULL;
}

void Creature::AddToWorld()
{
   /* ///- Register the creature for guid lookup
    if (!IsInWorld() && GetObjectGuid().IsCreatureOrVehicle())
        GetMap()->GetObjectsStore().insert<Creature>(GetObjectGuid(), (Creature*)this);

    Unit::AddToWorld();*/
}

void Creature::RemoveFromWorld()
{
    /*///- Remove the creature from the accessor
    if (IsInWorld() && GetObjectGuid().IsCreatureOrVehicle())
        GetMap()->GetObjectsStore().erase<Creature>(GetObjectGuid(), (Creature*)NULL);

    Unit::RemoveFromWorld();*/
}

void Creature::Update(uint32 update_diff, uint32 diff)
{

}