#include "MClientProcessData.h"
#include "MByteBuffer.h"
#include "DynBuffer.h"
#include "BufferDefaultValue.h"

MClientProcessData::MClientProcessData()
{
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new MByteBuffer(INITCAPACITY);
	m_sendClientBA = new MByteBuffer(INITCAPACITY);
}

MClientProcessData::~MClientProcessData()
{
	delete m_pHeaderBA;
	delete m_pMsgBA;
	delete m_sendClientBA;
}