#ifndef __MCLIENT_H
#define __MCLIENT_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class NetClientBuffer;

class MClient
{
public:
	MClient(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator);
	void close();

private:
	void do_connect(tcp::resolver::iterator endpoint_iterator);
	void start_async_read();
	void start_async_send();

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	NetClientBuffer* m_pNetClientBuffer;
};

#endif