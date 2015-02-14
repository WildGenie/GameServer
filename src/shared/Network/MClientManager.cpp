#include "NetworkThread.h"
#include "Database/DatabaseEnv.h"

MClientManager::MClientManager() :
	m_Connections(0)
{
	
}

MClientManager::~MClientManager()
{
	
}

void MClientManager::Stop()
{
	
}

void MClientManager::Start()
{
	
}

void MClientManager::Wait()
{
	
}

void MClientManager::AddSocket(const SocketPtr& sock)
{
	++m_Connections;

	boost::lock_guard<boost::mutex> lock(m_SocketsLock);
	m_Sockets.insert(sock);
}

void MClientManager::RemoveSocket(const SocketPtr& sock)
{
	--m_Connections;

	boost::lock_guard<boost::mutex> lock(m_SocketsLock);
	m_Sockets.erase(sock);
}

void MClientManager::svc()
{
	// ·¢ËÍÏûÏ¢
	SocketSet::iterator itBegin = m_Sockets.begin();
	SocketSet::iterator itEnd = m_Sockets.end();
	for (; itBegin != itEnd; ++itBegin)
	{
		(*itBegin)->start_async_send();
	}
}