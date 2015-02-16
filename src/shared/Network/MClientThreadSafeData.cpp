#include "MClientThreadSafeData.h"
#include "MByteBuffer.h"
#include "DynBuffer.h"
#include "BufferDefaultValue.h"

MClientThreadSafeData::MClientThreadSafeData()
{
	m_pHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);
	m_pMsgBA = new MByteBuffer(INIT_CAPACITY);
	m_recvSocketDynBuffer = new DynBuffer(INIT_CAPACITY);
}

MClientThreadSafeData::~MClientThreadSafeData()
{
	delete m_pHeaderBA;
	delete m_pMsgBA;

	//if (m_recvSocketDynBuffer)
	//{
		delete m_recvSocketDynBuffer;
	//}
}

//void MClientThreadSafeData::newRecvSocketDynBuffer()
//{
//	m_recvSocketDynBuffer = new DynBuffer(INIT_CAPACITY);
//}

//void MClientThreadSafeData::newSendClientBA()
//{
//	m_sendClientBA = new MByteBuffer(INIT_CAPACITY);
//}