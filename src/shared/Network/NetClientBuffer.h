#ifndef __NETCLIENTBUFFER_H
#define __NETCLIENTBUFFER_H

class MsgBuffer;
class DynBuffer;
class MByteBuffer;
class MCircularBuffer;
class Mutex;

#include <boost/thread/thread.hpp>

/**
* @brief ��Ϣ������
*/
class NetClientBuffer
{
public:
	// ���յ� Buffer
	MsgBuffer* m_recvSocketBuffer;		// ֱ�Ӵӷ��������յ���ԭʼ�����ݣ�����ѹ���ͼ��ܹ�
	MsgBuffer* m_recvClientBuffer;		// ��ѹ���ܺ����ʹ�õĻ�����
	DynBuffer* m_recvSocketDynBuffer;	// ���յ�����ʱ���ݣ���Ҫ�ŵ� m_recvRawBuffer ��ȥ

	// ���͵� Buffer
	MCircularBuffer* m_sendClientBuffer;		// ������ʱ�����������͵����ݶ���ʱ��������
	MCircularBuffer* m_sendSocketBuffer;		// ���ͻ�������ѹ�����߼��ܹ���
	MByteBuffer* m_sendClientBA;			// ��Ž�Ҫ���͵���ʱ���ݣ���Ҫ�ŵ� m_sendClientBuffer ��ȥ

	MByteBuffer* m_unCompressHeaderBA;  // ��Ž�ѹ���ͷ�ĳ���
	MByteBuffer* m_pHeaderBA;	// д���ĸ��ֽ�ͷ��
	DynBuffer* m_pMsgBA;		// ������Ϣ��ʱ����������С��������

	boost::mutex* m_pMutex;

public:
	NetClientBuffer();
	~NetClientBuffer();

	void moveRecvSocketDyn2RecvSocket();
	void moveRecvSocket2RecvClient();
	MByteBuffer* getMsg();

	void sendMsg();
	void moveSendClient2SendSocket();
	void setRecvMsgSize(size_t len);
};

#endif				// __NETCLIENTBUFFER_H