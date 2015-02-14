#include "MyProject.h"
#include "DynBuffer.h"
#include "DynBufResizePolicy.h"
#include <string.h>

DynBuffer::DynBuffer(std::size_t len)
	: m_iCapacity(len)
{
	m_storage = new char[len];
}

DynBuffer::~DynBuffer()
{
	delete[] m_storage;
}

std::size_t DynBuffer::size()
{
	return m_size;
}

size_t DynBuffer::capacity()
{
	return m_iCapacity;
}

void DynBuffer::setSize(std::size_t len)
{
	m_size = len;

	if (m_size > capacity())
	{
		setCapacity(m_size);
	}
}

void DynBuffer::setCapacity(std::size_t newCapacity)
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

char* DynBuffer::getStorage()
{
	return m_storage;
}

void DynBuffer::push(char* pItem, std::size_t len)
{
	if (len > m_iCapacity)
	{
		uint32 closeSize = DynBufResizePolicy::getCloseSize(len, capacity());
		setCapacity(closeSize);
	}

	memcpy(m_storage, pItem, len);
	m_size = len;
}