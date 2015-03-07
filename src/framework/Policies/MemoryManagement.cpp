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

// lets use Intel scalable_allocator by default and
// switch to OS specific allocator only when _STANDARD_MALLOC is defined
#ifndef USE_STANDARD_MALLOC

#define REPLACE_SYSTEM_ALLOCATOR
#define NO_NED_NAMESPACE

#include "nedmalloc.h"

void* operator new(size_t sz)
{
	void* res = nedmalloc(sz);

    if (res == NULL)
        throw std::bad_alloc();

    return res;
}

void* operator new[](size_t sz)
{
	void* res = nedmalloc(sz);

    if (res == NULL)
        throw std::bad_alloc();

    return res;
}

void operator delete(void* ptr) throw()
{
	nedfree(ptr);
}

void operator delete[](void* ptr) throw()
{
	nedfree(ptr);
}

void* operator new(size_t sz, const std::nothrow_t&) throw()
{
	return nedmalloc(sz);
}

void* operator new[](size_t sz, const std::nothrow_t&) throw()
{
	return nedmalloc(sz);
}

void operator delete(void* ptr, const std::nothrow_t&) throw()
{
	nedfree(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) throw()
{
	nedfree(ptr);
}

#endif
