#ifndef __MSGBUFFER_H
#define __MSGBUFFER_H

class MCircularBuffer;
class MByteBuffer;

#include "Platform/Define.h"

/**
 * @brief ��Ϣ������
 */
class MMsgBuffer
{
	friend class NetClientBuffer;

protected:
	MCircularBuffer* m_pMCircularBuffer;
	MByteBuffer* m_pHeaderBA;	// ��Ҫ����������ͷ�Ĵ�С���ĸ��ֽ�
	MByteBuffer* m_pMsgBA;       // ���ص��ֽ����飬û����Ϣ���ȵ�һ����������Ϣ
	uint32 m_msgLen;			// һ����Ϣ�ĳ���

public:
	MsgBuffer();
	~MsgBuffer();
	bool checkHasMsg();
	void moveOutOneMsg();
};

#endif				// __MSGBUFFER_H