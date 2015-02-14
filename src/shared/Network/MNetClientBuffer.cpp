#include "NetClientBuffer.h"
#include "MsgBuffer.h"
#include "DynBuffer.h"
#include "MByteBuffer.h"
#include "MCircularBuffer.h"
#include "BufferDefaultValue.h"

NetClientBuffer::NetClientBuffer()
	�� m_canSend(true)
{
	m_recvSocketBuffer = new MsgBuffer();
	m_recvClientBuffer = new MsgBuffer();
	m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);

	m_sendClientBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendSocketBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendClientBA = new MByteBuffer(INITCAPACITY);

	m_unCompressHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new DynBuffer(INITCAPACITY);

	m_pMutex = new boost::mutex();
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

void NetClientBuffer::setRecvMsgSize(size_t len)
{
	m_recvSocketDynBuffer->setCapacity(len);
}

void NetClientBuffer::moveRecvSocketDyn2RecvSocket(size_t dynLen)
{
	m_recvSocketDynBuffer->setSize(dynLen);
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

MByteBuffer* NetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void NetClientBuffer::onReadComplete(size_t dynLen)
{
	moveRecvSocketDyn2RecvSocket(dynLen);		// ���������Ϣ��������
}

void NetClientBuffer::sendMsg()
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(m_sendClientBA->size());      // ��䳤��

	m_pMutex->lock();

	m_sendClientBuffer->pushBack((char*)m_pHeaderBA->contents(), 0, m_pHeaderBA->size());
	m_sendClientBuffer->pushBack((char*)m_sendClientBA->contents(), 0, m_sendClientBA->size());

	m_pMutex->unlock();
}

// ��ȡ���ݣ�Ȼ��ѹ������
void NetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��

	m_pMutex->lock();

	m_pMsgBA->setSize(m_sendClientBuffer->size());
	m_sendClientBuffer->popFront(m_pMsgBA->getStorage(), 0, m_sendClientBuffer->size());

	m_pMutex->unlock();

	m_sendSocketBuffer->pushBack(m_pMsgBA->getStorage(), 0, m_pMsgBA->size());
	m_sendClientBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��
}

bool NetClientBuffer::startAsyncSend()
{
	if (!m_canSend || m_sendClientBuffer->size() == 0)
	{
		return false;
	}

	m_canSend = false;		// ���ò��ܷ��ͱ�־
	moveSendClient2SendSocket();		// ������Ϣ���ݣ��ȴ�����

	return true;
}

void NetClientBuffer::onWriteComplete(size_t len)
{
	if (len < m_pNetClientBuffer->m_sendSocketBuffer->size())		// �������û�з������
	{
		m_pNetClientBuffer->m_sendSocketBuffer->popFrontLenNoData(len);
	}
	else				// ���ȫ���������
	{
		m_canSend = true;		// ���ò��ܷ��ͱ�־
		m_pNetClientBuffer->m_sendSocketBuffer->clear();	// ��������
	}
}