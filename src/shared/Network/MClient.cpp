#include "MClient.h"
#include "MNetClientBuffer.h"
#include "MCircularBuffer.h"
#include "MDynBuffer.h"
#include "MByteBuffer.h"

MClient::MClient(boost::asio::io_service& io_service,
	tcp::resolver::iterator endpoint_iterator)
	: io_service_(io_service),
	socket_(io_service)
{
	do_connect(endpoint_iterator);
}

void MClient::close()
{
	io_service_.post([this]() { socket_.close(); });
}

void MClient::do_connect(tcp::resolver::iterator endpoint_iterator)
{
	boost::asio::async_connect(socket_, endpoint_iterator,
		[this](boost::system::error_code ec, tcp::resolver::iterator)
	{
		if (!ec)
		{
			start_async_read();
		}
	});
}

void MClient::start_async_read()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(m_pNetClientBuffer->m_recvSocketDynBuffer->getStorage(), m_pNetClientBuffer->m_recvSocketDynBuffer->capacity()),
		[this](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			m_pNetClientBuffer->startAsyncSend();		// 放入接收消息处理缓冲区
		}
		else
		{
			socket_.close();
		}
	});
}

void MClient::start_async_send()
{
	if (!m_pNetClientBuffer->startAsyncSend())	// 如果 Client 不能发送数据
	{
		return;
	}

	boost::asio::async_write(socket_,
		boost::asio::buffer(m_pNetClientBuffer->m_sendSocketBuffer->getStorage(), m_pNetClientBuffer->m_sendSocketBuffer->size()),
		[this](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			m_pNetClientBuffer->onWriteComplete(length);

			start_async_send();
		}
		else
		{
			socket_.close();
		}
	});
}