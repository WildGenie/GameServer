#include "MDynBufResizePolicy.h"
#include "MBufferDefaultValue.h"

// 获取一个最近的大小
uint32 MDynBufResizePolicy::getCloseSize(uint32 needSize, uint32 capacity)
{
	uint32 ret = 0;

	if (needSize <= MAX_CAPACITY)
	{
		if (capacity > needSize)		// 使用 > ，不适用 >= ，浪费一个自己，方便判断
		{
			ret = capacity;
		}
		else
		{
			ret = 2 * capacity;
			while (ret < needSize && ret < MAX_CAPACITY)
			{
				ret *= 2;
			}

			if (ret > MAX_CAPACITY)
			{
				ret = MAX_CAPACITY;
			}
		}
	}
	else	// 需要的数据比允许最大允许分配的大小还大
	{
		// 日志，需要的存储空间太大
	}

	return ret;
}