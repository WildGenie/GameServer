#ifndef __MCLIENTTHREAD_H
#define __MCLIENTTHREAD_H

#include "ProtocolDefinitions.h"

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <set>

class MClientThread : public boost::noncopyable
{
public:
	MClientThread();

	virtual ~MClientThread();

	void Stop();

	void Start();

	void Wait();

	long Connections() const
	{
		return m_Connections;
	}

	protocol::Service& service()
	{
		return m_networkingService;
	}

private:

	virtual void svc();

	boost::atomic_long m_Connections;
	std::auto_ptr<boost::thread> m_thread;

	protocol::Service m_networkingService;
	std::auto_ptr<protocol::Service::work> m_work;
};


#endif