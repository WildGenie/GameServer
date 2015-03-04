#include "MDynBuffer.h"
#include "MDynBufResizePolicy.h"
#include <string.h>
#include "StorageBuffer.h"

MDynBuffer::MDynBuffer(std::size_t len)
{
	m_pStorageBuffer = new StorageBuffer(len);
}

MDynBuffer::~MDynBuffer()
{
	delete[] m_pStorageBuffer;
}

std::size_t MDynBuffer::size()
{
	return m_pStorageBuffer->m_size;
}

size_t MDynBuffer::capacity()
{
	return m_pStorageBuffer->m_iCapacity;
}

void MDynBuffer::setSize(std::size_t len)
{
	m_pStorageBuffer->setSize(len);
}

void MDynBuffer::setCapacity(std::size_t newCapacity)
{
	m_pStorageBuffer->setCapacity(newCapacity);
}

char* MDynBuffer::getStorage()
{
	return m_pStorageBuffer->m_storage;
}

void MDynBuffer::push(char* pItem, std::size_t len)
{
	if (len > m_pStorageBuffer->m_iCapacity)
	{
		uint32 closeSize = MDynBufResizePolicy::getCloseSize(len, capacity());
		setCapacity(closeSize);
	}

	memcpy(m_pStorageBuffer->m_storage, pItem, len);
	m_pStorageBuffer->m_size = len;
}