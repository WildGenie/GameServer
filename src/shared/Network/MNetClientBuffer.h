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
* @brief 消息缓冲区
*/
class MNetClientBuffer
{
public:
	// 接收的 Buffer
	MMsgBuffer* m_recvSocketBuffer;		// 直接从服务器接收到的原始的数据，可能压缩和加密过，线程中执行
	MMsgBuffer* m_recvClientBuffer;		// 解压解密后可以使用的缓冲区，主线程或者某个子线程中执行
	DynBuffer* m_recvSocketDynBuffer;	// 接收到的临时数据，将要放到 m_recvRawBuffer 中去，同一个线程中所有 MNetClientBuffer 共享同一个这个变量

	// 发送的 Buffer
	MCircularBuffer* m_sendClientBuffer;		// 发送临时缓冲区，发送的数据都暂时放在这里
	MCircularBuffer* m_sendSocketBuffer;		// 发送缓冲区，压缩或者加密过的
	//MByteBuffer* m_sendClientBA;			// 存放将要发送的临时数据，将要放到 m_sendClientBuffer 中去

	MByteBuffer* m_unCompressHeaderBA;  // 存放解压后的头的长度
	MByteBuffer* m_pHeaderBA;	// 写入四个字节头部
	DynBuffer* m_pMsgBA;		// 发送消息临时缓冲区，减小加锁粒度

	boost::mutex* m_pMutex;
	bool m_canSend;		// 异步发送的过程中，当前发送的数据是否发送完成

public:
	MNetClientBuffer();
	~MNetClientBuffer();

	// 接收
	void moveRecvSocketDyn2RecvSocket(size_t dynLen);
	void moveRecvSocket2RecvClient();
	MByteBuffer* getMsg();
	void onReadComplete(size_t dynLen);

	// 发送
	void sendMsg(MByteBuffer* sendClientBA);
	void moveSendClient2SendSocket();
	void setRecvMsgSize(size_t len);
	bool startAsyncSend();
	void onWriteComplete(size_t len);

	// 设置线程安全的数据
	void setRecvSocketBufferTSData(MClientThreadSafeData* tsData);
	void setRecvClientProcessData(MClientProcessData* pMClientProcessData);
};

#endif				// __NETCLIENTBUFFER_H