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

#include "Unit.h"
#include "Log.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Creature.h"
#include "ObjectAccessor.h"
#include "Util.h"

#include <math.h>
#include <stdarg.h>

////////////////////////////////////////////////////////////
// Methods of class Unit

Unit::Unit()
{
    
}

Unit::~Unit()
{
    
}
//
//void Unit::Update(uint32 update_diff, uint32 p_time)
//{
//    if (!IsInWorld())
//        return;
//
//    /*if(p_time > m_AurasCheck)
//    {
//    m_AurasCheck = 2000;
//    _UpdateAura();
//    }else
//    m_AurasCheck -= p_time;*/
//
//    // WARNING! Order of execution here is important, do not change.
//    // Spells must be processed with event system BEFORE they go to _UpdateSpells.
//    // Or else we may have some SPELL_STATE_FINISHED spells stalled in pointers, that is bad.
//}