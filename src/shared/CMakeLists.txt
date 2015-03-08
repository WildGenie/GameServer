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

set(LIBRARY_NAME shared)

cmake_policy(SET CMP0043 NEW)

set(Auth_SRCS
    Auth/AuthCrypt.cpp
    Auth/AuthCrypt.h
    Auth/BigNumber.cpp
    Auth/BigNumber.h
    Auth/HMACSHA1.cpp
    Auth/HMACSHA1.h
    Auth/md5.h
    Auth/SARC4.cpp
    Auth/SARC4.h
    Auth/Sha1.cpp
    Auth/Sha1.h
	)
	
set(Config_SRCS
    Config/Config.cpp
    Config/Config.h
	)
	
set(Database_SRCS
    Database/Database.cpp
    Database/Database.h
    Database/DatabaseEnv.h
    Database/DatabaseImpl.h
    Database/DatabaseMysql.cpp
    Database/DatabaseMysql.h
    Database/DatabasePostgre.cpp
    Database/DatabasePostgre.h
    Database/Field.cpp
    Database/Field.h
    Database/PGSQLDelayThread.h
    Database/QueryResult.h
    Database/QueryResultMysql.cpp
    Database/QueryResultMysql.h
    Database/QueryResultPostgre.cpp
    Database/QueryResultPostgre.h
    Database/SqlDelayThread.cpp
    Database/SqlDelayThread.h
    Database/SqlOperations.cpp
    Database/SqlOperations.h
    Database/SqlPreparedStatement.cpp
    Database/SqlPreparedStatement.h
    Database/SQLStorage.cpp
    Database/SQLStorage.h
    Database/SQLStorageImpl.h
	)

set(DatabaseDBC_SRCS
	Database/DBCFileLoader.cpp
    Database/DBCFileLoader.h
    Database/DBCStore.h
	)
	
set(DataStruct_SRCS
	DataStruct/MBufferDefaultValue.h
	DataStruct/MBufferUtil.h
	DataStruct/MBufferUtil.cpp
	DataStruct/MByteBuffer.h
	DataStruct/MByteBuffer.cpp
	DataStruct/MByteConverter.h
	DataStruct/MCircularBuffer.cpp
	DataStruct/MCircularBuffer.h
	DataStruct/MDynBuffer.cpp
	DataStruct/MDynBuffer.h
	DataStruct/MDynBufResizePolicy.cpp
	DataStruct/MDynBufResizePolicy.h
	DataStruct/MStorageBuffer.cpp
	DataStruct/MStorageBuffer.h
	DataStruct/MSystemEndian.h
	DataStruct/Singleton.h
	DataStruct/System.cpp
	DataStruct/System.h
	)
	
set(Log_SRCS
    Log/Log.cpp
    Log/Log.h
	)
	
set(Network_SRCS
	Network/ProtocolDefinitions.h
	Network/Socket.cpp
	Network/Socket.h
	Network/NetworkThread.cpp
	Network/NetworkThread.h
	Network/NetworkManager.cpp
	Network/NetworkManager.h
	
	Network/MClient.cpp
	Network/MClient.h
	Network/MClientManager.cpp
	Network/MClientManager.h
	Network/MClientProcessData.cpp
	Network/MClientProcessData.h
	Network/MClientThread.cpp
	Network/MClientThread.h
	Network/MClientThreadSafeData.cpp
	Network/MClientThreadSafeData.h
	Network/MMsgBuffer.cpp
	Network/MMsgBuffer.h
	Network/MNetClientBuffer.cpp
	Network/MNetClientBuffer.h
	)
	
set(Util_SRCS
	Util/ByteBuffer.cpp
    Util/ByteBuffer.h
    Util/Errors.h
    Util/ProgressBar.cpp
	Util/ProgressBar.h
    Util/Timer.h
    Util/Util.cpp
    Util/Util.h
	Util/WarnPop.h
	Util/WarnPush.h
	Util/Encrypt.h
	Util/Encrypt.h
	Util/MCompress.h
	Util/MCompress.h
	)
	
set(FastDelegate_SRCS
	FastDelegate/FastDelegate.h
	FastDelegate/FastDelegateBind.h
	)
	
set(Thread_SRCS
    Thread/Threading.cpp
    Thread/Threading.h
	)
	
set(Common_SRCS
    Common.cpp
    Common.h
	LockedQueue.h
	Version.h
    SystemConfig.h
	)

# OS specific files
if(WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  
  set(Platform_SRCS
      Platform/ServiceWin32.cpp
      Platform/ServiceWin32.h
     )
else()
  set(Platform_SRCS
      Platform/PosixDaemon.cpp
      Platform/PosixDaemon.h
     )
endif()

set(LIBRARY_SRCS
	${Auth_SRCS}
	${Config_SRCS}
	${Database_SRCS}
	${DatabaseDBC_SRCS}
	${DataStruct_SRCS}
	${Log_SRCS}
	${Network_SRCS}
	${Util_SRCS}
	${FastDelegate}
	${Thread_SRCS}
	${Platform_SRCS}
	${Common_SRCS}
   )

source_group("Auth" FILES ${Auth_SRCS})
source_group("Config" FILES ${Config_SRCS})
source_group("Database" FILES ${Database_SRCS})
source_group("Database\\DataStores" FILES ${DatabaseDBC_SRCS})
source_group("DataStruct" FILES ${DataStruct_SRCS})
source_group("Log" FILES ${Log_SRCS})
source_group("Network" FILES ${Network_SRCS})
source_group("Util" FILES ${Util_SRCS})
source_group("FastDelegate" FILES ${FastDelegate_SRCS})
source_group("Thread" FILES ${Thread_SRCS})
source_group("Platform" FILES ${Platform_SRCS})
source_group("" FILES ${Common_SRCS})

# TODO: really needed?
foreach(SRC ${LIBRARY_SRCS})
  get_filename_component(PTH ${SRC} PATH)
  if(PTH)
    if(NOT XCODE) # FIXME: Xcode Generator has bug with nested dirs
      string(REPLACE "/" "\\\\" PTH ${PTH})
    endif()
    source_group(${PTH} FILES ${SRC})
  endif()
endforeach(SRC)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/DataStruct
  ${CMAKE_CURRENT_SOURCE_DIR}/FastDelegate
  ${CMAKE_CURRENT_SOURCE_DIR}/Log
  ${CMAKE_CURRENT_SOURCE_DIR}/Thread
  ${CMAKE_CURRENT_SOURCE_DIR}/Util
  ${CMAKE_CURRENT_SOURCE_DIR}/Platform
  ${CMAKE_SOURCE_DIR}/dep/include
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_BINARY_DIR}
  ${MYSQL_INCLUDE_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SystemConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/SystemConfig.h)

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SRCS}
)