#include "MClientThread.h"

MClientThread::MClientThread() :
	m_Connections(0)
{
	m_work.reset(new protocol::Service::work(m_networkingService));
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
}

void NetworkThread::Wait()
{
	if (m_thread.get())
	{
		m_thread->join();
		m_thread.reset();
	}
}

void NetworkThread::svc()
{
	DEBUG_LOG("Network Thread Starting");

	LoginDatabase.ThreadStart();

	m_networkingService.run();

	LoginDatabase.ThreadEnd();

	// ·¢ËÍÏûÏ¢
	SocketSet::iterator itBegin = m_Sockets.begin();
	SocketSet::iterator itEnd = m_Sockets.end();
	for (; itBegin != itEnd; ++itBegin)
	{
		(*itBegin)->start_async_send();
	}

	DEBUG_LOG("Network Thread Exitting");
}