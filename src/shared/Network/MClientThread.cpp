#include "MClientThread.h"

MClientThread::MClientThread() :
	m_Connections(0)
{
	m_work.reset(new protocol::Service::work(m_networkingService));
}

MClientThread::~MClientThread()
{
	Stop();
	Wait();
}

void MClientThread::Stop()
{
	m_work.reset();
	m_networkingService.stop();

	Wait();
}

void MClientThread::Start()
{
	m_thread.reset(new boost::thread(boost::bind(&MClientThread::svc, this)));
}

void MClientThread::Wait()
{
	if (m_thread.get())
	{
		m_thread->join();
		m_thread.reset();
	}
}

void MClientThread::svc()
{
	
}