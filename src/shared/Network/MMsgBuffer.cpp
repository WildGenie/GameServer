#include "MMsgBuffer.h"
#include "MCircularBuffer.h"
#include "MByteBuffer.h"
#include "BufferDefaultValue.h"
#include "MClientThreadSafeData.h"

MMsgBuffer::MMsgBuffer()
{
	m_pMCircularBuffer = new MCircularBuffer(INITCAPACITY);
	//m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);		// 这两个字段需要线程共享
	//m_pMsgBA = new MByteBuffer(INITCAPACITY);			// 这两个字段需要线程共享
}

MMsgBuffer::~MMsgBuffer()
{
	delete m_pMCircularBuffer;
	//delete m_pHeaderBA;
	//delete m_pMsgBA;
}

bool MMsgBuffer::checkHasMsg()
{
	if (m_pMCircularBuffer->size() <= MSGHEADERSIZE)
	{
		return false;
	}

	m_pHeaderBA->clear();
	m_pMCircularBuffer->front((char*)m_pHeaderBA->contents(), 0, MSGHEADERSIZE);
	m_pHeaderBA->setSize(MSGHEADERSIZE);

	m_pHeaderBA->readUnsignedInt32(m_msgLen);
	if (m_msgLen + MSGHEADERSIZE <= m_pMCircularBuffer->size())
	{
		return true;
	}

	return false;
}

// 移出一个消息
void MMsgBuffer::moveOutOneMsg()
{
	m_pMCircularBuffer->popFrontLenNoData(MSGHEADERSIZE);

	m_pMsgBA->setCapacity(m_msgLen);
	m_pMsgBA->clear();

	m_pMCircularBuffer->popFront((char*)m_pMsgBA->contents(), 0, m_msgLen);
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