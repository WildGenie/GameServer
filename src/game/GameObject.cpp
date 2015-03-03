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

#include "GameObject.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Database/DatabaseEnv.h"
#include "Util.h"
#include <G3D/Quat.h>

GameObject::GameObject() : WorldObject()
{
    
}

GameObject::~GameObject()
{
    
}

void GameObject::AddToWorld()
{
    /////- Register the gameobject for guid lookup
    //if (!IsInWorld())
    //    GetMap()->GetObjectsStore().insert<GameObject>(GetObjectGuid(), (GameObject*)this);

    //if (m_model)
    //    GetMap()->InsertGameObjectModel(*m_model);

    //Object::AddToWorld();

    //// After Object::AddToWorld so that for initial state the GO is added to the world (and hence handled correctly)
    //UpdateCollisionState();
}

void GameObject::RemoveFromWorld()
{
    /////- Remove the gameobject from the accessor
    //if (IsInWorld())
    //{
    //    // Notify the outdoor pvp script
    //    if (OutdoorPvP* outdoorPvP = sOutdoorPvPMgr.GetScript(GetZoneId()))
    //        outdoorPvP->HandleGameObjectRemove(this);

    //    // Remove GO from owner
    //    if (ObjectGuid owner_guid = GetOwnerGuid())
    //    {
    //        if (Unit* owner = ObjectAccessor::GetUnit(*this, owner_guid))
    //            owner->RemoveGameObject(this, false);
    //        else
    //        {
    //            sLog.outError("Delete %s with SpellId %u LinkedGO %u that lost references to owner %s GO list. Crash possible later.",
    //                          GetGuidStr().c_str(), m_spellId, GetGOInfo()->GetLinkedGameObjectEntry(), owner_guid.GetString().c_str());
    //        }
    //    }

    //    if (m_model && GetMap()->ContainsGameObjectModel(*m_model))
    //        GetMap()->RemoveGameObjectModel(*m_model);

    //    GetMap()->GetObjectsStore().erase<GameObject>(GetObjectGuid(), (GameObject*)NULL);
    //}

    //Object::RemoveFromWorld();
}

//bool GameObject::Create(uint32 guidlow, uint32 name_id, Map* map, uint32 phaseMask, float x, float y, float z, float ang, QuaternionData rotation, uint8 animprogress, GOState go_state)
//{
//    
//
//    return true;
//}

void GameObject::Update(uint32 update_diff, uint32 p_time)
{
    //if (GetObjectGuid().IsMOTransport())
    //{
    //    //((Transport*)this)->Update(p_time);
    //    return;
    //}

    //switch (m_lootState)
    //{
    //    case GO_NOT_READY:
    //    {
    //        switch (GetGoType())
    //        {
    //            case GAMEOBJECT_TYPE_TRAP:                  // Initialized delayed to be able to use GetOwner()
    //            {
    //                // Arming Time for GAMEOBJECT_TYPE_TRAP (6)
    //                Unit* owner = GetOwner();
    //                if (owner && owner->isInCombat())
    //                    m_cooldownTime = time(NULL) + GetGOInfo()->trap.startDelay;
    //                m_lootState = GO_READY;
    //                break;
    //            }
    //            case GAMEOBJECT_TYPE_FISHINGNODE:           // Keep not ready for some delay
    //            {
    //                // fishing code (bobber ready)
    //                if (time(NULL) > m_respawnTime - FISHING_BOBBER_READY_TIME)
    //                {
    //                    // splash bobber (bobber ready now)
    //                    Unit* caster = GetOwner();
    //                    if (caster && caster->GetTypeId() == TYPEID_PLAYER)
    //                    {
    //                        SetGoState(GO_STATE_ACTIVE);
    //                        // SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NODESPAWN);

    //                        SendForcedObjectUpdate();

    //                        SendGameObjectCustomAnim(GetObjectGuid());
    //                    }

    //                    m_lootState = GO_READY;             // can be successfully open with some chance
    //                }
    //                break;
    //            }
    //        }
    //        break;
    //    }
    //    case GO_READY:
    //    {
    //        if (m_respawnTime > 0)                          // timer on
    //        {
    //            if (m_respawnTime <= time(NULL))            // timer expired
    //            {
    //                m_respawnTime = 0;
    //                ClearAllUsesData();

    //                switch (GetGoType())
    //                {
    //                    case GAMEOBJECT_TYPE_FISHINGNODE:   // can't fish now
    //                    {
    //                        Unit* caster = GetOwner();
    //                        if (caster && caster->GetTypeId() == TYPEID_PLAYER)
    //                        {
    //                            caster->FinishSpell(CURRENT_CHANNELED_SPELL);

    //                            WorldPacket data(SMSG_FISH_NOT_HOOKED, 0);
    //                            ((Player*)caster)->GetSession()->SendPacket(&data);
    //                        }
    //                        // can be deleted
    //                        m_lootState = GO_JUST_DEACTIVATED;
    //                        return;
    //                    }
    //                    case GAMEOBJECT_TYPE_DOOR:
    //                    case GAMEOBJECT_TYPE_BUTTON:
    //                        // we need to open doors if they are closed (add there another condition if this code breaks some usage, but it need to be here for battlegrounds)
    //                        if (GetGoState() != GO_STATE_READY)
    //                            ResetDoorOrButton();
    //                        // flags in AB are type_button and we need to add them here so no break!
    //                    default:
    //                        if (!m_spawnedByDefault)        // despawn timer
    //                        {
    //                            // can be despawned or destroyed
    //                            SetLootState(GO_JUST_DEACTIVATED);
    //                            // Remove Wild-Summoned GO on timer expire
    //                            if (!HasStaticDBSpawnData())
    //                            {
    //                                if (Unit* owner = GetOwner())
    //                                    owner->RemoveGameObject(this, false);
    //                                Delete();
    //                            }
    //                            return;
    //                        }

    //                        // respawn timer
    //                        GetMap()->Add(this);
    //                        break;
    //                }
    //            }
    //        }

    //        if (isSpawned())
    //        {
    //            // traps can have time and can not have
    //            GameObjectInfo const* goInfo = GetGOInfo();
    //            if (goInfo->type == GAMEOBJECT_TYPE_TRAP)   // traps
    //            {
    //                if (m_cooldownTime >= time(NULL))
    //                    return;

    //                // FIXME: this is activation radius (in different casting radius that must be selected from spell data)
    //                // TODO: move activated state code (cast itself) to GO_ACTIVATED, in this place only check activating and set state
    //                float radius = float(goInfo->trap.radius);
    //                if (!radius)
    //                {
    //                    if (goInfo->trap.cooldown != 3)     // cast in other case (at some triggering/linked go/etc explicit call)
    //                        return;
    //                    else
    //                    {
    //                        if (m_respawnTime > 0)
    //                            break;

    //                        // battlegrounds gameobjects has data2 == 0 && data5 == 3
    //                        radius = float(goInfo->trap.cooldown);
    //                    }
    //                }

    //                // Should trap trigger?
    //                Unit* enemy = NULL;                     // pointer to appropriate target if found any
    //                MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck u_check(this, radius);
    //                MaNGOS::UnitSearcher<MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck> checker(enemy, u_check);
    //                Cell::VisitAllObjects(this, checker, radius);
    //                if (enemy)
    //                    Use(enemy);
    //            }

    //            if (uint32 max_charges = goInfo->GetCharges())
    //            {
    //                if (m_useTimes >= max_charges)
    //                {
    //                    m_useTimes = 0;
    //                    SetLootState(GO_JUST_DEACTIVATED);  // can be despawned or destroyed
    //                }
    //            }
    //        }
    //        break;
    //    }
    //    case GO_ACTIVATED:
    //    {
    //        switch (GetGoType())
    //        {
    //            case GAMEOBJECT_TYPE_DOOR:
    //            case GAMEOBJECT_TYPE_BUTTON:
    //                if (GetGOInfo()->GetAutoCloseTime() && (m_cooldownTime < time(NULL)))
    //                    ResetDoorOrButton();
    //                break;
    //            case GAMEOBJECT_TYPE_CHEST:
    //                if (m_groupLootId)
    //                {
    //                    if (m_groupLootTimer <= update_diff)
    //                        StopGroupLoot();
    //                    else
    //                        m_groupLootTimer -= update_diff;
    //                }
    //                break;
    //            case GAMEOBJECT_TYPE_GOOBER:
    //                if (m_cooldownTime < time(NULL))
    //                {
    //                    RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);

    //                    SetLootState(GO_JUST_DEACTIVATED);
    //                    m_cooldownTime = 0;
    //                }
    //                break;
    //            case GAMEOBJECT_TYPE_CAPTURE_POINT:
    //                m_captureTimer += p_time;
    //                if (m_captureTimer >= 5000)
    //                {
    //                    TickCapturePoint();
    //                    m_captureTimer -= 5000;
    //                }
    //                break;
    //            default:
    //                break;
    //        }
    //        break;
    //    }
    //    case GO_JUST_DEACTIVATED:
    //    {
    //        switch (GetGoType())
    //        {
    //            case GAMEOBJECT_TYPE_GOOBER:
    //                // if gameobject should cast spell, then this, but some GOs (type = 10) should be destroyed
    //                if (uint32 spellId = GetGOInfo()->goober.spellId)
    //                {
    //                    for (GuidSet::const_iterator itr = m_UniqueUsers.begin(); itr != m_UniqueUsers.end(); ++itr)
    //                    {
    //                        if (Player* owner = GetMap()->GetPlayer(*itr))
    //                            owner->CastSpell(owner, spellId, false, NULL, NULL, GetObjectGuid());
    //                    }

    //                    ClearAllUsesData();
    //                }

    //                SetGoState(GO_STATE_READY);

    //                // any return here in case battleground traps
    //                break;
    //            case GAMEOBJECT_TYPE_CAPTURE_POINT:
    //                // remove capturing players because slider wont be displayed if capture point is being locked
    //                for (GuidSet::const_iterator itr = m_UniqueUsers.begin(); itr != m_UniqueUsers.end(); ++itr)
    //                {
    //                    if (Player* owner = GetMap()->GetPlayer(*itr))
    //                        owner->SendUpdateWorldState(GetGOInfo()->capturePoint.worldState1, WORLD_STATE_REMOVE);
    //                }

    //                m_UniqueUsers.clear();
    //                SetLootState(GO_READY);
    //                return; // SetLootState and return because go is treated as "burning flag" due to GetGoAnimProgress() being 100 and would be removed on the client
    //            default:
    //                break;
    //        }

    //        // Remove wild summoned after use
    //        if (!HasStaticDBSpawnData() && (!GetSpellId() || GetGOInfo()->GetDespawnPossibility()))
    //        {
    //            if (Unit* owner = GetOwner())
    //                owner->RemoveGameObject(this, false);
    //            Delete();
    //            return;
    //        }

    //        // burning flags in some battlegrounds, if you find better condition, just add it
    //        if (GetGOInfo()->IsDespawnAtAction() || GetGoAnimProgress() > 0)
    //        {
    //            SendObjectDeSpawnAnim(GetObjectGuid());
    //            // reset flags
    //            if (GetMap()->Instanceable())
    //            {
    //                // In Instances GO_FLAG_LOCKED, GO_FLAG_INTERACT_COND or GO_FLAG_NO_INTERACT are not changed
    //                uint32 currentLockOrInteractFlags = GetUInt32Value(GAMEOBJECT_FLAGS) & (GO_FLAG_LOCKED | GO_FLAG_INTERACT_COND | GO_FLAG_NO_INTERACT);
    //                SetUInt32Value(GAMEOBJECT_FLAGS, GetGOInfo()->flags & ~(GO_FLAG_LOCKED | GO_FLAG_INTERACT_COND | GO_FLAG_NO_INTERACT) | currentLockOrInteractFlags);
    //            }
    //            else
    //                SetUInt32Value(GAMEOBJECT_FLAGS, GetGOInfo()->flags);
    //        }

    //        loot.clear();
    //        SetLootRecipient(NULL);
    //        SetLootState(GO_READY);

    //        if (!m_respawnDelayTime)
    //            return;

    //        // since pool system can fail to roll unspawned object, this one can remain spawned, so must set respawn nevertheless
    //        m_respawnTime = m_spawnedByDefault ? time(NULL) + m_respawnDelayTime : 0;

    //        // if option not set then object will be saved at grid unload
    //        if (sWorld.getConfig(CONFIG_BOOL_SAVE_RESPAWN_TIME_IMMEDIATELY))
    //            SaveRespawnTime();

    //        // if part of pool, let pool system schedule new spawn instead of just scheduling respawn
    //        if (uint16 poolid = sPoolMgr.IsPartOfAPool<GameObject>(GetGUIDLow()))
    //            sPoolMgr.UpdatePool<GameObject>(*GetMap()->GetPersistentState(), poolid, GetGUIDLow());

    //        // can be not in world at pool despawn
    //        if (IsInWorld())
    //            UpdateObjectVisibility();

    //        break;
    //    }
    //}
}