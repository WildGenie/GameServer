#ifndef __NETCLIENTBUFFER_H
#define __NETCLIENTBUFFER_H

class MsgBuffer;
class DynBuffer;
class ByteBuffer;
class MCircularBuffer;
class Mutex;

/**
* @brief ��Ϣ������
*/
class NetClientBuffer
{
protected:
	// ���յ� Buffer
	MsgBuffer* m_recvSocketBuffer;		// ֱ�Ӵӷ��������յ���ԭʼ�����ݣ�����ѹ���ͼ��ܹ�
	MsgBuffer* m_recvClientBuffer;		// ��ѹ���ܺ����ʹ�õĻ�����
	DynBuffer* m_recvSocketDynBuffer;	// ���յ�����ʱ���ݣ���Ҫ�ŵ� m_recvRawBuffer ��ȥ

	// ���͵� Buffer
	MCircularBuffer* m_sendClientBuffer;		// ������ʱ�����������͵����ݶ���ʱ��������
	MCircularBuffer* m_sendSocketBuffer;		// ���ͻ�������ѹ�����߼��ܹ���
	ByteBuffer* m_sendClientBA;			// ��Ž�Ҫ���͵���ʱ���ݣ���Ҫ�ŵ� m_sendClientBuffer ��ȥ

	ByteBuffer* m_unCompressHeaderBA;  // ��Ž�ѹ���ͷ�ĳ���
	ByteBuffer* m_pHeaderBA;	// д���ĸ��ֽ�ͷ��
	DynBuffer* m_pMsgBA;		// ������Ϣ��ʱ����������С��������

	Mutex* m_pMutex;

public:
	NetClientBuffer();
	~NetClientBuffer();

	void moveRecvSocketDyn2RecvSocket();
	void moveRecvSocket2RecvClient();
	ByteBuffer* getMsg();

	void sendMsg();
	void moveSendClient2SendSocket();
};

#endif				// __NETCLIENTBUFFER_H