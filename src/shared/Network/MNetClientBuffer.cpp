#include "MNetClientBuffer.h"
#include "MMsgBuffer.h"
#include "DynBuffer.h"
#include "MByteBuffer.h"
#include "MCircularBuffer.h"
#include "BufferDefaultValue.h"
#include "MClientThreadSafeData.h"

MNetClientBuffer::MNetClientBuffer()
	: m_canSend(true)
{
	m_recvSocketBuffer = new MMsgBuffer();
	m_recvClientBuffer = new MMsgBuffer();
	//m_recvSocketDynBuffer = new DynBuffer(INITCAPACITY);	// ͬһ���̹߳��������

	m_sendClientBuffer = new MCircularBuffer(INITCAPACITY);
	m_sendSocketBuffer = new MCircularBuffer(INITCAPACITY);
	//m_sendClientBA = new MByteBuffer(INITCAPACITY);	// ͬһ���̹߳��������

	m_unCompressHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pHeaderBA = new MByteBuffer(MSGHEADERSIZE);
	m_pMsgBA = new DynBuffer(INITCAPACITY);

	m_pMutex = new boost::mutex();
}

MNetClientBuffer::~MNetClientBuffer()
{
	delete m_recvSocketBuffer;
	delete m_recvClientBuffer;
	//delete m_recvSocketDynBuffer;

	delete m_sendClientBuffer;
	delete m_sendSocketBuffer;
	//delete m_sendClientBA;

	delete m_unCompressHeaderBA;

	delete m_pMutex;
}

void MNetClientBuffer::setRecvMsgSize(size_t len)
{
	m_recvSocketDynBuffer->setCapacity(len);
}

void MNetClientBuffer::moveRecvSocketDyn2RecvSocket(size_t dynLen)
{
	m_recvSocketDynBuffer->setSize(dynLen);
	m_recvSocketBuffer->m_pMCircularBuffer->pushBack(m_recvSocketDynBuffer->m_storage, 0, m_recvSocketDynBuffer->size());

	//moveRecvSocket2RecvClient();
}

// �п���һ�����ݰ��ж����Ϣ������ط�û�д�������ж����Ϣ����Ҫ��������ᶪʧ��Ϣ
void MNetClientBuffer::moveRecvSocket2RecvClient()
{
	while (m_recvSocketBuffer->checkHasMsg())  // ���������
	{
		m_recvSocketBuffer->moveOutOneMsg();
		//m_recvSocketBuffer->m_pMsgBA->uncompress();
		m_unCompressHeaderBA->clear();
		m_unCompressHeaderBA->writeUnsignedInt32(m_recvSocketBuffer->m_pMsgBA->size());
		m_unCompressHeaderBA->pos(0);
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_unCompressHeaderBA->contents(), 0, MSGHEADERSIZE);             // ������Ϣ��С�ֶ�
		m_recvClientBuffer->m_pMCircularBuffer->pushBack((char*)m_recvSocketBuffer->m_pMsgBA->contents(), 0, m_recvSocketBuffer->m_pMsgBA->size());      // ������Ϣ��С�ֶ�
	}
}

MByteBuffer* MNetClientBuffer::getMsg()
{
	if (m_recvClientBuffer->checkHasMsg())
	{
		m_recvClientBuffer->moveOutOneMsg();
		return m_recvClientBuffer->m_pMsgBA;
	}

	return nullptr;
}

void MNetClientBuffer::onReadComplete(size_t dynLen)
{
	moveRecvSocketDyn2RecvSocket(dynLen);		// ���������Ϣ��������
}

void MNetClientBuffer::sendMsg(MByteBuffer* sendClientBA)
{
	m_pHeaderBA->clear();
	m_pHeaderBA->writeUnsignedInt32(sendClientBA->size());      // ��䳤��

	m_pMutex->lock();

	m_sendClientBuffer->pushBack((char*)m_pHeaderBA->contents(), 0, m_pHeaderBA->size());
	m_sendClientBuffer->pushBack((char*)sendClientBA->contents(), 0, sendClientBA->size());

	m_pMutex->unlock();

	sendClientBA->clear();
}

// ��ȡ���ݣ�Ȼ��ѹ������
void MNetClientBuffer::moveSendClient2SendSocket()
{
	m_sendSocketBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��

	m_pMutex->lock();

	m_pMsgBA->setSize(m_sendClientBuffer->size());
	m_sendClientBuffer->popFront(m_pMsgBA->getStorage(), 0, m_sendClientBuffer->size());

	m_pMutex->unlock();

	m_sendSocketBuffer->pushBack(m_pMsgBA->getStorage(), 0, m_pMsgBA->size());
	m_sendClientBuffer->clear(); // �����������λ������ֿ��Դ� 0 ������ʼ��
}

bool MNetClientBuffer::startAsyncSend()
{
	if (!m_canSend || m_sendClientBuffer->size() == 0)
	{
		return false;
	}

	m_canSend = false;		// ���ò��ܷ��ͱ�־
	moveSendClient2SendSocket();		// ������Ϣ���ݣ��ȴ�����

	return true;
}

void MNetClientBuffer::onWriteComplete(size_t len)
{
	if (len < m_sendSocketBuffer->size())		// �������û�з������
	{
		m_sendSocketBuffer->popFrontLenNoData(len);
	}
	else				// ���ȫ���������
	{
		m_canSend = true;		// ���ò��ܷ��ͱ�־
		m_sendSocketBuffer->clear();	// ��������
	}
}

// ������ж���߳�ȥ���ղ�ͬ�� socket ����
void MNetClientBuffer::setRecvSocketBufferTSData(MClientThreadSafeData* tsData)
{
	m_recvSocketBuffer->setHeaderBATSData(tsData);
	m_recvSocketBuffer->setMsgBATSData(tsData);
	
	m_recvSocketDynBuffer = tsData->m_recvSocketDynBuffer;
}

// �����������һ�����̷߳�����Ϣ
void MNetClientBuffer::setRecvClientBufferTSData(MClientThreadSafeData* tsData)
{
	m_recvClientBuffer->setHeaderBATSData(tsData);
	m_recvClientBuffer->setMsgBATSData(tsData);

	//m_sendClientBA = tsData->m_sendClientBA;
}