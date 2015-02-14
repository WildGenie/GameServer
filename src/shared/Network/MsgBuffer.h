#ifndef __MSGBUFFER_H
#define __MSGBUFFER_H

class MCircularBuffer;
class ByteBuffer;

/**
 * @brief 消息缓冲区
 */
class MsgBuffer
{
	friend class NetClientBuffer;

protected:
	MCircularBuffer* m_pMCircularBuffer;
	ByteBuffer* m_pHeaderBA;	// 主要是用来分析头的大小的四个字节
	ByteBuffer* m_pMsgBA;       // 返回的字节数组，没有消息长度的一个完整的消息
	uint32 m_msgLen;			// 一个消息的长度

public:
	MsgBuffer();
	~MsgBuffer();
	bool checkHasMsg();
	void moveOutOneMsg();
};

#endif				// __MSGBUFFER_H