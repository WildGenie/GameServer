#include "DynBufResizePolicy.h"
#include "BufferDefaultValue.h"

// 获取一个最近的大小
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
	else	// 需要的数据比允许最大允许分配的大小还大
	{
		// 日志，需要的存储空间太大
	}

	return ret;
}