#ifndef __MCLIENTMANAGER_H
#define __MCLIENTMANAGER_H

#include "ProtocolDefinitions.h"

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <set>

class MClientManager : public boost::noncopyable
{
public:
	MClientManager();

	virtual ~MClientManager();

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
	typedef std::set<SocketPtr> SocketSet;

	boost::atomic_long m_Connections;
	std::auto_ptr<boost::thread> m_thread;

	SocketSet m_Sockets;
	boost::mutex m_SocketsLock;
};


#endif