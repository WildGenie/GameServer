#include "MNetClientBuffer.h"
#include "MMsgBuffer.h"
#include "MDynBuffer.h"
#include "MByteBuffer.h"
#include "MCircularBuffer.h"
#include "MBufferDefaultValue.h"
#include "MClientThreadSafeData.h"
#include "MClientProcessData.h"

MNetClientBuffer::MNetClientBuffer()
{
	m_recvSocketBuffer = new MMsgBuffer();
	m_recvClientBuffer = new MMsgBuffer();
	//m_recvSocketDynBuffer = new MDynBuffer(INIT_CAPACITY);	// 同一个线程共享的数据

	m_sendClientBuffer = new MByteBuffer(INIT_CAPACITY);
	m_sendSocketBuffer = new MByteBuffer(INIT_CAPACITY);
	//m_sendClientBA = new MByteBuffer(INIT_CAPACITY);	// 同一个线程共享的数据

	m_unCompressHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);
	m_pHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);

	m_pSendMutex = new boost::mutex();
	m_pRevMutex = new boost::mutex();
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

	delete m_pSendMutex;
	delete m_pRevMutex;
}

void MNetClientBuffer::setRecvMsgSize(size_t len)
{
	m_recvSocketDynBuffer->setCapacity(len);
}

void MNetClientBuffer::moveRecvSocketDyn2RecvSocket(size_t dynLen)
{
	m_recvSocketDynBuffer->setSize(dynLen);
	m_recvSocketBuffer->m_pMCircularBuffer->pushBack(m_recvSocketDynBuffer->getStorage(), 0, m_recvSocketDynBuffer->size());

	//moveRecvSocket2RecvClient();
}

// 有可能一个数据包有多个消息，这个地方没有处理，如果有多个消息，需要处理，否则会丢失消息
void MNetClientBuffer::moveRecvSocket2RecvClient()
{
	while (m_recvSocketBuffer->checkHasMsg())  // 如果有数据
	{
		UnCompressAndDecryptEveryOne();
	}
}

MByteBuffer* MNetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		m_pRevMutex->lock();
		m_recvClientBuffer->moveOutOneMsg();
		m_pRevMutex->unlock();
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void MNetClientBuffer::onReadComplete(size_t dynLen)
{
	moveRecvSocketDyn2RecvSocket(dynLen);		// 放入接收消息处理缓冲区
	moveRecvSocket2RecvClient();
}

void MNetClientBuffer::sendMsg(MByteBuffer* sendClientBA)
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(sendClientBA->size());      // 填充长度

	m_pSendMutex->lock();

	m_sendClientBuffer->writeUnsignedInt32(sendClientBA->size());
	m_sendClientBuffer->writeBytes((char*)sendClientBA->getStorage(), 0, sendClientBA->size());

	m_pSendMutex->unlock();

	sendClientBA->clear();
}

// 获取数据，然后压缩加密
void MNetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // 清理，这样环形缓冲区又可以从 0 索引开始了
	m_sendClient2SocketBuffer->m_pMCircularBuffer->clear();

	m_pSendMutex->lock();

	m_sendClient2SocketBuffer->m_pMCircularBuffer->pushBack((char*)m_sendClientBuffer->getStorage(), 0, m_sendClientBuffer->size());
	m_sendClientBuffer->clear();	// 清理，这样环形缓冲区又可以从 0 索引开始了

	m_pSendMutex->unlock();

	m_sendSocketBuffer->writeBytes(m_sendClient2SocketBuffer->m_pMCircularBuffer->getStorage(), 0, m_sendClient2SocketBuffer->m_pMCircularBuffer->size());
	m_sendSocketBuffer->pos(0);				// 设置位置为初始位置
}

bool MNetClientBuffer::startAsyncSend()
{
	if (m_sendClientBuffer->size() > 0 && m_sendSocketBuffer->size() == 0)		// 只有缓冲区为空的时候才能拷贝数据，如果不为空，就是有数据正在发送
	{
		moveSendClient2SendSocket();		// 处理消息数据，等待发送
	}

	if (m_sendSocketBuffer->size() == 0)		// 如果还是没有数据
	{
		return false;
	}

	return true;
}

void MNetClientBuffer::onWriteComplete(size_t len)
{
	if (len < m_sendSocketBuffer->avaliableBytes())		// 如果数据没有发送完成
	{
		m_sendSocketBuffer->pos(len);
	}
	else				// 如果全部发送完成
	{
		m_sendSocketBuffer->clear();	// 清理数据
	}
}

// 这个会有多个线程去接收不同的 socket 数据
void MNetClientBuffer::setRecvSocketBufferTSData(MClientThreadSafeData* tsData)
{
	m_recvSocketBuffer->setHeaderBATSData(tsData);
	m_recvSocketBuffer->setMsgBATSData(tsData);
	
	m_recvSocketDynBuffer = tsData->m_recvSocketDynBuffer;
	m_sendClient2SocketBuffer = tsData->m_sendClient2SocketBuffer;
}

// 这个仅仅会有一个主线程发送消息
void MNetClientBuffer::setRecvClientProcessData(MClientProcessData* pMClientProcessData)
{
	m_recvClientBuffer->setHeaderBAProcessData(pMClientProcessData);
	m_recvClientBuffer->setMsgBAProcessData(pMClientProcessData);

	//m_sendClientBA = tsData->m_sendClientBA;
}

void MNetClientBuffer::UnCompressAndDecryptEveryOne()
{
	m_recvSocketBuffer->moveOutOneMsg();
	//m_recvSocketBuffer->m_pMsgBA->uncompress();
	m_unCompressHeaderBA->clear();
	m_unCompressHeaderBA->writeUnsignedInt32(m_recvSocketBuffer->m_pMsgBA->size());
	m_unCompressHeaderBA->pos(0);

	m_pRevMutex->lock();

	m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_unCompressHeaderBA->getStorage(), 0, MSG_HEADER_SIZE);             // 保存消息大小字段
	m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_recvSocketBuffer->m_pMsgBA->getStorage(), 0, m_recvSocketBuffer->m_pMsgBA->size());      // 保存消息大小字段

	m_pRevMutex->unlock();
}

bool MNetClientBuffer::canSendData()
{
	if (m_sendSocketBuffer->size() > 0)		// 只有缓冲区为空的时候才能拷贝数据，如果不为空，就是有数据正在发送
	{
		return false;
	}

	return true;
}

bool MNetClientBuffer::hasSendData()
{
	if (m_sendClientBuffer->size() == 0)		// 如果为 0 就说明没有可以发送的数据
	{
		return false;
	}

	return true;
}