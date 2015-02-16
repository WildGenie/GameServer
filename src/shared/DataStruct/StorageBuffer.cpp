#include "StorageBuffer.h"
#include "DynBufResizePolicy.h"
#include <string.h>

StorageBuffer::StorageBuffer(std::size_t len)
	: m_iCapacity(len)
{
	m_storage = new char[len];
}

StorageBuffer::~StorageBuffer()
{
	delete[] m_storage;
}

std::size_t StorageBuffer::size()
{
	return m_size;
}

size_t StorageBuffer::capacity()
{
	return m_iCapacity;
}

void StorageBuffer::setSize(std::size_t len)
{
	m_size = len;

	if (m_size > capacity())
	{
		setCapacity(m_size);
	}
}

void StorageBuffer::setCapacity(std::size_t newCapacity)
{
	if (newCapacity <= capacity())
	{
		return;
	}

	char* tmpbuff = new char[newCapacity];   // 分配新的空间
	memcpy(tmpbuff, m_storage, m_iCapacity);
	m_iCapacity = newCapacity;

	delete[] m_storage;
	m_storage = tmpbuff;
}

char* StorageBuffer::getStorage()
{
	return m_storage;
}