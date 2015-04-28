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

#include "NetworkThread.h"
#include "Database/DatabaseEnv.h"
#include "Socket.h"
#include "MClientThreadSafeData.h"
#include "MNetClientBuffer.h"

NetworkThread::NetworkThread() :
	m_Connections(0), m_pSocketBufferTSData(new MClientThreadSafeData()), m_bExitFlag(false)
{
    m_work.reset( new protocol::Service::work(m_networkingService));

	//m_pSocketBufferTSData->newRecvSocketDynBuffer();
}

NetworkThread::~NetworkThread()
{
    Stop();
    Wait();
}

void NetworkThread::Stop()
{
    m_work.reset();
    m_networkingService.stop();

    Wait();
}

void NetworkThread::Start()
{
    m_thread.reset(new boost::thread(boost::bind(&NetworkThread::svc, this)));
	m_sendThread.reset(new boost::thread(boost::bind(&NetworkThread::sendAndRecvData, this)));
}

void NetworkThread::Wait()
{
    if(m_thread.get())
    {
        m_thread->join();
        m_thread.reset();
    }

	if (m_sendThread.get())
	{
		m_sendThread->join();
		m_sendThread.reset();
	}
}

void NetworkThread::AddSocket( const SocketPtr& sock )
{
    ++m_Connections;

    boost::lock_guard<boost::mutex> lock(m_SocketsLock);
    m_Sockets.insert(sock);

	// 初始化 socket 的缓冲区
	sock->getNetClientBuffer()->setRecvSocketBufferTSData(m_pSocketBufferTSData);
	sock->getNetClientBuffer()->setRecvClientProcessData(m_pMClientProcessData);
}

void NetworkThread::RemoveSocket( const SocketPtr& sock )
{
    --m_Connections;

    boost::lock_guard<boost::mutex> lock(m_SocketsLock);
    m_Sockets.erase(sock);
}

void NetworkThread::svc()
{
    DEBUG_LOG("Network Thread Starting");

    LoginDatabase.ThreadStart();

    m_networkingService.run();

    LoginDatabase.ThreadEnd();

    DEBUG_LOG("Network Thread Exitting");
}

void NetworkThread::sendAndRecvData()
{
	while (!m_bExitFlag)
	{
		boost::lock_guard<boost::mutex> lock(m_SocketsLock);
		// 发送消息
		SocketSet::iterator itBegin = m_Sockets.begin();
		SocketSet::iterator itEnd = m_Sockets.end();
		for (; itBegin != itEnd; ++itBegin)
		{
			//(*itBegin)->getNetClientBuffer()->moveRecvSocket2RecvClient();		// 将数据放入接收缓冲区中
			if ((*itBegin)->getNetClientBuffer()->canSendData() && (*itBegin)->getNetClientBuffer()->hasSendData())		// 如果 Client 不能发送数据
			{
				(*itBegin)->start_async_send();					// 发送数据
			}
		}
	}
}

void NetworkThread::setMClientProcessData(MClientProcessData* pMClientProcessData)
{
	m_pMClientProcessData = pMClientProcessData;
}