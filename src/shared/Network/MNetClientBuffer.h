#ifndef __NETCLIENTBUFFER_H
#define __NETCLIENTBUFFER_H

class MMsgBuffer;
class DynBuffer;
class MByteBuffer;
class MCircularBuffer;
class Mutex;
class MClientThreadSafeData;
class MClientProcessData;

#include <boost/thread/thread.hpp>

/**
* @brief ��Ϣ������
*/
class MNetClientBuffer
{
public:
	// ���յ� Buffer
	MMsgBuffer* m_recvSocketBuffer;		// ֱ�Ӵӷ��������յ���ԭʼ�����ݣ�����ѹ���ͼ��ܹ����߳���ִ��
	MMsgBuffer* m_recvClientBuffer;		// ��ѹ���ܺ����ʹ�õĻ����������̻߳���ĳ�����߳���ִ��
	DynBuffer* m_recvSocketDynBuffer;	// ���յ�����ʱ���ݣ���Ҫ�ŵ� m_recvRawBuffer ��ȥ��ͬһ���߳������� MNetClientBuffer ����ͬһ���������

	// ���͵� Buffer
	MCircularBuffer* m_sendClientBuffer;		// ������ʱ�����������͵����ݶ���ʱ��������
	MCircularBuffer* m_sendSocketBuffer;		// ���ͻ�������ѹ�����߼��ܹ���
	//MByteBuffer* m_sendClientBA;			// ��Ž�Ҫ���͵���ʱ���ݣ���Ҫ�ŵ� m_sendClientBuffer ��ȥ

	MByteBuffer* m_unCompressHeaderBA;  // ��Ž�ѹ���ͷ�ĳ���
	MByteBuffer* m_pHeaderBA;	// д���ĸ��ֽ�ͷ��
	DynBuffer* m_pMsgBA;		// ������Ϣ��ʱ����������С��������

	boost::mutex* m_pMutex;
	bool m_canSend;		// �첽���͵Ĺ����У���ǰ���͵������Ƿ������

public:
	MNetClientBuffer();
	~MNetClientBuffer();

	// ����
	void moveRecvSocketDyn2RecvSocket(size_t dynLen);
	void moveRecvSocket2RecvClient();
	MByteBuffer* getMsg();
	void onReadComplete(size_t dynLen);

	// ����
	void sendMsg(MByteBuffer* sendClientBA);
	void moveSendClient2SendSocket();
	void setRecvMsgSize(size_t len);
	bool startAsyncSend();
	void onWriteComplete(size_t len);

	// �����̰߳�ȫ������
	void setRecvSocketBufferTSData(MClientThreadSafeData* tsData);
	void setRecvClientProcessData(MClientProcessData* pMClientProcessData);
};

#endif				// __NETCLIENTBUFFER_H