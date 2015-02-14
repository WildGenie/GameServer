#include "MyProject.h"
#include "DynBufResizePolicy.h"
#include "BufferDefaultValue.h"

// ��ȡһ������Ĵ�С
uint32 DynBufResizePolicy::getCloseSize(uint32 needSize, uint32 capacity)
{
	uint32 ret = 0;

	if (needSize <= MAXCAPACITY)
	{
		if (capacity >= needSize)
		{
			ret = capacity;
		}
		else
		{
			ret = 2 * capacity;
			while (ret < needSize && ret < MAXCAPACITY)
			{
				ret *= 2;
			}

			if (ret > MAXCAPACITY)
			{
				ret = MAXCAPACITY;
			}
		}
	}
	else	// ��Ҫ�����ݱ���������������Ĵ�С����
	{
		// ��־����Ҫ�Ĵ洢�ռ�̫��
	}

	return ret;
}