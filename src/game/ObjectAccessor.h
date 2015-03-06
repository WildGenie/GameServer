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

#ifndef MANGOS_OBJECTACCESSOR_H
#define MANGOS_OBJECTACCESSOR_H

#include "Common.h"
#include "Platform/Define.h"
#include "Policies/Singleton.h"
#include "Utilities/UnorderedMapSet.h"
#include "Policies/ThreadingModel.h"

#include "Object.h"
#include "Player.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>

#include <set>
#include <list>

class Unit;
class WorldObject;

class MANGOS_DLL_DECL ObjectAccessor : public MaNGOS::Singleton<ObjectAccessor, MaNGOS::ClassLevelLockable<ObjectAccessor, boost::mutex> >
{
    friend class MaNGOS::OperatorNew<ObjectAccessor>;

    ObjectAccessor();
    ~ObjectAccessor();
    ObjectAccessor(const ObjectAccessor&);
    ObjectAccessor& operator=(const ObjectAccessor&);

private:
    typedef boost::mutex LockType;
    typedef MaNGOS::GeneralLock<LockType > Guard;
};

#define sObjectAccessor ObjectAccessor::Instance()

#endif
