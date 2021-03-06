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

/** \file
  \ingroup realmd
  */

#ifndef _PATCHHANDLER_H_
#define _PATCHHANDLER_H_

//#include <ace/Basic_Types.h>
//#include <ace/Synch_Traits.h>
//#include <ace/Svc_Handler.h>
//#include <ace/SOCK_Stream.h>
//#include <ace/Message_Block.h>
//#include <ace/Auto_Ptr.h>
#include <map>

#include <openssl/bn.h>
#include <openssl/md5.h>

#include "Policies/Singleton.h"
#include "Network/ProtocolDefinitions.h"
#include <boost/enable_shared_from_this.hpp>
#include <stdio.h>

/**
 * @brief Caches MD5 hash of client patches present on the server
 */
class PatchCache
{
    public:
        ~PatchCache();
        PatchCache();

        struct PATCH_INFO
        {
			// TEST
            //ACE_UINT8 md5[MD5_DIGEST_LENGTH];
			uint8 md5[MD5_DIGEST_LENGTH];
        };

        typedef std::map<std::string, PATCH_INFO*> Patches;

        Patches::const_iterator begin() const
        {
            return patches_.begin();
        }

        Patches::const_iterator end() const
        {
            return patches_.end();
        }

        void LoadPatchMD5(const char*);
        bool GetHash(const char* pat, ACE_UINT8 mymd5[MD5_DIGEST_LENGTH]);

    private:
        void LoadPatchesInfo();
        Patches patches_;
};

#define sPatchCache MaNGOS::Singleton<PatchCache>::Instance()

class PatchHandler : public boost::enable_shared_from_this<PatchHandler>
{
    public:
        PatchHandler(protocol::Socket& socket, ACE_HANDLE patch);
        virtual ~PatchHandler();

        bool open();

    protected:

        void on_timeout( const boost::system::error_code& error);

        void transmit_file();

        void start_async_write();

        void on_write_complete( const boost::system::error_code& error,
                                size_t bytes_transferred );

    private:

        size_t offset() const;

        protocol::Socket& m_socket;
        boost::asio::deadline_timer m_timer;

        //ACE_HANDLE patch_fd_;
		// TEST
		FILE* patch_fd_
        NetworkBuffer m_sendBuffer;
};

typedef boost::shared_ptr<PatchHandler> PatchHandlerPtr;

#endif /* _BK_PATCHHANDLER_H__ */

