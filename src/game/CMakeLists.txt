#
# This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

set(LIBRARY_NAME game)

set(Object_SRCS
    Creature.cpp
    Creature.h
    DynamicObject.cpp
    DynamicObject.h
    GameObject.cpp
    GameObject.h
    Object.cpp
    Object.h
    ObjectAccessor.cpp
    ObjectAccessor.h
    ObjectMgr.cpp
    ObjectMgr.h
	pchdef.cpp
    pchdef.h
    Player.cpp
    Player.h
    Unit.cpp
    Unit.h
	)
	
set(Server_SRCS
    WorldSession.cpp
    WorldSession.h
    WorldSocket.cpp
    WorldSocket.h
    WorldSocketMgr.cpp
    WorldSocketMgr.h
	)
	
set(World_SRCS
    World.cpp
    World.h
	)

set(Verify_SRCS
	Verify/VerifyThread.cpp
	Verify/VerifyThread.h
	)
	
set(NetMsgHandle_SRCS
	NetMsgHandle/ByCmdMsg.h
	NetMsgHandle/CmdType.h
	NetMsgHandle/GameObjectHandle.cpp
	NetMsgHandle/GameObjectHandle.h
	NetMsgHandle/NetDispHandle.h
	NetMsgHandle/NetMsgHandleManager.cpp
	NetMsgHandle/NetMsgHandleManager.h
	NetMsgHandle/NetMsgHandleManagerBase.cpp
	NetMsgHandle/NetMsgHandleManagerBase.h
	NetMsgHandle/NullUserCmd.h
	NetMsgHandle/ObjectCmd.h
	NetMsgHandle/UtilMsg.cpp
	NetMsgHandle/UtilMsg.h
	)
	
set(PCH_SRCS
	pchdef.cpp
    pchdef.h
	)

set(LIBRARY_SRCS
	${Object_SRCS}
    ${Server_SRCS}
	${World_SRCS}
	${Verify_SRCS}
	${NetMsgHandle_SRCS}
	${Server_SRCS}
	${PCH_SRCS}
   )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Verify
  ${CMAKE_CURRENT_SOURCE_DIR}/NetMsgHandle
  ${CMAKE_SOURCE_DIR}/dep/include/g3dlite
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/
  ${CMAKE_SOURCE_DIR}/dep/include
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/FastDelegate
  ${CMAKE_SOURCE_DIR}/src/shared/DataStruct
  ${CMAKE_SOURCE_DIR}/src/shared/Network
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/src/shared
  ${MYSQL_INCLUDE_DIR}
)

source_group("Object" FILES ${Object_SRCS})
source_group("World" FILES ${World_SRCS})
source_group("Server" FILES ${Server_SRCS})
source_group("Verify" FILES ${Verify_SRCS})
source_group("NetMsgHandle" FILES ${NetMsgHandle_SRCS})
source_group("" FILES ${PCH_SRCS})

if(PCH)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SRCS}
)

target_link_libraries(${LIBRARY_NAME}
  shared
  detour
)

if(WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

if(UNIX)
  # Both systems don't have libdl and don't need them
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(${LIBRARY_NAME}
      dl
    )
  endif()
endif()

add_dependencies(${LIBRARY_NAME} revision.h)

# Generate precompiled header
if(PCH)
  if(MSVC OR XCODE)
    if(MSVC)
      set(game_pch "${CMAKE_CURRENT_SOURCE_DIR}/pchdef.cpp")
    endif()
    add_native_precompiled_header(${LIBRARY_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pchdef.h)
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_precompiled_header(${LIBRARY_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pchdef.h)
  endif()
endif()
