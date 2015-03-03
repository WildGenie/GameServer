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

#ifndef MANGOSSERVER_GAMEOBJECT_H
#define MANGOSSERVER_GAMEOBJECT_H

#include "Common.h"
#include "Object.h"
#include "Database/DatabaseEnv.h"

class MANGOS_DLL_SPEC GameObject : public WorldObject
{
public:
    explicit GameObject();
    ~GameObject();

    void AddToWorld() override;
    void RemoveFromWorld() override;

    //bool Create(uint32 guidlow, uint32 name_id, Map* map, uint32 phaseMask, float x, float y, float z, float ang,
    //            QuaternionData rotation = QuaternionData(), uint8 animprogress = GO_ANIMPROGRESS_DEFAULT, GOState go_state = GO_STATE_READY);
    void Update(uint32 update_diff, uint32 p_time) override;
};

#endif
