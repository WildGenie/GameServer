#ifndef __NETCLIENTBUFFER_H
#define __NETCLIENTBUFFER_H

class MsgBuffer;
class DynBuffer;
class ByteBuffer;
class MCircularBuffer;
class Mutex;

/**
* @brief 消息缓冲区
*/
class NetClientBuffer
{
protected:
	// 接收的 Buffer
	MsgBuffer* m_recvSocketBuffer;		// 直接从服务器接收到的原始的数据，可能压缩和加密过
	MsgBuffer* m_recvClientBuffer;		// 解压解密后可以使用的缓冲区
	DynBuffer* m_recvSocketDynBuffer;	// 接收到的临时数据，将要放到 m_recvRawBuffer 中去

	// 发送的 Buffer
	MCircularBuffer* m_sendClientBuffer;		// 发送临时缓冲区，发送的数据都暂时放在这里
	MCircularBuffer* m_sendSocketBuffer;		// 发送缓冲区，压缩或者加密过的
	ByteBuffer* m_sendClientBA;			// 存放将要发送的临时数据，将要放到 m_sendClientBuffer 中去

	ByteBuffer* m_unCompressHeaderBA;  // 存放解压后的头的长度
	ByteBuffer* m_pHeaderBA;	// 写入四个字节头部
	DynBuffer* m_pMsgBA;		// 发送消息临时缓冲区，减小加锁粒度

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