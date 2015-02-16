#ifndef __MCLIENTPROCESSDATA_H
#define __MCLIENTPROCESSDATA_H

class MByteBuffer;
class DynBuffer;

/**
 * @brief 基于进程 socket buffer 数据，同一个进程共享这些数据
 */
class MClientProcessData
{
public:
	MByteBuffer* m_pHeaderBA;		// 主要是用来分析头的大小的四个字节.
	MByteBuffer* m_pMsgBA;			// 返回的字节数组，没有消息长度的一个完整的消息.

	MByteBuffer* m_sendClientBA;		// 存放将要发送的临时数据，将要放到 m_sendClientBuffer 中去

public:
	MClientProcessData();
	~MClientProcessData();
};

#endif