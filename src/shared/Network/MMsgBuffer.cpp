#include "MMsgBuffer.h"
#include "MCircularBuffer.h"
#include "MByteBuffer.h"
#include "BufferDefaultValue.h"
#include "MClientThreadSafeData.h"

MMsgBuffer::MMsgBuffer()
{
	m_pMCircularBuffer = new MCircularBuffer(INITCAPACITY);
	//m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);		// �������ֶ���Ҫ�̹߳���
	//m_pMsgBA = new MByteBuffer(INITCAPACITY);			// �������ֶ���Ҫ�̹߳���
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

	m_pHeaderBA->readUnsignedInt32(m_msgLen);
	if (m_msgLen + MSGHEADERSIZE <= m_pMCircularBuffer->size())
	{
		return true;
	}

	return false;
}

// �Ƴ�һ����Ϣ
void MMsgBuffer::moveOutOneMsg()
{
	m_pMCircularBuffer->popFrontLenNoData(MSGHEADERSIZE);

	m_pMsgBA->resize(m_msgLen);
	m_pMsgBA->clear();

	m_pMCircularBuffer->popFront((char*)m_pMsgBA->contents(), 0, m_msgLen);
}

void MMsgBuffer::setHeaderBATSData(MClientThreadSafeData* tsData)
{

}

void MMsgBuffer::setMsgBATSData(MClientThreadSafeData* tsData)
{

}