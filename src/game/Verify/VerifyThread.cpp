#include "VerifyThread.h"
#include "Database/DatabaseEnv.h"
#include "Socket.h"
#include "MClientThreadSafeData.h"
#include "MNetClientBuffer.h"

VerifyThread::VerifyThread() :
	m_Connections(0)
{

}

VerifyThread::~VerifyThread()
{
	Stop();
	Wait();
}

void VerifyThread::Stop()
{
	Wait();
}

void VerifyThread::Start()
{
	m_thread.reset(new boost::thread(boost::bind(&NetworkThread::svc, this)));
}

void VerifyThread::Wait()
{
	if (m_thread.get())
	{
		m_thread->join();
		m_thread.reset();
	}
}

void VerifyThread::AddSocket(const SocketPtr& sock)
{
	++m_Connections;

	boost::lock_guard<boost::mutex> lock(m_SocketsLock);
	m_Sockets.insert(sock);
}

void VerifyThread::RemoveSocket(const SocketPtr& sock)
{
	--m_Connections;

	boost::lock_guard<boost::mutex> lock(m_SocketsLock);
	m_Sockets.erase(sock);
}

void VerifyThread::svc()
{
	// ·¢ËÍÏûÏ¢
	SocketSet::iterator itBegin = m_Sockets.begin();
	SocketSet::iterator itEnd = m_Sockets.end();
	for (; itBegin != itEnd; ++itBegin)
	{
		(*itBegin)->start_async_send();
	}
}