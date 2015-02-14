#include "NetClientBuffer.h"
#include "MsgBuffer.h"
#include "DynBuffer.h"
#include "MByteBuffer.h"
#include "MCircularBuffer.h"
#include "BufferDefaultValue.h"

NetClientBuffer::NetClientBuffer()
	： m_canSend(true)
{
	m_recvSocketBuffer = new MsgBuffer();
	m_recvClientBuffer = new MsgBuffer();
	m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);

	m_sendClientBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendSocketBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendClientBA = new MByteBuffer(INITCAPACITY);

	m_unCompressHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new DynBuffer(INITCAPACITY);

	m_pMutex = new boost::mutex();
}

NetClientBuffer::~NetClientBuffer()
{
	delete m_recvSocketBuffer;
	delete m_recvClientBuffer;
	delete m_recvSocketDynBuffer;

	delete m_sendClientBuffer;
	delete m_sendSocketBuffer;
	delete m_sendClientBA;

	delete m_unCompressHeaderBA;

	delete m_pMutex;
}

void NetClientBuffer::setRecvMsgSize(size_t len)
{
	m_recvSocketDynBuffer->setCapacity(len);
}

void NetClientBuffer::moveRecvSocketDyn2RecvSocket(size_t dynLen)
{
	m_recvSocketDynBuffer->setSize(dynLen);
	m_recvSocketBuffer->m_pMCircularBuffer->pushBack(m_recvSocketDynBuffer->m_storage, 0, m_recvSocketDynBuffer->size());
}

// 有可能一个数据包有多个消息，这个地方没有处理，如果有多个消息，需要处理，否则会丢失消息
void NetClientBuffer::moveRecvSocket2RecvClient()
{
	while (m_recvSocketBuffer->checkHasMsg())  // 如果有数据
	{
		m_recvSocketBuffer->moveOutOneMsg();
		//m_recvSocketBuffer->m_pMsgBA->uncompress();
		m_unCompressHeaderBA->clear();
		m_unCompressHeaderBA->writeUnsignedInt32(m_recvSocketBuffer->m_pMsgBA->size());
		m_unCompressHeaderBA->rpos(0);
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_unCompressHeaderBA->contents(), 0, MSGHEADERSIZE);             // 保存消息大小字段
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_recvSocketBuffer->m_pMsgBA->contents(), 0, m_recvSocketBuffer->m_pMsgBA->size());      // 保存消息大小字段
	}
}

MByteBuffer* NetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void NetClientBuffer::onReadComplete(size_t dynLen)
{
	moveRecvSocketDyn2RecvSocket(dynLen);		// 放入接收消息处理缓冲区
}

void NetClientBuffer::sendMsg()
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(m_sendClientBA->size());      // 填充长度

	m_pMutex->lock();

	m_sendClientBuffer->pushBack((char*)m_pHeaderBA->contents(), 0, m_pHeaderBA->size());
	m_sendClientBuffer->pushBack((char*)m_sendClientBA->contents(), 0, m_sendClientBA->size());

	m_pMutex->unlock();
}

// 获取数据，然后压缩加密
void NetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // 清理，这样环形缓冲区又可以从 0 索引开始了

	m_pMutex->lock();

	m_pMsgBA->setSize(m_sendClientBuffer->size());
	m_sendClientBuffer->popFront(m_pMsgBA->getStorage(), 0, m_sendClientBuffer->size());

	m_pMutex->unlock();

	m_sendSocketBuffer->pushBack(m_pMsgBA->getStorage(), 0, m_pMsgBA->size());
	m_sendClientBuffer->clear(); // 清理，这样环形缓冲区又可以从 0 索引开始了
}

bool NetClientBuffer::startAsyncSend()
{
	if (!m_canSend || m_sendClientBuffer->size() == 0)
	{
		return false;
	}

	m_canSend = false;		// 设置不能发送标志
	moveSendClient2SendSocket();		// 处理消息数据，等待发送

	return true;
}

void NetClientBuffer::onWriteComplete(size_t len)
{
	if (len < m_pNetClientBuffer->m_sendSocketBuffer->size())		// 如果数据没有发送完成
	{
		m_pNetClientBuffer->m_sendSocketBuffer->popFrontLenNoData(len);
	}
	else				// 如果全部发送完成
	{
		m_canSend = true;		// 设置不能发送标志
		m_pNetClientBuffer->m_sendSocketBuffer->clear();	// 清理数据
	}
}