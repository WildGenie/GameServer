#ifndef __MSGBUFFER_H
#define __MSGBUFFER_H

class MCircularBuffer;
class MByteBuffer;
class MClientThreadSafeData;

#include "Platform/Define.h"

/**
 * @brief ��Ϣ������
 */
class MMsgBuffer
{
	friend class MNetClientBuffer;

protected:
	MCircularBuffer* m_pMCircularBuffer;
	MByteBuffer* m_pHeaderBA;	// ��Ҫ����������ͷ�Ĵ�С���ĸ��ֽ�.��ʵ����� MNetClientBuffer �е� m_recvSocketBuffer ��ͬһ���߳������� MNetClientBuffer ����ͬһ���������
	MByteBuffer* m_pMsgBA;       // ���ص��ֽ����飬û����Ϣ���ȵ�һ����������Ϣ.��ʵ����� MNetClientBuffer �е� m_recvSocketBuffer ��ͬһ���߳������� MNetClientBuffer ����ͬһ���������
	uint32 m_msgLen;			// һ����Ϣ�ĳ���

public:
	MMsgBuffer();
	~MMsgBuffer();
	bool checkHasMsg();
	void moveOutOneMsg();

	// �̰߳�ȫ����
	void setHeaderBATSData(MClientThreadSafeData* tsData);
	void setMsgBATSData(MClientThreadSafeData* tsData);
};

#endif				// __MSGBUFFER_H