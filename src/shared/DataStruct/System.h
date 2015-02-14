#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "Singleton.h"

enum SysEndian;

class System : public Singleton<System>
{
protected:
	SysEndian m_sysEndian;				// ����ϵͳ��С��
public:
	void checkEndian();		// ��Ȿ��ϵͳ�Ǵ�˻���С��
	bool isEndianDiffFromSys(SysEndian rhv);	// �����ϵͳ�Ĵ��С���Ƿ���ͬ
};

#endif