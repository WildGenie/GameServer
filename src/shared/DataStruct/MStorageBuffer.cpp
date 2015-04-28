#include "MStorageBuffer.h"
#include "MDynBufResizePolicy.h"
#include <string.h>

MStorageBuffer::MStorageBuffer(std::size_t len)
	: m_size(0), m_iCapacity(len)
{
	m_storage = new char[len];
}

MStorageBuffer::~MStorageBuffer()
{
	delete[] m_storage;
}

void MStorageBuffer::setSize(std::size_t len)
{
	m_size = len;

	if (m_size > m_iCapacity)
	{
		setCapacity(m_size);
	}
}

void MStorageBuffer::setCapacity(std::size_t newCapacity)
{
	if (newCapacity <= m_size)       // 不能分配比当前已经占有的空间还小的空间
	{
		return;
	}

	if (newCapacity <= m_iCapacity)
	{
		return;
	}

	char* tmpbuff = new char[newCapacity];   // 分配新的空间
	memcpy(tmpbuff, m_storage, m_iCapacity);
	m_iCapacity = newCapacity;

	delete[] m_storage;
	m_storage = tmpbuff;
}

/**
*@brief 能否添加 num 长度的数据
*/
bool MStorageBuffer::canAddData(uint32 num)
{
	if (m_iCapacity - m_size > num)		// 浪费一个字节，不用 >= ，使用 > 
	{
		return true;
	}

	return false;
}