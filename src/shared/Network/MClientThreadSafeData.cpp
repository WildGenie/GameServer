#include "MClientThreadSafeData.h"
#include "MByteBuffer.h"
#include "MDynBuffer.h"
#include "MMsgBuffer.h"
#include "MBufferDefaultValue.h"

MClientThreadSafeData::MClientThreadSafeData()
{
	m_pHeaderBA = new MByteBuffer(MSG_HEADER_SIZE);
	m_pMsgBA = new MByteBuffer(INIT_CAPACITY);
	m_recvSocketDynBuffer = new MDynBuffer(INIT_CAPACITY);
	m_sendClient2SocketBuffer = new MMsgBuffer();
}

MClientThreadSafeData::~MClientThreadSafeData()
{
	delete m_pHeaderBA;
	delete m_pMsgBA;
	delete m_recvSocketDynBuffer;
	delete m_sendClient2SocketBuffer;
}

//void MClientThreadSafeData::newRecvSocketDynBuffer()
//{
//	m_recvSocketDynBuffer = new MDynBuffer(INIT_CAPACITY);
//}

//void MClientThreadSafeData::newSendClientBA()
//{
//	m_sendClientBA = new MByteBuffer(INIT_CAPACITY);
//}