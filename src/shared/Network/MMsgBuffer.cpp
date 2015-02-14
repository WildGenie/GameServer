#include "MsgBuffer.h"
#include "MCircularBuffer.h"
#include "MByteBuffer.h"
#include "BufferDefaultValue.h"

MsgBuffer::MsgBuffer()
{
	m_pMCircularBuffer = new MCircularBuffer(INITCAPACITY);
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new MByteBuffer(INITCAPACITY);
}

MsgBuffer::~MsgBuffer()
{
	delete m_pMCircularBuffer;
	delete m_pHeaderBA;
	delete m_pMsgBA;
}

bool MsgBuffer::checkHasMsg()
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

// 移出一个消息
void MsgBuffer::moveOutOneMsg()
{
	m_pMCircularBuffer->popFrontLenNoData(MSGHEADERSIZE);

	m_pMsgBA->resize(m_msgLen);
	m_pMsgBA->clear();

	m_pMCircularBuffer->popFront((char*)m_pMsgBA->contents(), 0, m_msgLen);
}