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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "Common.h"

#include <set>
#include <string>

class MANGOS_DLL_SPEC Object
{
    public:
		Object(){};
        virtual ~Object();

        const bool& IsInWorld() const { return m_inWorld; }
        virtual void AddToWorld()
        {
            if (m_inWorld)
                return;

            m_inWorld = true;
        }
        virtual void RemoveFromWorld()
        {
            m_inWorld = false;
        }

        //ObjectGuid const& GetObjectGuid() const { return GetGuidValue(OBJECT_FIELD_GUID); }
        //uint32 GetGUIDLow() const { return GetObjectGuid().GetCounter(); }
        //PackedGuid const& GetPackGUID() const { return m_PackGUID; }
        //std::string GetGuidStr() const { return GetObjectGuid().GetString(); }

    private:
        bool m_inWorld;

        Object(const Object&);                              // prevent generation copy constructor
        Object& operator=(Object const&);                   // prevent generation assigment operator
};

struct WorldObjectChangeAccumulator;

class MANGOS_DLL_SPEC WorldObject : public Object
{
public:
	WorldObject(){};
    virtual ~WorldObject() {}

    virtual void Update(uint32 /*update_diff*/, uint32 /*time_diff*/) {}

    //void _Create(uint32 guidlow, HighGuid guidhigh, uint32 phaseMask);

    //void SetMap(Map* map);
    //Map* GetMap() const { MANGOS_ASSERT(m_currMap); return m_currMap; }
    // used to check all object's GetMap() calls when object is not in world!
    //void ResetMap() { m_currMap = NULL; }

private:
    //Map* m_currMap;                                     // current object's Map location

    uint32 m_mapId;                                     // object at map with
    //Position m_position;
};

#endif