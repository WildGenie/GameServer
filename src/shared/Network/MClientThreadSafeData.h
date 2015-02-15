#ifndef __MCLIENTTHREADSAFEDATA_H
#define __MCLIENTTHREADSAFEDATA_H

class MByteBuffer;
class DynBuffer;

/**
 * @brief 基于线程安全的 socket buffer 数据，同一个线程共享这些数据
 */
class MClientThreadSafeData
{
public:
	MByteBuffer* m_pHeaderBA;		// 主要是用来分析头的大小的四个字节.
	MByteBuffer* m_pMsgBA;			// 返回的字节数组，没有消息长度的一个完整的消息.

	// 这两个数据可能有的线程只需要一个数据，因此单独初始化
	DynBuffer* m_recvSocketDynBuffer;	// 接收临时数据
	MByteBuffer* m_sendClientBA;		// 存放将要发送的临时数据，将要放到 m_sendClientBuffer 中去

public:
	MClientThreadSafeData();
	~MClientThreadSafeData();

	void newRecvSocketDynBuffer();
	void newSendClientBA();
};

#endif