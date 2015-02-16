#include "MNetClientBuffer.h"
#include "MMsgBuffer.h"
#include "DynBuffer.h"
#include "MByteBuffer.h"
#include "MCircularBuffer.h"
#include "BufferDefaultValue.h"
#include "MClientThreadSafeData.h"

MNetClientBuffer::MNetClientBuffer()
	: m_canSend(true)
{
	m_recvSocketBuffer = new MMsgBuffer();
	m_recvClientBuffer = new MMsgBuffer();
	//m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);	// 同一个线程共享的数据

	m_sendClientBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendSocketBuffer = new MCircularBuffer(INITCAPACITY);
	//m_sendClientBA = new MByteBuffer(INITCAPACITY);	// 同一个线程共享的数据

	m_unCompressHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new DynBuffer(INITCAPACITY);

	m_pMutex = new boost::mutex();
}

MNetClientBuffer::~MNetClientBuffer()
{
	delete m_recvSocketBuffer;
	delete m_recvClientBuffer;
	//delete m_recvSocketDynBuffer;

	delete m_sendClientBuffer;
	delete m_sendSocketBuffer;
	//delete m_sendClientBA;

	delete m_unCompressHeaderBA;

	delete m_pMutex;
}

void MNetClientBuffer::setRecvMsgSize(size_t len)
{
	m_recvSocketDynBuffer->setCapacity(len);
}

void MNetClientBuffer::moveRecvSocketDyn2RecvSocket(size_t dynLen)
{
	m_recvSocketDynBuffer->setSize(dynLen);
	m_recvSocketBuffer->m_pMCircularBuffer->pushBack(m_recvSocketDynBuffer->m_storage, 0, m_recvSocketDynBuffer->size());

	//moveRecvSocket2RecvClient();
}

// 有可能一个数据包有多个消息，这个地方没有处理，如果有多个消息，需要处理，否则会丢失消息
void MNetClientBuffer::moveRecvSocket2RecvClient()
{
	while (m_recvSocketBuffer->checkHasMsg())  // 如果有数据
	{
		m_recvSocketBuffer->moveOutOneMsg();
		//m_recvSocketBuffer->m_pMsgBA->uncompress();
		m_unCompressHeaderBA->clear();
		m_unCompressHeaderBA->writeUnsignedInt32(m_recvSocketBuffer->m_pMsgBA->size());
		m_unCompressHeaderBA->pos(0);
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_unCompressHeaderBA->contents(), 0, MSGHEADERSIZE);             // 保存消息大小字段
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_recvSocketBuffer->m_pMsgBA->contents(), 0, m_recvSocketBuffer->m_pMsgBA->size());      // 保存消息大小字段
	}
}

MByteBuffer* MNetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		m_recvClientBuffer->moveOutOneMsg();
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void MNetClientBuffer::onReadComplete(size_t dynLen)
{
	moveRecvSocketDyn2RecvSocket(dynLen);		// 放入接收消息处理缓冲区
}

void MNetClientBuffer::sendMsg(MByteBuffer* sendClientBA)
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(sendClientBA->size());      // 填充长度

	m_pMutex->lock();

	m_sendClientBuffer->pushBack((char*)m_pHeaderBA->contents(), 0, m_pHeaderBA->size());
	m_sendClientBuffer->pushBack((char*)sendClientBA->contents(), 0, sendClientBA->size());

	m_pMutex->unlock();

	sendClientBA->clear();
}

// 获取数据，然后压缩加密
void MNetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // 清理，这样环形缓冲区又可以从 0 索引开始了

	m_pMutex->lock();

	m_pMsgBA->setSize(m_sendClientBuffer->size());
	m_sendClientBuffer->popFront(m_pMsgBA->getStorage(), 0, m_sendClientBuffer->size());

	m_pMutex->unlock();

	m_sendSocketBuffer->pushBack(m_pMsgBA->getStorage(), 0, m_pMsgBA->size());
	m_sendClientBuffer->clear(); // 清理，这样环形缓冲区又可以从 0 索引开始了
}

bool MNetClientBuffer::startAsyncSend()
{
	if (!m_canSend || m_sendClientBuffer->size() == 0)
	{
		return false;
	}

	m_canSend = false;		// 设置不能发送标志
	moveSendClient2SendSocket();		// 处理消息数据，等待发送

	return true;
}

void MNetClientBuffer::onWriteComplete(size_t len)
{
	if (len < m_sendSocketBuffer->size())		// 如果数据没有发送完成
	{
		m_sendSocketBuffer->popFrontLenNoData(len);
	}
	else				// 如果全部发送完成
	{
		m_canSend = true;		// 设置不能发送标志
		m_sendSocketBuffer->clear();	// 清理数据
	}
}

// 这个会有多个线程去接收不同的 socket 数据
void MNetClientBuffer::setRecvSocketBufferTSData(MClientThreadSafeData* tsData)
{
	m_recvSocketBuffer->setHeaderBATSData(tsData);
	m_recvSocketBuffer->setMsgBATSData(tsData);
	
	m_recvSocketDynBuffer = tsData->m_recvSocketDynBuffer;
}

// 这个仅仅会有一个主线程发送消息
void MNetClientBuffer::setRecvClientBufferTSData(MClientThreadSafeData* tsData)
{
	m_recvClientBuffer->setHeaderBATSData(tsData);
	m_recvClientBuffer->setMsgBATSData(tsData);

	//m_sendClientBA = tsData->m_sendClientBA;
}