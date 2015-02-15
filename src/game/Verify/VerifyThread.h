#ifndef _VERIFYTHREAD_H
#define _VERIFYTHREAD_H

#include "ProtocolDefinitions.h"

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <set>

class MClientThreadSafeData;

class VerifyThread : public boost::noncopyable
{
public:
	VerifyThread();

	virtual ~VerifyThread();

	void Stop();

	void Start();

	void Wait();

	long Connections() const
	{
		return m_Connections;
	}

	void AddSocket(const SocketPtr& sock);

	void RemoveSocket(const SocketPtr& sock);

private:

	virtual void svc();

	typedef std::set<SocketPtr> SocketSet;

	boost::atomic_long m_Connections;
	std::auto_ptr<boost::thread> m_thread;

	SocketSet m_Sockets;
	boost::mutex m_SocketsLock;

	bool m_exitFlag;
};


#endif