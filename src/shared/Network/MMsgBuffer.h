#ifndef __MSGBUFFER_H
#define __MSGBUFFER_H

class MCircularBuffer;
class MByteBuffer;
class MClientThreadSafeData;
class MClientProcessData;

#include "Platform/Define.h"

/**
 * @brief 消息缓冲区
 */
class MMsgBuffer
{
	friend class MNetClientBuffer;

protected:
	MCircularBuffer* m_pMCircularBuffer;
	MByteBuffer* m_pHeaderBA;	// 主要是用来分析头的大小的四个字节.其实如果是 MNetClientBuffer 中的 m_recvSocketBuffer ，同一个线程中所有 MNetClientBuffer 共享同一个这个变量
	MByteBuffer* m_pMsgBA;       // 返回的字节数组，没有消息长度的一个完整的消息.其实如果是 MNetClientBuffer 中的 m_recvSocketBuffer ，同一个线程中所有 MNetClientBuffer 共享同一个这个变量
	uint32 m_msgLen;			// 一个消息的长度

public:
	MMsgBuffer();
	~MMsgBuffer();
	bool checkHasMsg();
	void moveOutOneMsg();

	// 线程共享安全数据
	void setHeaderBATSData(MClientThreadSafeData* tsData);
	void setMsgBATSData(MClientThreadSafeData* tsData);

	// 进程共享数据
	void setHeaderBAProcessData(MClientProcessData* pMClientProcessData);
	void setMsgBAProcessData(MClientProcessData* pMClientProcessData);
	void getHeaderSize(uint32& msgLen);
};

#endif				// __MSGBUFFER_H