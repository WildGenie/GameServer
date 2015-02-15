#include "MClientThreadSafeData.h"
#include "MByteBuffer.h"
#include "DynBuffer.h"
#include "BufferDefaultValue.h"

MClientThreadSafeData::MClientThreadSafeData()
{
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new MByteBuffer(INITCAPACITY);
}

MClientThreadSafeData::~MClientThreadSafeData()
{

}

void MClientThreadSafeData::newRecvSocketDynBuffer()
{
	m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);
}

void MClientThreadSafeData::newSendClientBA()
{
	m_sendClientBA = new MByteBuffer(INITCAPACITY);
}