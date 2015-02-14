#include "MyProject.h"
#include "NetClientBuffer.h"
#include "MsgBuffer.h"
#include "DynBuffer.h"
#include "ByteBuffer.h"
#include "MCircularBuffer.h"
#include "BufferDefaultValue.h"

#include "Windows/AllowWindowsPlatformTypes.h"

#include "Sockets/Mutex.h"

#include "Windows/HideWindowsPlatformTypes.h"

NetClientBuffer::NetClientBuffer()
{
	m_recvSocketBuffer = new MsgBuffer();
	m_recvClientBuffer = new MsgBuffer();
	m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);

	m_sendClientBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendSocketBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendClientBA = new ByteBuffer(INITCAPACITY);

	m_unCompressHeaderBA = new ByteBuffer(MSGHEADERSIZE);
	m_pHeaderBA = new ByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new DynBuffer(INITCAPACITY);

	m_pMutex = new Mutex();
}

NetClientBuffer::~NetClientBuffer()
{
	delete m_recvSocketBuffer;
	delete m_recvClientBuffer;
	delete m_recvSocketDynBuffer;

	delete m_sendClientBuffer;
	delete m_sendSocketBuffer;
	delete m_sendClientBA;

	delete m_unCompressHeaderBA;

	delete m_pMutex;
}

void NetClientBuffer::moveRecvSocketDyn2RecvSocket()
{
	m_recvSocketBuffer->m_pMCircularBuffer->pushBack(m_recvSocketDynBuffer->m_storage, 0, m_recvSocketDynBuffer->size());
}

// �п���һ�����ݰ��ж����Ϣ������ط�û�д�������ж����Ϣ����Ҫ��������ᶪʧ��Ϣ
void NetClientBuffer::moveRecvSocket2RecvClient()
{
	while (m_recvSocketBuffer->checkHasMsg())  // ���������
	{
		m_recvSocketBuffer->moveOutOneMsg();
		//m_recvSocketBuffer->m_pMsgBA->uncompress();
		m_unCompressHeaderBA->clear();
		m_unCompressHeaderBA->writeUnsignedInt32(m_recvSocketBuffer->m_pMsgBA->size());
		m_unCompressHeaderBA->rpos(0);
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_unCompressHeaderBA->contents(), 0, MSGHEADERSIZE);             // ������Ϣ��С�ֶ�
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_recvSocketBuffer->m_pMsgBA->contents(), 0, m_recvSocketBuffer->m_pMsgBA->size());      // ������Ϣ��С�ֶ�
	}
}

ByteBuffer* NetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void NetClientBuffer::sendMsg()
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(m_sendClientBA->size());      // ��䳤��

	m_pMutex->Lock();

	m_sendClientBuffer->pushBack((char*)m_pHeaderBA->contents(), 0, m_pHeaderBA->size());
	m_sendClientBuffer->pushBack((char*)m_sendClientBA->contents(), 0, m_sendClientBA->size());

	m_pMutex->Unlock();
}

// ��ȡ���ݣ�Ȼ��ѹ������
void NetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��

	m_pMutex->Lock();

	m_pMsgBA->setSize(m_sendClientBuffer->size());
	m_sendClientBuffer->popFront(m_pMsgBA->getStorage(), 0, m_sendClientBuffer->size());

	m_pMutex->Unlock();

	m_sendSocketBuffer->pushBack(m_pMsgBA->getStorage(), 0, m_pMsgBA->size());
	m_sendClientBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��
}