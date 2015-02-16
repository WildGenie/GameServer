#include "MMsgBuffer.h"
#include "MCircularBuffer.h"
#include "MByteBuffer.h"
#include "BufferDefaultValue.h"
#include "MClientThreadSafeData.h"
#include "MClientProcessData.h"

MMsgBuffer::MMsgBuffer()
{
	m_pMCircularBuffer = new MCircularBuffer(INIT_CAPACITY);
	//m_pHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);		// 这两个字段需要线程共享
	//m_pMsgBA = new MByteBuffer(INIT_CAPACITY);			// 这两个字段需要线程共享
}

MMsgBuffer::~MMsgBuffer()
{
	delete m_pMCircularBuffer;
	//delete m_pHeaderBA;
	//delete m_pMsgBA;
}

bool MMsgBuffer::checkHasMsg()
{
	if (m_pMCircularBuffer->size() <= MSG_HEADER_SIZE)
	{
		return false;
	}

	m_pHeaderBA->clear();
	m_pMCircularBuffer->front((char*)m_pHeaderBA->getStorage(), 0, MSG_HEADER_SIZE);
	m_pHeaderBA->setSize(MSG_HEADER_SIZE);

	m_pHeaderBA->readUnsignedInt32(m_msgLen);
	if (m_msgLen + MSG_HEADER_SIZE <= m_pMCircularBuffer->size())
	{
		return true;
	}

	return false;
}

// 移出一个消息
void MMsgBuffer::moveOutOneMsg()
{
	m_pMCircularBuffer->popFrontLenNoData(MSG_HEADER_SIZE);

	m_pMsgBA->setCapacity(m_msgLen);
	m_pMsgBA->clear();

	m_pMCircularBuffer->popFront((char*)m_pMsgBA->getStorage(), 0, m_msgLen);
	m_pMsgBA->setSize(m_msgLen);
}

void MMsgBuffer::setHeaderBATSData(MClientThreadSafeData* tsData)
{
	m_pHeaderBA = tsData->m_pHeaderBA;
}

void MMsgBuffer::setMsgBATSData(MClientThreadSafeData* tsData)
{
	m_pMsgBA = tsData->m_pMsgBA;
}

void MMsgBuffer::setHeaderBAProcessData(MClientProcessData* pMClientProcessData)
{
	m_pHeaderBA = pMClientProcessData->m_pHeaderBA;
}

void MMsgBuffer::setMsgBAProcessData(MClientProcessData* pMClientProcessData)
{
	m_pMsgBA = pMClientProcessData->m_pMsgBA;
}