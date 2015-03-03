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

#include "Common.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "Database/DatabaseEnv.h"
#include "DynamicObject.h"

DynamicObject::DynamicObject() : WorldObject()
{
   
}

void DynamicObject::AddToWorld()
{
    /////- Register the dynamicObject for guid lookup
    //if (!IsInWorld())
    //    GetMap()->GetObjectsStore().insert<DynamicObject>(GetObjectGuid(), (DynamicObject*)this);

    //Object::AddToWorld();
}

void DynamicObject::RemoveFromWorld()
{
    /////- Remove the dynamicObject from the accessor
    //if (IsInWorld())
    //{
    //    GetMap()->GetObjectsStore().erase<DynamicObject>(GetObjectGuid(), (DynamicObject*)NULL);
    //    GetViewPoint().Event_RemovedFromWorld();
    //}

    //Object::RemoveFromWorld();
}

//bool DynamicObject::Create(uint32 guidlow, Unit* caster, uint32 spellId, SpellEffectIndex effIndex, float x, float y, float z, int32 duration, float radius, DynamicObjectType type)
//{
//    return true;
//}

void DynamicObject::Update(uint32 /*update_diff*/, uint32 p_time)
{
    
}

void DynamicObject::Delete()
{

}