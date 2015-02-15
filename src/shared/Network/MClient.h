#ifndef __MCLIENT_H
#define __MCLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class MNetClientBuffer;

class MClient
{
public:
	MClient(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator);
	void close();

	void start_async_read();
	void start_async_send();

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator);

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	MNetClientBuffer* m_pNetClientBuffer;
};

#endif