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

set(LIBRARY_NAME framework)

cmake_policy(SET CMP0043 NEW)

set(Dynamic_SRCS
    Dynamic/FactoryHolder.h
    Dynamic/ObjectRegistry.h
	)
	
set(GameSystem_SRCS
    GameSystem/Grid.h
    GameSystem/GridLoader.h
    GameSystem/GridReference.h
    GameSystem/GridRefManager.h
    GameSystem/NGrid.h
    GameSystem/TypeContainer.h
    GameSystem/TypeContainerFunctions.h
    GameSystem/TypeContainerVisitor.h
	)
	
set(LinkedReference_SRCS
    Utilities/LinkedReference/Reference.h
    Utilities/LinkedReference/RefManager.h
	)
	
set(Platform_SRCS
    Platform/CompilerDefs.h
    Platform/Define.h
	)
	
set(Policies_SRCS
	Policies/CreationPolicy.h
    Policies/MemoryManagement.cpp
    Policies/ObjectLifeTime.cpp
    Policies/ObjectLifeTime.h
    Policies/Singleton.h
    Policies/ThreadingModel.h
	)
	
set(Utilities_SRCS
    Utilities/ByteConverter.h
    Utilities/Callback.h
    Utilities/EventProcessor.cpp
    Utilities/EventProcessor.h
    Utilities/LinkedList.h

    Utilities/TypeList.h
    Utilities/UnorderedMapSet.h
	)
	
set(LIBRARY_SRCS
	${Dynamic_SRCS}
	${GameSystem_SRCS}
	${LinkedReference_SRCS}
	${Platform_SRCS}
	${Policies_SRCS}
	${Utilities_SRCS}
   )

source_group("Other"
  REGULAR_EXPRESSION .*
)

source_group("GameSystem"
  REGULAR_EXPRESSION GameSystem
)

source_group("Platform"
  REGULAR_EXPRESSION Platform
)

source_group("Policies"
  REGULAR_EXPRESSION Policies
)

source_group("Utilities"
  REGULAR_EXPRESSION Utilities
)

source_group("LinkedReference"
  REGULAR_EXPRESSION LinkedReference
)

source_group("Dynamic"
  REGULAR_EXPRESSION Dynamic
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SRCS}
)

if(NOT NEDMALLOC_USE_EXTERNAL)
  add_dependencies(${LIBRARY_NAME} nedmalloc)
  include_directories(${CMAKE_SOURCE_DIR}/dep/nedmalloc)
endif()

target_link_libraries(${LIBRARY_NAME}
  ${TBB_LIBRARIES}
)
