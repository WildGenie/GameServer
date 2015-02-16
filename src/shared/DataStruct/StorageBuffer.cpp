#include "StorageBuffer.h"
#include "DynBufResizePolicy.h"
#include <string.h>

StorageBuffer::StorageBuffer(std::size_t len)
	: m_size(0), m_iCapacity(len)
{
	m_storage = new char[len];
}

StorageBuffer::~StorageBuffer()
{
	delete[] m_storage;
}

void StorageBuffer::setSize(std::size_t len)
{
	m_size = len;

	if (m_size > m_iCapacity)
	{
		setCapacity(m_size);
	}
}

void StorageBuffer::setCapacity(std::size_t newCapacity)
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
bool StorageBuffer::canAddData(uint32 num)
{
	if (m_iCapacity - m_size >= num)
	{
		return true;
	}

	return false;
}