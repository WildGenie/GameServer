#include "MClientProcessData.h"
#include "MByteBuffer.h"
#include "DynBuffer.h"
#include "BufferDefaultValue.h"

MClientProcessData::MClientProcessData()
{
	m_pHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);
	m_pMsgBA = new MByteBuffer(INIT_CAPACITY);
	m_sendClientBA = new MByteBuffer(INIT_CAPACITY);
}

MClientProcessData::~MClientProcessData()
{
	delete m_pHeaderBA;
	delete m_pMsgBA;
	delete m_sendClientBA;
}